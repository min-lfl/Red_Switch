#include <stc15w408as.H>

#include <DELAY.H>
#include <Servo.H>
#include <RedWAI.H>
#include <UART.H>
#include <INTRINS.H>
#include "EEROM.h"
#include "VCC.h"


/***定义设备地址***/
sbit Key=P3^3;
sbit LED=P2^5;
sbit Sg90=P2^6;

/***全局变量***/
extern volatile uint8_t Red_status;
extern volatile unsigned char pData;

/***辅助缓存区***/
static volatile uint16_t timeout_cnt = 0; //唤醒后的超时计数
static volatile uint8_t Code=0;						//获取到的键值
static volatile uint8_t Angle=0;					//当前角度

static volatile uint16_t Power_VCC=0;						//当前电源电压缓存值
static volatile uint8_t Power_lower_status=0;		//低电量报警标志位
static volatile uint8_t i=0;										//循环标志位

static volatile uint8_t ste_key_status=0;				//修改模式标志位
static volatile uint16_t timekey_cnt = 0; 			//进入修改模式后超时控制
static volatile uint16_t LED_key=0;							//LED控制缓存
static volatile uint8_t pKeynum = 0;						//数组指针
static volatile uint8_t Keynum_w[2]={0x45,0x7};	//需要写入的
static volatile uint8_t Keynum_r[2]={0};				//读取出来的

/***电源管理函数声明***/
void Open_All_Power(void);								//打开所有电源
void Close_All_Power(void);								//关闭所有电源

void Ext_Init(void);											//外部中断1的配置

void main(){
	/* 初始化区 */
	P2M0 |= 0x60; P2M1 &= ~0x60; 	//LED和SG90推挽输出
	LED=0;
	
	RedWAI_init();
	Uart_Init();	
	PCA_Init();
	Ext_Init();
	
	Red_RED_Key(Keynum_r);
	
	while(1)
	{
		// ==========================================
		// 模块1：业务处理,接收键值控制舵机（如果收到了完整数据,就临时打开一下设备）
		// ==========================================
		Code=get_Red_Data();	//获取键值
		if (Code!=0)					//键值不为0
		{
			Open_All_Power();//打开所有模块供电,准备工作
			delay(200);				//等待舵机开机完成
			//改变舵机角度,实现灯光控制
			if(Code==Keynum_r[0]){
				Angle=45;
				Servo_Set(Angle);	//刷新舵机角度,完成控制
				delay(400);
				
				Angle=0;
				Servo_Set(Angle);	//刷新舵机角度,完成控制
				delay(30);
				
				Angle=90;
				Servo_Set(Angle);	//刷新舵机角度,完成复位
				delay(300);
//					printf("Angle:%d \t\n",(int)Angle);	//调试接口
			}else if(Code==Keynum_r[1]){
				
				Angle=135;
				Servo_Set(Angle);	//刷新舵机角度,完成控制
				delay(400);
				
				Angle=180;
				Servo_Set(Angle);	//刷新舵机角度,完成控制
				delay(30);
				
				Angle=90;
				Servo_Set(Angle);	//刷新舵机角度,完成复位
				delay(300);
				
//					printf("Angle:%d \t\n",(int)Angle);	//调试接口
			}
			
			Code=0;							//控制完成,清除标志位
			Close_All_Power();	//把设备断电
		}

		
		// ==========================================
		// 模块2：电池低电量保护
		// ==========================================
		
		Power_VCC=Get_Battery_Voltage();
		//如果电池小于3.4V
		if(Power_VCC<3400){
			Power_lower_status=1;	//低电量标志位置1
		}else if(Power_VCC>3500){
			Power_lower_status=0;	//低电量标志位置0
		}
//		printf("Vcc:%d \t\n",(int)Power_VCC);	//调试接口
		
		// ==========================================
		// 模块3：修改键值模块
		// ==========================================
		//进入修改键值模式
		if(ste_key_status!=0){
			timekey_cnt=0;			//清零修改模式超时时间
			//这个循环1ms执行一次
			while(timekey_cnt<35535 && ste_key_status!=0){
					
				Code=get_Red_Data();	//获取键值
				if (Code!=0){					//键值不为0
					LED=1;
					delay(600);
					Keynum_w[pKeynum]=Code;
					pKeynum++;
						
					//接收满了两个数据了
					if(pKeynum>=2){
						Write_RED_Key(Keynum_w);	//写入EEROM
						delay(100);
						Red_RED_Key(Keynum_r);		//读取回来
						pKeynum=0;
						ste_key_status=0;					//复位标志位
					}
				}
					
				//闪烁模块
				LED_key++;
				if(LED_key>100){
					//取反LED
					if(LED==1){
						LED=0;
					}else{
						LED=1;
					}
					LED_key=0;
				}
				
				//超时退出部分,保证进入后不会超过65秒
				delay(1);
				timekey_cnt++;
			}
			ste_key_status=0;//保证超时退出前可以复位标志位
			LED=0;
		}
//			printf("key1:%d \t\n",(int)Keynum_r[0]);	//调试接口
//			printf("key2:%d \t\n",(int)Keynum_r[1]);	//调试接口

		
		// ==========================================
		// 模块4：智能休眠管理
		// ==========================================
		// 只要 Red_status 等于 0，说明现在处于“空闲无事”状态
		if (Red_status == 0) 
		{
			CR = 0; //停止 PCA 计数器
			timeout_cnt = 0; 		// 清除超时计数
            
			//安心睡觉，时间静止
			PCON |= 0x02;
			_nop_();
			_nop_();
		}
        
		// 如果 Red_status 不为 0，说明红外正在接收中<不能休眠>
		else 
		{
			//超时处理
			delay(1); 	//延时 1 毫秒
			timeout_cnt++;
            
			// NEC协议完整一帧最多大概 110ms。如果过了 150ms 还没解完，说明是干扰！
			if (timeout_cnt > 150) 
			{
			set_InterruptTimer(0x00);// 强行清零定时器
			pData = 0;               // 强行废弃收了一半的数据
			Red_status = 0;          // 强行重置为空闲状态！
			//下一次循环，单片机会重新进入休眠状态
			}
		}
		

	}
}

/***外部中断1实现***/
void INT1_Isr(void) interrupt 2
{
	if(ste_key_status==1){
		ste_key_status=0;
		pKeynum=0;
	}else{
		ste_key_status=1;
	}
	
}

//外部中断1初始化配置
void Ext_Init(void)
{
	IT1 = 1;			//INT1(P3.3)下降沿中断
	EX1 = 1;			//使能INT1中断
}


//打开所有电源
void Open_All_Power(void){
	//前提是低电量标志位不为1
	if(Power_lower_status!=1){
		LED=1;		//打开电源指示灯
		Sg90=0;		//给舵机供电
		CR = 1;		//打开PCA计数
	}else{	//如果低电量了,就光闪灯不启动舵机
		//3500-3400就闪烁4次,
		//3400-3300就闪烁3次,
		//3300-3200就闪烁2次,
		//3200-3100就闪烁1次,
		//3100以下不再闪烁,此时非常危险了,必须马上充电
		for(i=0;i<(Power_VCC-3000)/100;i++){
			LED=1;
			delay(300);
			LED=0;
			delay(300);
		}
		if(Power_VCC<3100){
			Close_All_Power();	//关闭所有电源
			EA=0;
		}
	}
}

//关闭所有电源
void Close_All_Power(void){
	LED=0;		//关闭电源指示灯
	Sg90=1;		//给舵机断电
	CR = 0;
}




