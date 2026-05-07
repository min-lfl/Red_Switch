#ifndef __UART_H__
#define __UART_H__

#include <stc15w408as.H>

// ================= 配置区 =================
#define RX_BUF_SIZE 16 // 缓冲区大小建议设为 2 的幂次方
// ==========================================

// ##### 功能函数区 #####
void Uart_Init(void);
char putchar(char ch);
unsigned char Uart_ReadByte(unsigned char *dat);

#endif