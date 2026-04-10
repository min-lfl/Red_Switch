#include <stc15w408as.H>

//全局变量区
sbit PWM_IO=P1^3;
unsigned char Actual=0;

//void Servo_Init(){
//	//初始化定时器
//	TMOD &= 0xF0;		//设置定时器模式
//	TMOD |= 0x01;		//设置定时器模式
//	TL0 = 0x18;		//设置定时初值
//	TH0 = 0xFC;		//设置定时初值
//	TF0 = 0;		//清除TF0标志
//	TR0 = 1;		//定时器0开始计时
//	ET0=1;  //允许中断（没做解释，自行了解）
//	EA=1;   //允许总中断
//	PT0=0;   //中断优先级等于0
//	
//}

//高电平t占整个周期T（20ms）的时间
//舵机旋转的角度
//0.5ms ---------> 0度
//1ms ---------> 45度
//1.5ms ---------> 90度
//2ms ---------> 135度
//2.5ms ---------> 180度
//映射角度值
void Servo_Set(unsigned char Angle){
	unsigned char ticks;  //中间变量

    if(Angle > 180) Angle = 180;
    // 0.5ms + Angle*(2.0ms/180)  ≈ (Angle/45+1)*0.5ms
    ticks = Angle / 45 + 1;       // 1~5，对应 0.5~2.5ms

    ET0 = 0;
    Actual = ticks;
    ET0 = 1;
}


////中断函数
//void Timer0_Routine(void) interrupt 1
//{
//    static unsigned char cnt = 0;

//    TL0 = 0x33;
//    TH0 = 0xFE;

//	//采用首尾置位的方式减小cpu负担
//    // 周期开始：拉高
//    if(cnt == 0)
//        PWM_IO = 1;

//    // 到了应该结束高电平的位置：拉低
//    if(cnt == Actual)
//        PWM_IO = 0;

//    cnt++;
//    if(cnt >= 40)      // 40*0.5ms=20ms
//        cnt = 0;
//}