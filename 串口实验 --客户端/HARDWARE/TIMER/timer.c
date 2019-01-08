#include "timer.h"

u8 a = 0 , aa = 0;
u8 SecureTime_Flag = 0;

//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
void TIM2_Secure_Init(u16 arr,u16 psc)	//��ȫʱ�䣺1����arrû�б������򷢳�����
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef 				 NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period 				= arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler 		= psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel 									 = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  				//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 = 3;  				//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE; 		//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2					 
}

void TIM3_SendLocal_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef 				 NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period 				= arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler 		= psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel 									 = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  				//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 = 3;  				//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE; 		//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3					 
}

void TIM4_Echo_Init(u16 arr,u16 psc)		//��������ʱ����ʼ��
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ITConfig(TIM4,TIM_IT_Update ,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  					 
}

void TIM2_IRQHandler(void)   //TIM2�жϣ�����ͣ��,��������
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	//���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		LED=!LED;
		
//		for(a=0;a<20;a++){														
//			USART_SendData(USART1, Rfid_Data[aa]);
//			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
//		}
	}
}

void TIM3_IRQHandler(void)   //TIM3�жϣ�����һ��RFIDλ����Ϣ
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		LED=!LED;
		if(SecureTime_Flag < 2 )	//�������ν�����ж�SecureTime_Flag>2ʱ˵���Ѿ�����1sû�еõ������Ļ�Ӧ�ˣ���λ�ڴ���2�жϷ���������0
			SecureTime_Flag++;
		for(a=0;a<20;a++){														//����һ��RFIDλ����Ϣ,�ɲ���20���ֽ�����
			USART_SendData(USART1, RFID_Data[aa]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
		}
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );
/***************************����Ϊ�����������ݸ�ʽ****************************/
		USART_SendData(USART2, 0xe8);
		while((USART2->SR&0X40)==0);//ѭ������,ֱ���������
		delay_us(40);
		USART_SendData(USART2, 0x02);
		while((USART2->SR&0X40)==0);//ѭ������,ֱ���������
		delay_us(40);
		USART_SendData(USART2, 0xb4);
		while((USART2->SR&0X40)==0);//ѭ������,ֱ���������
		delay_us(200);
/*******************************************************************************/		
	}
}










