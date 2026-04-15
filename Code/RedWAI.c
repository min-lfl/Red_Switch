#include <stc15w408as.H>
#include <INTRINS.H> //标准变量库
#include <RedWAI.H>

#include <DELAY.H>

//调试用的灯
sbit LED=P2^5;
//调试点
sbit Debug=P2^3;
//调试用时间缓存区
unsigned int timeing=0;		

//############################---V---全局变量区---V-----#################################################
volatile unsigned int Time=0;  												//用于读取定时器时间
volatile unsigned char Red_status=0;  								//状态码判断状态码进入某种工作模式，0为空闲，1为等待，2为接收

volatile unsigned char Data[4]={0};											//用来存放数据，Data[0]为地址码，Data[1]为地址码反码，Data[1]为数据码，Data[3]为地址码反码
volatile unsigned char pData=0;											//上面数组的索引号，每接收一位加1

volatile unsigned char DataFlag=0;  									//数据标志位，置1说明读取到了数据，读取后清零
volatile unsigned char restartFlag=0;  							//重发标志位，收到代表检测到重发置为1，读取后清零



//############################---V---主函数区---V-----###################################################
void Interrupt_Routine() interrupt 0
{
	// TR0=0 放在最开头，只要进中断，第一时间停止定时器，防止读数过程定时器还在跑
	TR0 = 0; 
	
	if(Red_status == 0){
		Red_status = 1;
		TH0 = 0; TL0 = 0; // 清零定时器
		TR0 = 1;          // 开启定时器
	}
	else if(Red_status == 1){
		Time = ((unsigned int)TH0 << 8) | TL0; // 读取定时器
		
		if(Time > 12000 && Time < 16000){ // 收到13.5ms左右的引导码
			Red_status = 2;
			TH0 = 0; TL0 = 0; // 成功匹配，清零定时器，准备测下一个数据位
			TR0 = 1;
		}else{
			Red_status = 0; // 匹配失败，归零
			// 这里不需要再开启TR0，等待下一个下降沿触发状态0
		}
	}
	else if(Red_status == 2){
		Time = ((unsigned int)TH0 << 8) | TL0;
//		timeing = Time; // 用于串口打印调试
		
		if(Time > 500 && Time < 1600){ // 收到 Bit 0 (约 1.12ms / 1120)
//			LED=1;
			Data[pData/8] &= ~(0x01 << (pData%8));
			pData++;
            
			TH0 = 0; TL0 = 0; // 测完立刻清零
			TR0 = 1;          // 继续测下一个位
		}
		else if(Time > 1700 && Time < 2800){ // 收到 Bit 1 (约 2.25ms / 2250)
//			LED=1;
			Data[pData/8] |= (0x01 << (pData%8));
			pData++;
            
			TH0 = 0; TL0 = 0; // 测完立刻清零
			TR0 = 1;
		}
		else{
			// 如果读到的时间既不是Bit0也不是Bit1，说明传输出错或结束了
			Red_status = 0;
            pData = 0; // 接收错误，把位数也清零
		}
		
		// 接收满32位，通知主函数处理
		if(pData >= 32){
			pData = 0;
			Red_status = 0; 
			DataFlag=1;
			// 这里可以设置一个标志位，比如 IR_Recv_OK = 1;
            // 然后在主函数 while(1) 里判断 IR_Recv_OK == 1 再去点亮LED
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
	Interrupt0_init();
	Timer0_Init();
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
		LED=1;
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
void Interrupt0_init(){
//	IT0=1;				//设置为下降沿触发
//	EX0=1;				//打开EX0中断
//	IE0=0;     		//中断标志位

	IT0 = 1;			//INT0(P3.2)下降沿中断
	EX0 = 1;			//使能INT0中断
	IE0=0;     		//中断标志位
	EA=1;					//打开总中断
	PX0=1;      	//设置优先级高
	
//	IT0 = 1; // 设置INT0为下降沿触发 (千万不要设为0)
//	EX0 = 1; // 允许INT0中断
//	EA = 1;  // 开启总中断
}

/**
	* @brief		定时器初始化，用于计数每个下降沿的间隔判断是何信号
	* @param		无
	* @retval		无
	*/
//void Timer0_Init(void)		//65536微秒@12.000MHz
//{
//	AUXR |= 0x80;			//定时器时钟1T模式
//	TMOD &= 0xF0;			//设置定时器模式
//	TMOD |= 0x01;			//设置定时器模式
//	TL0 = 0x0;				//设置定时初始值
//	TH0 = 0x0;				//设置定时初始值
//	TF0 = 0;				//清除TF0标志
//	TR0 = 0;				//定时器0停止计时
//}

void Timer0_Init(void)		
{
	// 【核心修改】：把 |= 0x80 改为 &= 0x7F，强制设为 12T 模式！
	// 这样在 12MHz 晶振下，定时器刚好 1微秒(us) 计数加 1。
	AUXR &= 0x7F;			
	
	TMOD &= 0xF0;			// 清除定时器0的模式位
	TMOD |= 0x01;			// 设置定时器模式为 16位不自动重装载 (Mode 1)
	
	TL0 = 0x0;				// 设置定时初始值
	TH0 = 0x0;				// 设置定时初始值
	TF0 = 0;				// 清除TF0标志
	TR0 = 0;				// 初始化时先不开启定时器，等外部下降沿来了再开
}
//void Timer0_Isr(void) interrupt 1
//{
//	  	LED=1;
//			delay(500);
//}

/**
	* @brief		给定时器写入值，一般在开始计时时写入0
	* @param		要写入的值，0~65535
	* @retval		无
	*/
void set_InterruptTimer(unsigned int Time){
	TH0=Time*0;
	TL0=Time*0;
}
/**
	* @brief		用于打开停止定时器的计时
	* @param		0为停止，1为打开
	* @retval		无
	*/
void run_InterruptTimer(unsigned char Code){
	if(Code==1){
		TR0 = 1;
	}else{
		TR0 = 0;
	}
}

///**
//	* @brief		用于取出定时器的值，一般在计时结束取出
//	* @param		无
//	* @retval		取出的时间值，单位us，误差0.5%
//	*/
//unsigned int read_InterruptTimer(){
//	unsigned int Num=0;
//	Num=TH1<<8|TL1;
//	return (Num/92)*100;
//}

/**
  * @brief  用于取出定时器的值
  * @param  无
  * @retval 取出的时间值，单位us (12MHz时计数值直接等于微秒)
  */
unsigned int read_InterruptTimer(void){
	static unsigned int Num=0;
	Num=(unsigned int)TH0<<8|TL0;
//		timeing=Num;
  return Num;              // 12MHz下，1个计数就等于1us，直接返回即可
}

//###############################################################################################


