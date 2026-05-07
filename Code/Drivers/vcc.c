#include "VCC.h"

/**
	* @brief		获取当前电池电压 (单位: mV)
	* @note			比如获取到3.4v显示为3400
	* @param		无
	* @retval		获取到的电压值,3.4v也就是3400
	*/
uint16_t Get_Battery_Voltage(void)
{
    uint16_t adc_res = 0;
    uint32_t vcc_mv = 0;
    
    //这里的 1275 是 STC15W 系列内部基准电压的标称值 (1.275V = 1275mV)
    uint16_t bandgap_voltage = 1242; 

    //初始化 ADC 寄存器
    ADC_RES = 0;
    ADC_RESL = 0;

    //开启 ADC 电源，并选择通道 8 (内部 Bandgap 电压通道)
    // ADC_CONTR 格式: ADC_POWER | SPEED1 | SPEED0 | ADC_FLAG | ADC_START | CHS2 | CHS1 | CHS0
    // 选择通道 8 需要将 CHS3 置 1，但 15W408AS 寄存器位配置如下：
    ADC_CONTR = 0x88; // 1000 1000: 电源开, 速度最慢(540周期), 清标志, 启动位0, 通道1000(8)
    
    _nop_(); _nop_(); _nop_(); _nop_(); // 等待内部通道切换稳定

    //启动转换
    ADC_CONTR |= 0x08; // 置 ADC_START = 1
    
    //等待转换完成
    while (!(ADC_CONTR & 0x10)); // 等待 ADC_FLAG = 1
    
    //清除完成标志
    ADC_CONTR &= ~0x10;

    //读取 10 位 ADC 结果 (STC15W 系列 ADC_RES 存高 8 位，ADC_RESL 存低 2 位)
    adc_res = (ADC_RES << 2) | (ADC_RESL & 0x03);

    //计算 VCC 电压
    // 计算公式: VCC = (Bandgap_Value * 1024) / ADC_Result
    // 使用 32 位运算防止中间值溢出
    if (adc_res > 0) 
    {
        vcc_mv = ((uint32_t)bandgap_voltage * 1024) / adc_res;
    }
    else 
    {
        vcc_mv = 0;
    }

    return (uint16_t)vcc_mv;
}