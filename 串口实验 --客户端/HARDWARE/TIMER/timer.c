#include "timer.h"

u8 a = 0 , aa = 0;
u8 SecureTime_Flag = 0;

//这里时钟选择为APB1的2倍，而APB1为36M
void TIM2_Secure_Init(u16 arr,u16 psc)	//安全时间：1秒内arr没有被重置则发出警报
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef 				 NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Period 				= arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler 		= psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel 									 = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  				//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 = 3;  				//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIM2					 
}

void TIM3_SendLocal_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef 				 NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period 				= arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler 		= psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel 									 = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  				//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 = 3;  				//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIM3					 
}

void TIM4_Echo_Init(u16 arr,u16 psc)		//超声波定时器初始化
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM4,TIM_IT_Update ,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  					 
}

void TIM2_IRQHandler(void)   //TIM2中断，进入停车,警报程序
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	//检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		LED=!LED;
		
//		for(a=0;a<20;a++){														
//			USART_SendData(USART1, Rfid_Data[aa]);
//			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
//		}
	}
}

void TIM3_IRQHandler(void)   //TIM3中断，发送一次RFID位置信息
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		LED=!LED;
		if(SecureTime_Flag < 2 )	//当有两次进入该中断SecureTime_Flag>2时说明已经超过1s没有得到主机的回应了，该位在串口2中断服务函数里置0
			SecureTime_Flag++;
		for(a=0;a<20;a++){														//发送一次RFID位置信息,可不用20个字节数据
			USART_SendData(USART1, RFID_Data[aa]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
		}
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
/***************************以下为超声波发数据格式****************************/
		USART_SendData(USART2, 0xe8);
		while((USART2->SR&0X40)==0);//循环发送,直到发送完毕
		delay_us(40);
		USART_SendData(USART2, 0x02);
		while((USART2->SR&0X40)==0);//循环发送,直到发送完毕
		delay_us(40);
		USART_SendData(USART2, 0xb4);
		while((USART2->SR&0X40)==0);//循环发送,直到发送完毕
		delay_us(200);
/*******************************************************************************/		
	}
}










