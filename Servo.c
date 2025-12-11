#include <REGX52.H>

//全局变量区
sbit PWM_IO=P1^3;
unsigned char Actual=0;

void Servo_Init(){
	//初始化定时器
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;  //允许中断（没做解释，自行了解）
	EA=1;   //允许总中断
	PT0=0;   //中断优先级等于0
	
}

//高电平t占整个周期T（20ms）的时间
//舵机旋转的角度
//0.5ms ---------> 0度
//1ms ---------> 45度
//1.5ms ---------> 90度
//2ms ---------> 135度
//2.5ms ---------> 180度
//映射角度值
void Servo_Set(unsigned char Angle){
	Actual=Angle/45+1;
}


//中断函数
void Timer0_Routine() interrupt 1
{
	static  unsigned int T0Count;
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	
	T0Count++;
	if(T0Count>=40)  //20毫秒,每次500微秒，40次20毫秒
	{
		T0Count=0;
	}
	
	if(Actual>T0Count){
		PWM_IO=1;
	}else{
		PWM_IO=0;
	}
}