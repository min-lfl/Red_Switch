#ifndef __EEROM_H__
#define __EEROM_H__
/***宏定义---引用头文件***/
#include <stc15w408as.H>
#include <intrins.h>


/***用户函数区***/
void Write_RED_Key(uint8_t* keynum);	//写入函数,用于存储键值
void Red_RED_Key(uint8_t* keynum);		//读取函数,用于读取键值

#endif