#ifndef __RedWAI_H__
#define __RedWAI_H__


void InterruptTimer1_init(void);
void run_InterruptTimer(unsigned char Code);			//设置定时器开启或关闭
unsigned int read_InterruptTimer(void);								//读取定时器的值

void RedWAI_init();
unsigned char  get_Red_Data();
	
#endif