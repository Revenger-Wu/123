#include "usart.h"

//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
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
//u8	Table_Numble			 = 0;			//触摸屏传来的桌号
u32 AGV1_SeatData		   = 0;			//各AGV小车当前的位置信息
u32 AGV2_SeatData 		 = 0;
u32 AGV3_SeatData 		 = 0;
u32 AGV4_SeatData 		 = 0;
u32 AGV5_SeatData 		 = 0;
u32 AGV6_SeatData 		 = 0;
u8  U_are_One_Flag		 = 0;
u8  Master_Send_Flag 	 = 0;
u8  AGV_Here_Flag			 = 0;			//AGV处于第一个待出发状态标志位
u8  It_is_go_Flag			 = 0;			//AGV已出发标志位

u8  ReveTouch_Data[11] = {0};
u8  RevePort2_Data[11] = {0};
u8  buf[4]						 = {0};  
u8  BUF[5]						 = {0};  
u8  Rfid_Data[20]			 = {0};
u8	Table_Numble[3]		 = {0};		//触摸屏传来的桌号 第一位是标志号，第二位是RFID号，第三位是座位号(语音播报用)
/*u8  AGV1_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x13,0x13};	//AGV运行安全信息
u8  AGV2_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x23,0x23};	
u8  AGV3_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x33,0x33};	
u8  AGV4_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x43,0x43};	
u8  AGV5_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x53,0x53};	
u8  AGV6_Secure[6]		 = {0xAA,0xFB,0xFF,0xFF,0x63,0x63};	
u8  AGV1_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x14,0x14};	//AGV运行不安全信息
u8  AGV2_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x24,0x24};	
u8  AGV3_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x34,0x34};	
u8  AGV4_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x44,0x44};	
u8  AGV5_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x54,0x54};	
u8  AGV6_UnSecure[6]	 = {0xAA,0xFB,0xFF,0xFF,0x64,0x64};	*/

u8  AGV1_Secure[2]		 = {0x13,0x13};	//AGV运行安全信息
u8  AGV2_Secure[2]		 = {0x23,0x23};	
u8  AGV3_Secure[2]		 = {0x33,0x33};	
u8  AGV4_Secure[2]		 = {0x43,0x43};	
u8  AGV5_Secure[2]		 = {0x53,0x53};	
u8  AGV6_Secure[2]		 = {0x63,0x63};	
u8  AGV1_UnSecure[2]	 = {0x14,0x14};	//AGV运行不安全信息
u8  AGV2_UnSecure[2]	 = {0x24,0x24};	
u8  AGV3_UnSecure[2]	 = {0x34,0x34};	
u8  AGV4_UnSecure[2]	 = {0x44,0x44};	
u8  AGV5_UnSecure[2]	 = {0x54,0x54};	
u8  AGV6_UnSecure[2]	 = {0x64,0x64};	

//u8  Table_Buf[]				 = {0x23};
//u8  I_am_one_Flag[6]	 = {0xAA,0xFB,0xFF,0xFF,0xab,0xab};
//u8  U_are_One_Flag[6]	 = {0xAA,0xFB,0xFF,0xFF,0xbb,0xbb};

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
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
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = bound;               
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 

	/* 使能串口2接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* 使能串口2总线空闲中断 */
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
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
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
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}
}
void USART1_IRQHandler(void)                	//串口1接收来自触摸屏的数据
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		if(AGV_Here_Flag == 1){
			ReveTouch_Data[RecPort1_Num++] = USART_ReceiveData(USART1);	//建立缓存区
			if((ReveTouch_Data[0]==0xAA)&&(ReveTouch_Data[10]==0x0A)){	//接收到来自触摸屏的一段数组
				Table_Numble[0] = ReveTouch_Data[3];					//将此位作为桌号标志位
				Table_Numble[1] = ReveTouch_Data[4];
				
				Master_Send_Flag = 1;
				
			//	AGV_Here_Flag = 0;					//串口2用,在main函数里置0
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
void USART2_IRQHandler(void)                	//串口2与AGV通信
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//收到数据	
	{				
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);			//清除中断标志位
		RevePort2_Data[RecPort2_Num++] = USART_ReceiveData(USART2);    //建立缓存区
		if((RevePort2_Data[0]==0xAA)&&(RevePort2_Data[10]==0x0A)){
			if(RevePort2_Data[4]==0x01){								//01代表AGV1
				AGV1_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]的数据是第几个RFID的数字
				if(AGV1_SECURE_JUDGE)											//代表安全距离
					Master_TO_P(MTP1,AGV1_Secure,2);
				else
					Master_TO_P(MTP1,AGV1_Secure,2);						
			}
			if(RevePort2_Data[4]==0x02){								//02代表AGV2
				AGV2_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]的数据是第几个RFID的数字
				if(AGV2_SECURE_JUDGE)											//代表安全距离
					Master_TO_P(MTP2,AGV2_Secure,2);
				else
					Master_TO_P(MTP2,AGV2_Secure,2);						
			}
			if(RevePort2_Data[4]==0x03){								//03代表AGV3
				AGV3_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]的数据是第几个RFID的数字
				if(AGV3_SECURE_JUDGE)											//代表安全距离
					Master_TO_P(MTP3,AGV3_Secure,2);
				else
					Master_TO_P(MTP3,AGV3_Secure,2);							
			}
			if(RevePort2_Data[4]==0x04){								//04代表AGV4
				AGV4_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]的数据是第几个RFID的数字
				if(AGV4_SECURE_JUDGE)											//代表安全距离
					Master_TO_P(MTP4,AGV4_Secure,2);
				else
					Master_TO_P(MTP4,AGV4_Secure,2);							
			}
			if(RevePort2_Data[4]==0x05){								//05代表AGV5
				AGV5_SeatData = RevePort2_Data[4];				//RevePort2_Data[4]的数据是第几个RFID的数字
				if(AGV5_SECURE_JUDGE)											//代表安全距离
					Master_TO_P(MTP5,AGV5_Secure,2);
				else
					Master_TO_P(MTP5,AGV5_Secure,2);						
			}
			if(RevePort2_Data[4]==0x06){								//06代表AGV6
				AGV6_SeatData = RevePort2_Data[5];				//RevePort2_Data[5]的数据是第几个RFID的数字
				if((AGV6_SECURE_JUDGE)&&(AGV6_SeatData != 0xDD))	//代表安全距离 //需改：因为接餐区RFID不止0xDD一个
					Master_TO_P(MTP6,AGV6_Secure,2);			//需改成：Master_TO_P()		（已改）
				else																//此处else 逻辑有问题！不应该采用else
					Master_TO_P(MTP6,AGV6_UnSecure,2);		
				
				if(AGV6_SeatData == 0xDD){						//接收到第一辆待出发AGV就位信号
//可以不要这条回应 	Master_TO_P(MTP6,&U_are_One_Flag);	//回应第一辆待出发AGV，若第一辆待出发AGV没有收到此回应则不断发送I_am_one_Flag
					AGV_Here_Flag = 1;									//串口1用
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

