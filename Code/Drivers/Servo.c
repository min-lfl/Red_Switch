#include <stc15w408as.H>

// --- 参数计算 (假设 Fosc = 12MHz, 12T模式) ---
// PCA时钟频率 = 12MHz / 12 = 2MHz (每个tick 1微秒)
// 周期 20ms 所需的 tick 数 = 20,000us / 1us = 20,000
// 假设高电平时间(占空比)为 3000 个计数周期，总周期为 40000 个计数周期
unsigned int duty_ticks = 0;   		//占空比(微秒)
unsigned int period_ticks = 20000; 		//总周期(微秒)

//全局变量区
sbit PWM_PIN=P1^1;	//需要翻转电平的端口(PWM输出口)

void PCA_Init(void)
{
    //管脚配置
		P_SW1 &= ~(0x30); 					//切换到第一组管脚,只是为了增加鲁棒性

    //PCA 基础配置
    CCON = 0x00;   							// 停止PCA计数，清除所有中断标志
    CL = 0x00;     							// 总计数器清零
    CH = 0x00;
    CMOD = 0x00;   							// CPS2~CPS0=000 (时钟源为 Fosc/12), 允许PCA中断

    //通道0配置：负责【占空比】(到达此值时拉低电平) ---
    CCAP0L = duty_ticks;       	// 初次赋值后，中断里无需再修改
    CCAP0H = duty_ticks >> 8;
    CCAPM0 = 0x49;             	// 16位软件定时模式，允许比较，允许中断

    //通道1配置：负责【总周期】(到达此值时拉高电平并清零总计数) ---
    CCAP1L = period_ticks;     	// 初次赋值后，中断里无需再修改
    CCAP1H = period_ticks >> 8;
    CCAPM1 = 0x49;             	// 16位软件定时模式，允许比较，允许中断

    //启动设置 ---
    PWM_PIN = 1;   							// 初始状态置高电平
    CR = 1;        							// 启动 PCA 计数器
    EA = 1;        							// 打开 CPU 总中断
}



// PCA 中断服务函数
void PCA_ISR(void) interrupt 7
{
    // --- 通道0中断：到达占空比时间，拉低电平 ---
    if (CCF0)
    {
        CCF0 = 0;       // 清空中断标志位
        PWM_PIN = 0;    // 输出低电平
    }

    // --- 通道1中断：到达总周期时间，拉高电平并清空计数器 ---
    if (CCF1)
    {
        CCF1 = 0;       // 清空中断标志位
        PWM_PIN = 1;    // 输出高电平
        
        // 核心要求：清零总计数器，重新开始新一轮周期
        // 建议：清零时短暂关闭一下 CR，防止在清零 CL 和 CH 的间隙发生硬件进位计数，清零后再打开
        CR = 0;  
        CL = 0;
        CH = 0;
			
				//重新设置占空比
				CCAP0L = duty_ticks;
				CCAP0H = duty_ticks >> 8;
			
        CR = 1;
    }
}



//高电平t占整个周期T（20ms）的时间
//舵机旋转的角度
//0.5ms ---------> 0度
//1ms ---------> 45度
//1.5ms ---------> 90度
//2ms ---------> 135度
//2.5ms ---------> 180度
//映射角度值
void Servo_Set(unsigned char Angle){

    if(Angle > 180) Angle = 180;
		duty_ticks=(Angle/45+1)*500;
}
