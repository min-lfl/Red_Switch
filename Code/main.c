#include <stc15.h>  // 或者 #include "stc15.h"
#include <INTRINS.H>

#include <LCD1602.H>
#include <DELAY.H>
#include <SWITCH0000.H>
#include <Servo.H>
#include <RedWAI.H>
#include <UART.H>

unsigned char Code=0;
sbit LED=P2^5;

void main(){
	LCD_Init();
	Servo_Init();
	RedWAI_init();
	Uart_Init();		
	while(1){

		LED=1;
		Code=get_Red_Data();
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


