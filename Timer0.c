#include <REGX52.H> //c52库


//1毫秒触发一次的定时器，让他触发就自加，触发多少次就是多少毫秒
void Timer0Init(void)		//1毫秒@12.000MHz
{
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

/*这个是一个一秒执行一次的模板，在笑脸处输入需要一秒执行一次的代码,记得复制回主函数
void Timer0_Routine() interrupt 1
{
	static  unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)  //1000毫秒
	{
		T0Count=0;
		^_^
	}
}
*/


/*  方便理解的中断初始化程序
//定时器初始化
void Tinmer0_Init(){
	//TMOD=0x01; //0000 0001
	TMOD=TMOD&0xF0; //把TMOD的低四位清零，高四位不变
	TMOD=TMOD|0x01; //把TMOD的低一位置1，其他七位不变
	
	TF0=0;
	TR0=1;
	TH0=64535/256;  //取64535高八位
	TL0=64535%256;  //取64535低八位
	ET0=1;
	EA=1;
	PT0=0;
}
*/