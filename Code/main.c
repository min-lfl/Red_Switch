#include <stc15w408as.H>


#include <LCD1602.H>
#include <DELAY.H>
#include <SWITCH0000.H>
#include <Servo.H>
#include <RedWAI.H>
#include <UART.H>

unsigned char Code=0;
sbit LED=P2^5;

extern unsigned int Time;
extern unsigned int timeing;
unsigned int timeing_old;
extern unsigned char Red_status;
extern unsigned char Data[4];
void main(){
	    P3M0 = 0x00; P3M1 = 0x04; //P3.2高阻抗输入
//	LCD_Init();
//	Servo_Init();
	RedWAI_init();
	Uart_Init();	

	while(1){
		
		LED=1;
		Code=get_Red_Data();
		if(Code!=0){
			TR0=0;
			printf("%d \t\n",(int)Code);
			TR0=1;
		}
//		if(Data[0]!=0){
//			printf("Data0:%d \t\n",(int)Data[0]);
//			printf("Data1:%d \t\n",(int)Data[1]);
//			printf("Data2:%d \t\n",(int)Data[2]);
//			printf("Data3:%d \t\n",(int)Data[3]);
//		}
		
		//测试起始位后脉宽
//		if(timeing_old!=timeing){
//			printf("time:%u \t\n",(unsigned int)timeing);
//			timeing_old=timeing;
//		}
		
//		//开始写计时电路的原理验证
//		set_InterruptTimer(0x00);	//设定初值
//		run_InterruptTimer(1);		//开启定时器
//		delay(65);
//		run_InterruptTimer(0);		//关闭定时器
//		Time=read_InterruptTimer();
//		
//		printf("time:%u \t\n",(unsigned int)Time);
//		
		
		
//		TR0=0;
//		printf("time:%u \t\n",(unsigned int)timeing);
//		printf("status:%d \t\n",(int)Red_status);
//		delay(10);
//		TR0=1;
//		printf("%d \t\n",(int)read_InterruptTimer());
//		Servo_Set(0);
//		delay(3000);
//		Servo_Set(45);
//		delay(3000);
//		Servo_Set(90);
//		delay(3000);
//		Servo_Set(135);
//		delay(3000);
//		Servo_Set(180);
//		delay(3000);
//		Code=get_Red_Data();
//		if(Code!=0){
//			LCD_ShowHexNum(2,1,Code,2);
//			if(Code==0x47){
//				Servo_Set(90);
//			}
//			if(Code==0x07){
//				Servo_Set(45);
//				delay(300);
//				Servo_Set(90);
//			}
//			if(Code==0x45){
//				Servo_Set(135);
//				delay(300);
//				Servo_Set(90);
//			}
//		}
		
	}
}


