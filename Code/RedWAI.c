#include <stc15.h>  // 或者 #include "stc15.h"
#include <INTRINS.H> //标准变量库

#include <LCD1602.H>

//############################---V---定时器和中断相关函数---V-----#######################################
void Interrupt_init();  										//初始化外部中断
void InterruptTimer2_init();  							//初始化定时器
void set_InterruptTimer(unsigned int Time);	//写入定时器初值
void run_InterruptTimer(unsigned char Code);//启停计时
unsigned int read_InterruptTimer();					//读取定时器计时

//############################---V---红外相关函数---V-----###############################################
void RedWAI_init();													//红外初始化（执行一次就好）
void pData_init();													//内部函数，用于准备接收下一个按键
unsigned char get_Red_Data();								//外部函数，用于输出按键值，无按键时输出0


//############################---V---全局变量区---V-----#################################################
unsigned int Time;  												//用于读取定时器时间
unsigned char Red_status=0;  								//状态码判断状态码进入某种工作模式，0为空闲，1为等待，2为接收

unsigned char Data[4];											//用来存放数据，Data[0]为地址码，Data[1]为地址码反码，Data[1]为数据码，Data[3]为地址码反码
unsigned char pData=0;											//上面数组的索引号，每接收一位加1

unsigned char DataFlag=0;  									//数据标志位，置1说明读取到了数据，读取后清零
unsigned char restartFlag=0;  							//重发标志位，收到代表检测到重发置为1，读取后清零

//############################---V---主函数区---V-----###################################################
void Interrupt_Routine() interrupt 0
{
	
	//状态为0时
	if(Red_status==0){
		Red_status=1;
		set_InterruptTimer(0x00);
		run_InterruptTimer(1);
	}
	
	
	//状态为1时
	else if(Red_status==1){
		Time=read_InterruptTimer();
		set_InterruptTimer(0x00);
		
		
		if(Time>13000 && Time<14000){     	 	//收到开始码，状态置1
			Red_status=2;
			
			
		}else if(Time>10750 && Time<11750){		//收到按住码，重发标志位置1
			restartFlag=1;
			
			
		}else{																//都不是，继续回到等待状态
			Red_status=1;
		}
	}
	
	//状态为2时
	else if(Red_status==2){
		//读取计时并且开始清零计时
		Time=read_InterruptTimer();
		set_InterruptTimer(0x00);
		
		//等于0时
		if(Time>620 && Time<1520){  
			Data[pData/8]&=~(0x01<<(pData%8));
			pData++;
			if(pData>=32){pData_init();}
		}
		
		//等于1时
		else if(Time>1750 && Time<2650){  
			Data[pData/8]|=0x01<<(pData%8);
			pData++;
			if(pData>=32){pData_init();}
		}
		
		//如果数据错误时，结束接收，进入空闲模式等待开始信号
		else{
			Red_status=1;
		}
	}
}

//############################---V---红外相关函数---V-----########################################
/**
	* @brief		初始化，打开外部中断和定时器1
	* @param		无
	* @retval		无
	*/
void RedWAI_init(){
	Interrupt_init();
	InterruptTimer2_init();
}

/**
	* @brief		在索引号溢出后的操作，停止接收，状态变为0空闲，标志位置1表示接收到了数据
	* @param		无	
	* @retval		无
	*/
void pData_init(){
	pData=0;
	Red_status=0;
	set_InterruptTimer(0x00);
	DataFlag=1;  //数据标志位置1
}

/**
	* @brief		获取键值
	* @param		无
	* @retval		返回的键值
	*/
unsigned char  get_Red_Data(){  
	if(DataFlag){  //如果存在数据
		if(Data[2]==~Data[3] && Data[0]==~Data[1]){  //校验数据完整性
			DataFlag=0; 
			return Data[2];
		}else {
			return 0;
		}
	}else{
		return 0;
	}
}

//############################---V---定时器和中断相关函数---V-----#######################################

/**
	* @brief		初始化外部中断1，用于接收红外下降沿
	* @param		无
	* @retval		无
	*/
void Interrupt_init(){
	IT0=1;				//设置为下降沿触发
	IE0=0;     		//中断标志位
	EX0=1;				//打开EX0中断
	EA=1;					//打开总中断
	PX0=1;      	//设置优先级高
}

/**
	* @brief		定时器初始化，用于计数每个下降沿的间隔判断是何信号
	* @param		无
	* @retval		无
	*/
void InterruptTimer2_init(){
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TH1 = 0x00;			//设置定时初值
	TL1 = 0x00;			//设置定时初值
	TF1 = 0;				//清除TF0标志
	TR1 = 0;				//定时器0停止计时
//	ET1=1;    		//允许定时器0中断
}
/**
	* @brief		给定时器写入值，一般在开始计时时写入0
	* @param		要写入的值，0~65535
	* @retval		无
	*/
void set_InterruptTimer(unsigned int Time){
	TH1=Time/256;
	TL1=Time%256;
}
/**
	* @brief		用于打开停止定时器的计时
	* @param		0为停止，1为打开
	* @retval		无
	*/
void run_InterruptTimer(unsigned char Code){
	if(Code==1){
		TR1 = 1;
	}else{
		TR1 = 0;
	}
}
/**
	* @brief		用于取出定时器的值，一般在计时结束取出
	* @param		无
	* @retval		取出的时间值，单位us，误差0.5%
	*/
unsigned int read_InterruptTimer(){
	unsigned int Num=0;
	Num=TH1<<8|TL1;
	return (Num/92)*100;
}
//###############################################################################################


