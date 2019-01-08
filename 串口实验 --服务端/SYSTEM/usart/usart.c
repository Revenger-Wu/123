#include "usart.h"

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif

#define AGV1_AGV2					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV1_AGV3					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV1_AGV4					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV1_AGV5					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV1_AGV6					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV2_AGV3					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV2_AGV4					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV2_AGV5					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV2_AGV6					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV3_AGV4					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV3_AGV5					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV3_AGV6					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV4_AGV5					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV4_AGV6					 (abs(AGV1_SeatData - AGV2_SeatData))
#define AGV5_AGV6					 (abs(AGV1_SeatData - AGV2_SeatData))

#define AGV1_SECURE_JUDGE	 ((AGV1_AGV2>2)&&(AGV1_AGV3>2)&&(AGV1_AGV4>2)&&(AGV1_AGV5>2)&&(AGV1_AGV6>2))
#define AGV2_SECURE_JUDGE	 ((AGV1_AGV2>2)&&(AGV2_AGV3>2)&&(AGV2_AGV4>2)&&(AGV2_AGV5>2)&&(AGV2_AGV6>2))
#define AGV3_SECURE_JUDGE	 ((AGV1_AGV3>2)&&(AGV2_AGV3>2)&&(AGV3_AGV4>2)&&(AGV3_AGV5>2)&&(AGV3_AGV6>2))
#define AGV4_SECURE_JUDGE	 ((AGV1_AGV4>2)&&(AGV2_AGV4>2)&&(AGV3_AGV4>2)&&(AGV4_AGV5>2)&&(AGV4_AGV6>2))
#define AGV5_SECURE_JUDGE	 ((AGV1_AGV5>2)&&(AGV2_AGV5>2)&&(AGV3_AGV5>2)&&(AGV4_AGV5>2)&&(AGV5_AGV6>2))
#define AGV6_SECURE_JUDGE	 ((AGV1_AGV6>2)&&(AGV2_AGV6>2)&&(AGV3_AGV6>2)&&(AGV4_AGV6>2)&&(AGV5_AGV6>2))


#define AGV_SECURE_JUDGE 	 ((abs(AGV1_SeatData - AGV6_SeatData>2))&& \
														(abs(AGV1_SeatData - AGV2_SeatData>2))&& \
														(abs(AGV1_SeatData - AGV3_SeatData>2))&& \
														(abs(AGV1_SeatData - AGV4_SeatData>2))&& \
														(abs(AGV1_SeatData - AGV5_SeatData>2))   )												
u8  RecPort1_Num			 = 0;
u8  RecPort2_Num			 = 0;
u8  count							 = 0;
u8  t 						     = 0;
//u8	Table_Numble			 = 0;			//����������������
u32 AGV1_SeatData		   = 0;			//��AGVС����ǰ��λ����Ϣ
u32 AGV2_SeatData 		 = 0;
u32 AGV3_SeatData 		 = 0;
u32 AGV4_SeatData 		 = 0;
u32 AGV5_SeatData 		 = 0;
u32 AGV6_SeatData 		 = 0;
u8  U_are_One_Flag		 = 0;
u8  Master_Send_Flag 	 = 0;
u8  AGV_Here_Flag			 = 0;			//AGV���ڵ�һ��������״̬��־λ
u8  It_is_go_Flag			 = 0;			//AGV�ѳ�����־λ

u8  ReveTouch_Data[11] = {0};
u8  RevePort2_Data[11] = {0};
u8  buf[4]						 = {0};  
u8  BUF[5]						 = {0};  
u8  Rfid_Data[20]			 = {0};
u8	Table_Numble[3]		 = {0};		//���������������� ��һλ�Ǳ�־�ţ��ڶ�λ��RFID�ţ�����λ����λ��(����������)
/*u8  AGV1_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x13,0x13};	//AGV���а�ȫ��Ϣ
u8  AGV2_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x23,0x23};	
u8  AGV3_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x33,0x33};	
u8  AGV4_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x43,0x43};	
u8  AGV5_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x53,0x53};	
u8  AGV6_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x63,0x63};	
u8  AGV1_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x14,0x14};	//AGV���в���ȫ��Ϣ
u8  AGV2_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x24,0x24};	
u8  AGV3_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x34,0x34};	
u8  AGV4_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x44,0x44};	
u8  AGV5_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x54,0x54};	
u8  AGV6_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x64,0x64};	*/

u8  AGV1_Secure[2]		 = {0x13,0x13};	//AGV���а�ȫ��Ϣ
u8  AGV2_Secure[2]		 = {0x23,0x23};	
u8  AGV3_Secure[2]		 = {0x33,0x33};	
u8  AGV4_Secure[2]		 = {0x43,0x43};	
u8  AGV5_Secure[2]		 = {0x53,0x53};	
u8  AGV6_Secure[2]		 = {0x63,0x63};	
u8  AGV1_UnSecure[2]	 = {0x14,0x14};	//AGV���в���ȫ��Ϣ
u8  AGV2_UnSecure[2]	 = {0x24,0x24};	
u8  AGV3_UnSecure[2]	 = {0x34,0x34};	
u8  AGV4_UnSecure[2]	 = {0x44,0x44};	
u8  AGV5_UnSecure[2]	 = {0x54,0x54};	
u8  AGV6_UnSecure[2]	 = {0x64,0x64};	

//u8  Table_Buf[]				 = {0x23};
//u8  I_am_one_Flag[6]	 = {0xAA,0xFB,0xFF,0xFF,0xab,0xab};
//u8  U_are_One_Flag[6]	 = {0xAA,0xFB,0xFF,0xFF,0xbb,0xbb};

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
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart1_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
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

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
void uart2_init(u32 bound){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = bound;               
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 

	/* ʹ�ܴ���2�����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* ʹ�ܴ���2���߿����ж� */
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
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
void Usart_SendString(USART_TypeDef *USARTx, u8 *str, unsigned short len)
{
	unsigned short count = 0;
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}
}
void USART1_IRQHandler(void)                	//����1�������Դ�����������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		if(AGV_Here_Flag == 1){
			ReveTouch_Data[RecPort1_Num++] = USART_ReceiveData(USART1);	//����������
			if((ReveTouch_Data[0]==0xAA)&&(ReveTouch_Data[10]==0x0A)){	//���յ����Դ�������һ������
				Table_Numble[0] = ReveTouch_Data[3];					//����λ��Ϊ���ű�־λ
				Table_Numble[1] = ReveTouch_Data[4];
				
				Master_Send_Flag = 1;
				
			//	AGV_Here_Flag = 0;					//����2��,��main��������0
			}
		}
		
		if((ReveTouch_Data[0]!=0xAA)&&(ReveTouch_Data[10]!=0x0A)){
			RecPort1_Num = 0;	
			for(t=0;t<11;t++)
				ReveTouch_Data[t] = 0x00;
		}
		if(RecPort1_Num>10){
			RecPort1_Num = 0;	
			for(t=0;t<11;t++)
				ReveTouch_Data[t] = 0x00;
		}
	}	
}
void USART2_IRQHandler(void)                	//����2��AGVͨ��
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//�յ�����	
	{				
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);			//����жϱ�־λ
		RevePort2_Data[RecPort2_Num++] = USART_ReceiveData(USART2);    //����������
		if((RevePort2_Data[0]==0xAA)&&(RevePort2_Data[10]==0x0A)){
			if(RevePort2_Data[4]==0x01){								//01����AGV1
				AGV1_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]�������ǵڼ���RFID������
				if(AGV1_SECURE_JUDGE)											//����ȫ����
					Master_TO_P(MTP1,AGV1_Secure,2);
				else
					Master_TO_P(MTP1,AGV1_Secure,2);						
			}
			if(RevePort2_Data[4]==0x02){								//02����AGV2
				AGV2_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]�������ǵڼ���RFID������
				if(AGV2_SECURE_JUDGE)											//����ȫ����
					Master_TO_P(MTP2,AGV2_Secure,2);
				else
					Master_TO_P(MTP2,AGV2_Secure,2);						
			}
			if(RevePort2_Data[4]==0x03){								//03����AGV3
				AGV3_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]�������ǵڼ���RFID������
				if(AGV3_SECURE_JUDGE)											//����ȫ����
					Master_TO_P(MTP3,AGV3_Secure,2);
				else
					Master_TO_P(MTP3,AGV3_Secure,2);							
			}
			if(RevePort2_Data[4]==0x04){								//04����AGV4
				AGV4_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]�������ǵڼ���RFID������
				if(AGV4_SECURE_JUDGE)											//����ȫ����
					Master_TO_P(MTP4,AGV4_Secure,2);
				else
					Master_TO_P(MTP4,AGV4_Secure,2);							
			}
			if(RevePort2_Data[4]==0x05){								//05����AGV5
				AGV5_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]�������ǵڼ���RFID������
				if(AGV5_SECURE_JUDGE)											//����ȫ����
					Master_TO_P(MTP5,AGV5_Secure,2);
				else
					Master_TO_P(MTP5,AGV5_Secure,2);						
			}
			if(RevePort2_Data[4]==0x06){								//06����AGV6
				AGV6_SeatData = RevePort2_Data[5];				//RevePort2_Data[5]�������ǵڼ���RFID������
				if((AGV6_SECURE_JUDGE)&&(AGV6_SeatData != 0xDD))	//����ȫ���� //��ģ���Ϊ�Ӳ���RFID��ֹ0xDDһ��
					Master_TO_P(MTP6,AGV6_Secure,2);			//��ĳɣ�Master_TO_P()		���Ѹģ�
				else																//�˴�else �߼������⣡��Ӧ�ò���else
					Master_TO_P(MTP6,AGV6_UnSecure,2);		
				
				if(AGV6_SeatData == 0xDD){						//���յ���һ��������AGV��λ�ź�
//���Բ�Ҫ������Ӧ 	Master_TO_P(MTP6,&U_are_One_Flag);	//��Ӧ��һ��������AGV������һ��������AGVû���յ��˻�Ӧ�򲻶Ϸ���I_am_one_Flag
					AGV_Here_Flag = 1;									//����1��
				}		
			}
						
			RecPort2_Num = 0;	
			for(t=0;t<11;t++)
				RevePort2_Data[t] = 0x00;
		}
		
		if((RevePort2_Data[0]!=0xaa)&&(RevePort2_Data[10]!=0x0a)){
			RecPort2_Num = 0;	
			for(t=0;t<11;t++)
				RevePort2_Data[t] = 0x00;
		}
		if(RecPort2_Num>10){
			RecPort2_Num = 0;	
			for(t=0;t<11;t++)
				RevePort2_Data[t] = 0x00;
		}
	}
} 
#endif	

