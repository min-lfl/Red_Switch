#ifndef __SWITCH0000_H__
#define __SWITCH0000_H__

//使用方法:把Scan_key();函数放入定时器中，20ms执行一次，它会不断扫描哪个按键按下把键值存入全局变量
//再到主函数循环中通过switch0000()函数接收键值


/**
	* @brief  获取独立按键键码
	* @param   无
	* @retval  按下的第几个键，范围0~4，无按键按下时返回值为0
	*/
unsigned char switch0000();


/**
	* @brief  放入定时器中不断扫描按键，建议20ms执行一次
	* @param   无
	* @retval  无
	*/
void Scan_key();
#endif

/*定时器示例
void Timer0_Routine() interrupt 1
{
	static  unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=20)  //20毫秒
	{
		T0Count=0;
		Scan_key();
	}
}
*/