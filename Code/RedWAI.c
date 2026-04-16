#include <RedWAI.H>

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
	//状态为0时
	if(Red_status==0){
		Red_status=1;

		set_InterruptTimer(0x00);								//把定时器装载0
		Red_TRx=1;															//开启定时器
	}
	
	
	//状态为1时
	else if(Red_status==1){
		Red_TRx=0;															//关闭定时器
		Time=read_InterruptTimer();							//读取定时器
		
		if(Time>12000 && Time<16000){   				//收到开始码，状态置1
			Red_status=2;
			
			set_InterruptTimer(0x00);							//把定时器装载0
			Red_TRx=1;														//开启定时器
		}else{																	//都不是，继续回到空闲状态
			Red_TRx=0;														//关闭定时器
			set_InterruptTimer(0x00);							//把定时器清零
			Red_status=0;													//回到空闲状态0
		}
	}
	
	//状态为2时
	else if(Red_status==2){
		//读取计时并且开始清零计时
		Red_TRx=0;															//关闭定时器
		Time=read_InterruptTimer();							//读取定时器值

		//等于0时
		if(Time>500 && Time<1600){  
			set_InterruptTimer(0x00);							//设定初值
			Red_TRx=1;														//开启定时器
			Data[pData/8]&=~(0x01<<(pData%8));		//存入二进制0
			pData++;															//数据指针+1
			if(pData>=32){pData_init();}		//判断有没有满32位二进制
		}
		
		//等于1时
		else if(Time>1700 && Time<2800){  
			set_InterruptTimer(0x00);							//设定初值
			Red_TRx=1;														//开启定时器
			
			Data[pData/8]|=0x01<<(pData%8);				//存入二进制1
			pData++;															//数据指针+1
			if(pData>=32){pData_init();}		//判断有没有满32位二进制
		}
		//如果数据错误时，结束接收，进入空闲模式等待开始信号
		else{
			Red_TRx=0;														//关闭定时器
			set_InterruptTimer(0x00);							//清零定时器
			pData=0;															//清零数据指针
			Red_status=0;
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
		if(Data[2]==~Data[3] && Data[0]==~Data[1]){  //校验数据完整性
			DataFlag=0; 
			return Data[2];
		}else {
			DataFlag=0;
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
	IT0 = 1;			//INT0(P3.2)下降沿中断
	EX0 = 1;			//使能INT0中断
	IE0=0;     		//中断标志位
	EA=1;					//打开总中断
	PX0=1;      	//设置优先级高
}


/**
	* @brief		初始化定时器,12T模式,16位自动重装
	* @note			注意必须用1T模式,这里需要更改
	* @param		无
	* @retval		无
	*/
void Timer0_Init(void)		
{
	//这里是导致调试了三天的地方,原来默认设置1T模式
	//把 |= 0x80 改为 &= 0x7F，强制设为 12T 模式！
	// 这样在 12MHz 晶振下，定时器刚好 1微秒(us) 计数加 1。
	AUXR &= 0x7F;			
	
	TMOD &= 0xF0;			// 清除定时器0的模式位
	TMOD |= 0x01;			// 设置定时器模式为 16位不自动重装载 (Mode 1)
	
	Red_TLx = 0x0;				// 设置定时初始值
	Red_THx = 0x0;				// 设置定时初始值
	TF0 = 0;					// 清除TF0标志
	TR0 = 0;					// 初始化时先不开启定时器，等外部下降沿来了再开
	
	ET0 = 1; 					//定时器中断
}



//定时器看门狗,如果达到中断还没有来脉冲,就初始化复位
void Timer0_Routine() interrupt 1 
{
    Red_TRx = 0;        // 关定时器
    Red_status = 0;     // 状态机归零
    pData = 0;          // 数组指针归零
}


/**
	* @brief		给定时器写入值，一般在开始计时时写入0
	* @param		要写入的值，0~65535
	* @retval		无
	*/
void set_InterruptTimer(unsigned int Time){
	Red_THx=Time>>8;
	Red_TLx=(unsigned char)Time;
}



/**
  * @brief  用于取出定时器的值
  * @param  无
  * @retval 取出的时间值，单位us (12MHz时计数值直接等于微秒)
  */
unsigned int read_InterruptTimer(void){
	static unsigned int Num=0;
	Num=(unsigned int)Red_THx<<8|Red_TLx;
  return Num;              					// 12MHz下，1个计数就等于1us，直接返回即可
}

//###############################################################################################


