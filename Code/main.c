#include <stc15w408as.H>

#include <INTRINS.H>

#include <LCD1602.H>
#include <DELAY.H>
#include <SWITCH0000.H>
#include <Servo.H>
#include <RedWAI.H>
#include <UART.H>

unsigned char Code=0;
unsigned char test_val=0;
sbit LED=P2^5;

void main(){
//	LCD_Init();
//	Servo_Init();
//	RedWAI_init();
	
//	InterruptTimer1_init();
////	run_InterruptTimer(1);
	Uart_Init();	
//	TL1=99;
	
	    // ----- 抓内鬼专用代码开始 -----
    EA = 0;             // 【第一步：斩断所有中断！】关闭总中断，让任何后台程序停摆
//		TL0 = 9; 
//    TL1 = 10;
		TL2 = 11;
		TL3 = 12;
		EA = 1;

	while(1){
//		printf("TMOD: %02X \n", (unsigned int)TMOD);
//		printf("TL1 (Hex): %bX \n", TL1); 
//		printf("TL1 (Dec): %bu \n", TL1);
//		printf("%u \t\n",read_InterruptTimer());
			    // 不用 printf 打印变量，直接用 IF 语句让单片机自己判断！
//		printf("TL0: %d \n",(int)TL0); 
//		printf("TL1: %d \n",(int)TL1); 
		printf("TL2: %d \n",(int)TL2); 
		printf("TL3: %d \n",(int)TL3); 
		
//		LED=1;
//		Code=get_Red_Data();
//		if(Code!=0){
//			printf("%d \t\n",(int)Code);
//		}
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


