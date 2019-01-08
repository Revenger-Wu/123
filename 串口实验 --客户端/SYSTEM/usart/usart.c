#include "main.h"

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif

u8 count=0;			//Usart_SendString()��

// Serial Port 1 
extern unsigned char MTP0[4];
u8  a1 = 0;												//RevePort1_Data��RFID_Data������ֵ
u8  RecPort1_Num=0;
u8  t1=0;									//����2�������������
u8  CarArrive_Flag = 0;		//AGV����Ŀ�������־λ
u8  RevePort1_Data[20] = {0};
u8  RFID_Data[20] 		 = {0};				//����1��ȡRFID�Ͷ�ʱ��3���ʹ�����
u8  Arrival_Where[3]	 = {0x21,0,0};	//��һλ��ʾ�˳��ţ��ڶ���λ��ʾ����RFIDֵ(��������)
u16 Target_Seat[3]		 = {0};				//��һλ��RFID�ţ��ڶ�λ����λ��(����������)

// Serial Port 2
u8  RecPort2_Num = 0;
u8  t2=0;									//����2�������������
u8  Safety_Flag = 0;				//��ȫ��־λ
u8  RevePort2_Data[11] = {0,0,0,0,0,0,0,0,0,0,0};
u8  Reve_TargetSeat_Flag = 0;

// Serial Port 3
u8  RecPort3_Num = 0;
u8  Echo_Data[2] = {0};
u8  t3 = 0;
u16 distance = 0;
u8  EchoWarning_Flag = 0;

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
  
void uart1_init(u32 bound){
    //GPIO�˿�����
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
     //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ����

	NVIC_InitStructure.NVIC_IRQChannel 									= USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd 							= ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate   					= bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 	 					= USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity 		 					= USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}
void uart2_init(u32 bound){
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	NVIC_InitStructure.NVIC_IRQChannel 									= USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3 ;	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority				= 3;	//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate 						= bound;               
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;
	USART_InitStructure.USART_Parity 							= USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 

	/* ʹ�ܴ���2�����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* ʹ�ܴ���2���߿����ж� */
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}
void uart3_init(u32 bound){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10 
	

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);    
}

void CopeSerial1Data(unsigned char ucData)
{
}
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//��ʽ��
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{
	unsigned short count = 0;
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}
}
void USART1_IRQHandler(void)				//����1--��ȡRFID��Ϣ
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//�յ�����	
	{				
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);			//����жϱ�־λ
		RFID_Data[RecPort1_Num++] = USART_ReceiveData(USART1);    //����������
		
		if((RevePort1_Data[0]==0x24)&&(RevePort1_Data[19]==0x23)){
			for(a1=0;a1<20;a1++)					//�����������ı�ǩ��Ϣ������Բ���ѭ��20��д��
				RFID_Data[a1] = RevePort1_Data[a1];
/*			
			if(���յ������ǰ��RFID��Ϣ){		//��Щ�ɲ��ڴ����ж���ʵ��
				����������ת��ɲ����;
			}
			if(���յ���������RFID��Ϣ){
				����������ת�����٣�;
			}
			if(��ȡ����Ŀ��ǰ��λ������RFID��Ϣ){
				����������ת��ɲ����;
			}
			if(��ȡ����Ŀ�������RFID��Ϣ){
				CarArrive_Flag = 1;
				TIM_Cmd(TIM2, DISABLE);
			}
			if(���յ��˽Ӳʹ��ڵ�RFID��Ϣ){
				ͣ��;
				��֪��ʦ�����Լ��Ǽ���AGV�Ѿ����Ӳ�;
				�ȴ���ʦ�������͹���λ����Ϣ�ٷ���;
			}
*/			
			
/**************************�˶�Ϊ�ص���ʦλ����������**********************************/
			if((RFID_Data[3] == 0xda)&&(RFID_Data[4] == 0xda)){			//���������λ�Ǵ洢��ǩ�ŵ�,0xda��ʾ���������ʦλ׼��ɲ��
				//ɲ������;
			}
			if((RFID_Data[3] == 0xdd)&&(RFID_Data[4] == 0xdd)){
				//ɲ������SetCompare(0);
				TIM_Cmd(TIM2, DISABLE);						//ʧ�ܶ�ʱ��2
				USART_Cmd(USART3, DISABLE); 			//ʧ�ܳ��������� ������Ϊ�ָ��ų���������ʹ�ܶ�ʱ��2
				Arrival_Where[1] = 0xdd;					//RFID_Data[3]
				Arrival_Where[2] = 0xdd;
				Master_TO_P(MTP0,Arrival_Where,3);	//��Ե���������Լ��ĳ��ź�λ�ú�
			}
/*****************************************************************************************/
			
			RecPort1_Num = 0;	
			for(t1=0;t1<20;t1++)
				RevePort1_Data[t1] = 0x00;
		}
		
		if((RevePort1_Data[0]!=0x24)&&(RevePort1_Data[19]!=0x23)){
			RecPort1_Num = 0;	
			for(t1=0;t1<20;t1++)
				RevePort1_Data[t1] = 0x00;
		}
		if(RecPort1_Num>19){
			RecPort1_Num = 0;	
			for(t1=0;t1<20;t1++)
				RevePort1_Data[t1] = 0x00;
		}	
	}
} 
void USART2_IRQHandler(void)				//����2--AGV������֮��ͨ��
{																		//����Zigbee֮���ͨ��
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//�յ�����	
	{				
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);			//����жϱ�־λ
		RevePort2_Data[RecPort2_Num++] = USART_ReceiveData(USART2);		//����������
		if((RevePort2_Data[0]==0xAA)&&(RevePort2_Data[10]==0x0A)){
			if(RevePort2_Data[3] == 0x11)			//ǰ1����AGV���ţ���1����ȫ�ź�
				Safety_Flag = 0;
			if(RevePort2_Data[3] == 0x12)			//02����ͣ���ź�
				Safety_Flag = 1;
			SecureTime_Flag = 0;		//��0
//			TIM2->CNT = 0;								//TIM_SetCounter(TIM2,0);	//��0
			if((Arrival_Where[1]==0xdd)&&(Arrival_Where[2]==0xdd)&&(RevePort2_Data[4]==0xdd)){	//��ʾ����ӲͿڲ����յ�����������λ����Ϣ
				Target_Seat[0] = RevePort2_Data[4];		//��ʾ���յ�����������λ����Ϣ
				Target_Seat[1] = RevePort2_Data[5];		//��ģ������˷��͵�Table_Numble��3λ��������(�������˸�)
				Target_Seat[2] = RevePort2_Data[6];
//				Master_TO_P(MTP0, &Reve_TargetSeat_Flag);
			}
			
			RecPort2_Num = 0;	
			for(t2=0;t2<11;t2++)
				RevePort2_Data[t2] = 0x00;
		}
		
		if((RevePort2_Data[0]!=0xAA)&&(RevePort2_Data[10]!=0x0A)){
			RecPort2_Num = 0;	
			for(t2=0;t2<11;t2++)
				RevePort2_Data[t2] = 0x00;
		}
		if(RecPort2_Num>10){
			RecPort2_Num = 0;	
			for(t2=0;t2<11;t2++)
				RevePort2_Data[t2] = 0x00;
		}
//*****************************TEST NORMAL******************************//		
/*		Reve_Data[RecNum++] = USART_ReceiveData(USART2);    //����������
		USART_SendData(USART1, Reve_Data[RecNum-1]);	//���͸�����1 ����2���յ�������
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	
	
		if((Reve_Data[0]==0xAA)&&(Reve_Data[10]==0x0A)){
			RecNum = 0;	
			for(t=0;t<11;t++)
				Reve_Data[t] = 0x00;
			LED=0;
			delay_ms(100);
			LED=1;
			delay_ms(100);
		}
		if((Reve_Data[0]!=0xAA)&&(Reve_Data[10]!=0x0A)){
			RecNum = 0;	
			for(t=0;t<11;t++)
				Reve_Data[t] = 0x00;
		}
		
		if(RecNum>10){
			RecNum = 0;	
			for(t=0;t<11;t++)
				Reve_Data[t] = 0x00;
		}
		*/
		
	}
}
void USART3_IRQHandler(void)		//���ճ�������������
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//�յ�����	
	{				
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Echo_Data[RecPort3_Num++] = USART_ReceiveData(USART3);
		
		if(RecPort3_Num>1){
			distance = Echo_Data[0]*256+Echo_Data[1];
			if(distance>100){		//��ȫ,100����1��
				if(EchoWarning_Flag == 1){
					EchoWarning_Flag = 0;
					TIM_Cmd(TIM2, ENABLE);
				}
			}
			else{
				//SetCompare(0);����ռ�ձȼ���ɲ��
				TIM_Cmd(TIM2, DISABLE);
				EchoWarning_Flag = 1;
			}
			
			RecPort3_Num = 0;	
			for(t3=0;t3<2;t3++)
				RevePort2_Data[t3] = 0x00;
		}
	}
	
}


#endif	

