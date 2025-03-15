#include <REGX52.H>
#include "Timer1.h"
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

//引脚定义,电机控制端口
sbit Motor=P1^0;

unsigned char Counter,Compare;
unsigned char SpeedDelta = 1; // 速度变化量，每次增加或减少的速度值

/**
  * @brief  电机初始化
  * @param  无
  * @retval 无
  */
void Motor_Init(void)
{
	Timer1_Init(); // 定时器初始化函数，配置定时器以生成PWM信号
}

/**
  * @brief  电机设置速度
  * @param  Speed 要设置的速度，范围0~100
  * @retval 无
  */
void Motor_SetSpeed(unsigned char Speed)
{
	Compare=Speed;
}

//定时器1中断函数
void Timer1_Routine() interrupt 3
{
	static unsigned int T1Count;
	
	TL1 = 0x9C;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值 65436
	Counter++;		// 每次中断计数器加1
	Counter%=100;	//计数值变化范围限制在0~99
	if(Counter<Compare)	//计数值小于比较值,Compare是PWM占空比的值
	{
		Motor=1;		//输出1
	}
	else				//计数值大于比较值
	{
		Motor=0;		//输出0
	}
	
	/*
	// 无级变速逻辑
    //Compare += SpeedDelta; // 改变占空比
    if (Compare >= 100 || Compare <= 0) // 如果达到速度极限，改变增加方向
    {
        //SpeedDelta = -SpeedDelta;
		Compare = 30;
    }
	*/
	
	//按键扫描
	T1Count++;
	if(T1Count>=200)
	{
		T1Count=0;
		MatrixKey_Loop();	//20ms调用一次矩阵按键驱动函数
	}
}

void ShowCompare() {
	LCD_ShowNum(1,10,Compare, 2);
}

void MotorAddSpeed() {
	// 将占空比从0%逐渐增加到100%
	unsigned char currentSpeed;
	LCD_ShowString(2,1,"Speed:");
    for (currentSpeed = 0; currentSpeed < 100; currentSpeed++) {
		Compare=currentSpeed;
		// 在这里添加延时，以改变占空比变化的速率
		LCD_ShowNum(2,7,Compare, 2);
        Delay(10); // 延时函数
    }
}

void MotorReduceSpeed() {
	// 从100%逐渐减少到0%
	unsigned char currentSpeed;
	LCD_ShowString(2,1,"Speed:");
    for (currentSpeed = 99; currentSpeed > 0; currentSpeed--) {
		Compare=currentSpeed;
		LCD_ShowNum(2,7,Compare, 2);
		Delay(10); // 延时函数
    }
}

/*放到定时器中断里
void MotorNaturalWind(unsigned char currentSpeed) {
	Compare = currentSpeed;
		Compare += SpeedDelta; // 改变占空比
		if (Compare >= 99 || Compare <= 0) // 如果达到速度极限，改变增加方向
		{
			SpeedDelta = -SpeedDelta;
		}
}
*/