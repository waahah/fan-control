#include <REGX52.H>
#include "Delay.h"

#include "LCD1602.h"
#include "DS18B20.h"
#include "AT24C02.h"

#include "Key.h"
#include "Nixie.h"
#include "Motor.h"
#include "IR.h"

#include "Buzzer.h"

#include "MatrixKey.h"

//温度显示存储
float T,TShow;
char TLow,THigh;
//红外电机
unsigned char Command,Speed;
//模式控制
unsigned char Mode;

unsigned char KeyNum;

void Thermosensation() {
	//设定的最小温度大于室温则开启风扇
	if(TShow>= TLow ) {    
		if(Speed == 0) {
			Speed = 1;
		}
		// 无级变速逻辑
		Motor_SetSpeed(TShow); //改变占空比
		//ShowCompare(); //显示占空比
			
	}else{
		Speed = 0;
		Motor_SetSpeed(0);
	}
		
	//高于设定温度则会进行报警
	if(THigh >= TShow) {
			
		Buzzer_Time(100);
	}
}

void main()
{
	//温度模块
	DS18B20_ConvertT();		//上电先转换一次温度，防止第一次读数据错误
	Delay(1000);			//等待转换完成
	THigh=AT24C02_ReadByte(0);	//读取温度阈值数据
	TLow=AT24C02_ReadByte(1);
	if(THigh>125 || TLow<-55 || THigh<=TLow)
	{
		THigh=20;			//如果阈值非法，则设为默认值
		TLow=15;
	}
	Mode = AT24C02_ReadByte(2);	//读取模式设置
	if(Mode>3 || Mode<=0 )
	{
		Mode = 1;			//如果阈值非法，则设为默认值
	}
	LCD_Init();
	LCD_ShowString(1,1,"T:");
	/*LCD_ShowString(2,1,"TH:");
	LCD_ShowString(2,9,"TL:");
	LCD_ShowSignedNum(2,4,THigh,3);
	LCD_ShowSignedNum(2,12,TLow,3);
	*/
	
	//红外模块
	Motor_Init();
	IR_Init();
	
	while(1)
	{	
		//温度模块
		/*温度读取及显示*/
		DS18B20_ConvertT();	//转换温度
		T=DS18B20_ReadT();	//读取温度
		if(T<0)				//如果温度小于0
		{
			LCD_ShowChar(1,3,'-');	//显示负号
			TShow=-T;		//将温度变为正数
		}
		else				//如果温度大于等于0
		{
			LCD_ShowChar(1,3,'+');	//显示正号
			TShow=T;
		}
		LCD_ShowNum(1,4,TShow,2);		//显示温度整数部分
		LCD_ShowChar(1,6,'.');		//显示小数点
		LCD_ShowNum(1,7,(unsigned long)(TShow*100)%100,2);//显示温度小数部分
		//LCD_ShowString(2,13,"S:  "); //电机显示速度
		
		//设定的最小温度大于室温则开启风扇
		
		KeyNum = MatrixKey();		//获取按键数
		
		if(IR_GetDataFlag() || KeyNum /*|| IR_GetRepeatFlag()*/)	//如果收到数据帧//或者收到连发帧 || 按键
		{
			
			if(KeyNum) {
				LCD_ShowNum(1,10,KeyNum,2);
						//获取遥控器命令码
			}
			
			
			Command=IR_GetCommand();		//获取遥控器命令码
			
			if(Command == IR_MODE || KeyNum == 1) {    //模式切换
				
				//自动模式
				if(Mode == 1) {
					LCD_ShowString(1,13,"Auto");
					LCD_ShowString(2,1,"                ");
				}
				//遥控模式
				if(Mode == 2) {
					LCD_ShowString(1,13,"Hand");
					LCD_ShowString(2,1,"                ");
				}
				//自然风模式
				if(Mode == 3) {
					LCD_ShowString(1,13," N W");
					LCD_ShowString(2,1,"                ");
				}
				
				Mode += 1;
				if (Mode > 3 || Mode <= 0) // 循环切换--
				{
					Mode = 1;
					//ModeDelta = -ModeDelta;
				}
				AT24C02_WriteByte(2, Mode);
				Delay(5);
				//LCD_ShowString(1,13,"auto");
			
			}
			
			/*阈值判断及显示*/
			if(Command==IR_NEXT || KeyNum == 2)	//K2按键，THigh自增
			{
				THigh++;
				if(THigh>125){THigh=125;}
			}
			if(Command==IR_PREVIOUS || KeyNum == 3)	//K3按键，THigh自减
			{
				THigh--;
				//if(THigh<=TLow){THigh++;}
			}
			if(Command==IR_VOL_ADD || KeyNum == 4)	//K4按键，TLow自增
			{
				TLow++;
				//if(TLow>=THigh){TLow--;}
			}
			if(Command==IR_VOL_MINUS || KeyNum == 5)	//K5按键，TLow自减
			{
				TLow--;
				if(TLow<-55){TLow=-55;}
			}
			LCD_ShowSignedNum(2,4,THigh,3);	//显示阈值数据
			LCD_ShowSignedNum(2,12,TLow,3);
			AT24C02_WriteByte(0,THigh);		//写入到At24C02中保存
			Delay(5);
			AT24C02_WriteByte(1,TLow);
			Delay(5);
			
			if(Command==IR_0 || Command==IR_POWER || KeyNum == 6){Speed=0;}		//根据遥控器命令码设置速度
			if(Command==IR_1 || KeyNum == 7){Speed=1;}
			if(Command==IR_2 || KeyNum == 8){Speed=2;}
			if(Command==IR_3 || KeyNum == 9){Speed=3;}
			
			if(Speed==0){Motor_SetSpeed(0);}	//速度输出
			if(Speed==1){Motor_SetSpeed(25);}
			if(Speed==2){Motor_SetSpeed(75);}
			if(Speed==3){Motor_SetSpeed(100);}
		}
		
		switch (Mode){
			case 1:
				LCD_ShowString(1, 13, "Auto");
				LCD_ShowString(2, 1, "TH:");
				LCD_ShowString(2, 9, "TL:");
				LCD_ShowSignedNum(2, 4, THigh, 3);
				LCD_ShowSignedNum(2, 12, TLow, 3);
				Thermosensation();
				break;
			
			case 2:
				
				LCD_ShowString(1, 13, "Hand");
				LCD_ShowString(2,11,"     ");
				LCD_ShowString(2, 1, "Gear:");
				LCD_ShowSignedNum(2, 6, Speed, 1);
				break;
			case 3:
				LCD_ShowString(2,11,"     ");
				LCD_ShowString(1,13," N W");
				MotorAddSpeed();
				MotorReduceSpeed();
				break;
		}
		
		//LCD_ShowNum(1,15,Speed, 2); //LCD1602显示当前速度
		//Nixie(1,Speed);						//数码管显示速度*/
	}
}

