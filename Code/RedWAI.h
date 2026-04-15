#ifndef __RedWAI_H__
#define __RedWAI_H__

//############关于红外接收的宏定义###########
#define Red_TRx TR0

//############外部中断相关(内部)###########
void Interrupt0_init(void);												//初始化内部中断

//############定时器相关(内部)##########
void Timer0_Init(void);														//定时器0初始化
void run_InterruptTimer(unsigned char Code);			//设置定时器开启或关闭
void set_InterruptTimer(unsigned int Time);				//写入定时器初值
unsigned int read_InterruptTimer(void);						//读取定时器的值
void pData_init();																//内部函数，用于准备接收下一个按键

//############红外相关(外部)#############
void RedWAI_init();							//外部函数,初始化红外
unsigned char  get_Red_Data();	

	
#endif