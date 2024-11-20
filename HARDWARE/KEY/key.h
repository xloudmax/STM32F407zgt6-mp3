#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //上一首 PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)	//EQ PE6
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //下一首 PE2
#define KEY3 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)	//暂停 PE5

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define KEY3_PRES   4
void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif
