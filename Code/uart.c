#include "uart.h"
#include <stdio.h>

// ##### 全局变量区 #####
static volatile bit busy; // 使用 volatile 防止编译器过度优化

static unsigned char rx_buffer[RX_BUF_SIZE];
static unsigned char write_ptr = 0; 
static unsigned char read_ptr = 0;  

// ######宏定义区#######
#define UART_CON  SCON
#define UART_BUF  SBUF
#define UART_ISR_NUM 4  
// 串口1位定义
#define UART_TI_BIT TI
#define UART_RI_BIT RI

/**
 * @brief  串口初始化 (9600bps @ 12MHz)
 * @note   STC15W408AS 默认使用定时器2作为波特率发生器
 */
void Uart_Init(void)
{
    busy = 0;

    SCON = 0x50;        // 8位数据, 可变波特率
    AUXR |= 0x01;       // 串口1选择定时器2为波特率发生器


    // --- 定时器2设置 (针对12MHz时钟) ---
    AUXR |= 0x04;       // 定时器2时钟为1T模式 (不分频)
    T2L = 0xC7;         // 9600bps: 65536 - (12000000/4/9600) = 65223 (0xFE87)
    T2H = 0xFE;         // 注意：1T模式下精度更高
    AUXR |= 0x10;       // 启动定时器2

    // --- 中断配置 ---
    ES = 1;             // 开启串口1中断
    EA = 1;
}

/**
 * @brief 重定向 putchar，支持 printf
 */
char putchar(char ch)
{
    while(busy);        // 等待上一个数据发送完成
    busy = 1;
    UART_BUF = ch;
    return ch;
}

/**
 * @brief 接收读取
 */
unsigned char Uart_ReadByte(unsigned char *dat)
{
    if (read_ptr == write_ptr) return 0; 
    
    *dat = rx_buffer[read_ptr];
    read_ptr = (read_ptr + 1) % RX_BUF_SIZE;
    return 1;
}

/**
 * @brief 统一中断服务函数
 */
void Uart_Isr(void) interrupt UART_ISR_NUM
{
    if (TI)
    {
        TI = 0;
        busy = 0;
    }
    if (RI)
    {
        RI = 0;
        rx_buffer[write_ptr] = SBUF;
        write_ptr = (write_ptr + 1) % RX_BUF_SIZE;
    }
}