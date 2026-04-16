/*	
		这是我写的一个关于红外接收头HS0038B/IRM3638T的接收驱动代码,
		代码中适配的是stc15w408as单片机,
		占用的是定时器0和外部中断0(P3.2)
*/

//$$$需要如何更改以适配其他单片机
//需要把红外接收头的信号线接到3.2引脚,并且配置外部中断0(关于外部中断最好不要更改)
//@@@1.定时器0也需要按你的单片机型号进行更改,参考我的默认配置参数,重点的必须配置成12T模式,即12个时钟周期对应一个机器周期
//@@@2.更改下方19行到21行的定时器寄存器,完成初始化和宏定义修改,就可以完成全文定时器的修改了,测试代码在下面第40行
//@@@3.如果你的单片机晶振不是12MHZ,那么需要修改unsigned int read_InterruptTimer(void);函数的实现,具体修改方法可以询问AI
#ifndef __RedWAI_H__
#define __RedWAI_H__

//############头文件引用区#################
#include <stc15w408as.H>
#include <INTRINS.H> //标准变量库

//############关于红外接收的宏定义(如果更换定时器需要修改)###########
#define Red_TRx TR0																//定时器启停标志位
#define Red_THx	TH0																//定时器高八位
#define Red_TLx	TL0																//定时器低八位

//############外部中断相关(内部)###########
void Interrupt0_init(void);												//初始化内部中断

//############定时器相关(内部)##########
void Timer0_Init(void);														//定时器0初始化
void run_InterruptTimer(unsigned char Code);			//设置定时器开启或关闭
void set_InterruptTimer(unsigned int Time);				//写入定时器初值
unsigned int read_InterruptTimer(void);						//读取定时器的值
void pData_init();																//内部函数，用于准备接收下一个按键

//############红外相关(外部)#############
void RedWAI_init();																//外部函数,初始化红外
unsigned char  get_Red_Data();										//获取键码

#endif


//参考的测试代码,注意使用时我们只需要调用这两个外部函数即可
//void main(){
//	RedWAI_init();
//	while(1){
//		
//		Code=get_Red_Data();
//		if(Code!=0){
//			printf("%d \t\n",(int)Code);//这个地方我用的串口打印,你也可以使用其他输出接口输出,比如屏幕打印或者LED灯闪的次数
//		}
//	}
//}