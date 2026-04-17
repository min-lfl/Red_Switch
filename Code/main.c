#include <stc15w408as.H>


#include <DELAY.H>
#include <Servo.H>
#include <RedWAI.H>
#include <UART.H>

unsigned char Code=0;
unsigned char Angle=0;

sbit Key=P3^3;
sbit LED=P2^5;
sbit Sg90=P2^6;


void main(){
	/* 初始化区 */
	P2M0 |= 0x60; P2M1 &= ~0x60; 	//LED和SG90推挽输出
	LED=0;Sg90=0;									//LED和Sg90初始化
	
	RedWAI_init();
	Uart_Init();	
	PCA_Init();
	
	
//	Sg90=1;
	while(1){
		
		Code=get_Red_Data();
		if(Code!=0){
			printf("Code:%d \t\n",(int)Code);
				if(Code==69){
					if(Angle<=135)Angle=Angle+45;
					printf("Angle:%d \t\n",(int)Angle);
				}else if(Code==7){
					if(Angle>=45)Angle=Angle-45;
					printf("Angle:%d \t\n",(int)Angle);
				}
		}


		Servo_Set(Angle);
		
		
		if(Key==0){
			LED=~LED;
			if(LED==1){
				Sg90=1;
			}else{
				Sg90=0;
			}
			delay(300);
		}
		
		
	}
}


