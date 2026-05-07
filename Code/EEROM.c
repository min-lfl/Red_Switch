#include "EEROM.h"

// --- IAP 命令定义 ---
#define CMD_IDLE    0
#define CMD_READ    1
#define CMD_WRITE   2
#define CMD_ERASE   3

// --- 等待时间设置 (针对不同主频) ---
// 如果你的主频在 24MHz 以下，设为 0x02 比较稳
#define WT_24M      0x02 

//==========================
/***基础函数部分***/
//==========================
// 关闭 IAP，回到安全状态
void IAP_Disable() {
    IAP_CONTR = 0;
    IAP_CMD = 0;
    IAP_TRIG = 0;
    IAP_ADDRH = 0xFF; // 指向非法区域防止误触发
    IAP_ADDRL = 0xFF;
}

// 从指定地址读取一个字节
uint8_t EEPROM_Read(uint16_t addr){
    uint8_t dat;
    IAP_CONTR = 0x80 | WT_24M; // 开启 IAP 模块
    IAP_CMD = CMD_READ;        // 设置读取命令
    IAP_ADDRH = addr >> 8;     // 设置高8位地址
    IAP_ADDRL = addr;          // 设置低8位地址
    IAP_TRIG = 0x5A;           // 触发序列 1
    IAP_TRIG = 0xA5;           // 触发序列 2
    _nop_();                   // 必须等待
    dat = IAP_DATA;            // 读取数据
    IAP_Disable();             // 及时关闭
    return dat;
}

// 擦除一个扇区 (STC15 一个扇区是 512 字节)
// 注意：写新数据前，必须先擦除！
void EEPROM_SectorErase(uint16_t addr) {
    IAP_CONTR = 0x80 | WT_24M;
    IAP_CMD = CMD_ERASE;
    IAP_ADDRH = addr >> 8;
    IAP_ADDRL = addr;
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;
    _nop_();
    IAP_Disable();
}

// 在指定地址写入一个字节
void EEPROM_Write(uint16_t addr, uint8_t dat) {
    IAP_CONTR = 0x80 | WT_24M;
    IAP_CMD = CMD_WRITE;
    IAP_ADDRH = addr >> 8;
    IAP_ADDRL = addr;
    IAP_DATA = dat;            // 放入要写的数据
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;
    _nop_();
    IAP_Disable();
}

//==========================
/***用户函数部分(项目差异)***/
//==========================


/**
	* @brief		写入函数,用于存储键值
	* @note			固定占用0x0000地址到0x0004地址下的ROM空间
	* @param		需要保存的键值,一共两个,前一个开灯,后一个关灯,以数组形式传入
	* @retval		无
	*/
void Write_RED_Key(uint8_t* keynum){
	EEPROM_SectorErase(0x0000);			//写之前必须先擦除整个扇区 (512字节都会被擦成 0xFF)
	
	
	EEPROM_Write(0x0000,keynum[0]);	//写入开灯码
	EEPROM_Write(0x0001,keynum[1]);	//写入关灯码
}


/**
	* @brief		读取函数,用于读取键值
	* @note			固定占用0x0000地址到0x0004地址下的ROM空间
	* @param		需要读取后存放进去的数组
	* @retval		无
	*/
void Red_RED_Key(uint8_t* keynum){
	keynum[0] = EEPROM_Read(0x0000);
	keynum[1] = EEPROM_Read(0x0001);
}