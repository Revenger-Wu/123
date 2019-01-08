#include "main.h"

//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif

u8 count=0;			//Usart_SendString()用

// Serial Port 1 
extern unsigned char MTP0[4];
u8  a1 = 0;												//RevePort1_Data给RFID_Data计数赋值
u8  RecPort1_Num=0;
u8  t1=0;									//串口2计数清空数组用
u8  CarArrive_Flag = 0;		//AGV到达目标餐桌标志位
u8  RevePort1_Data[20] = {0};
u8  RFID_Data[20] 		 = {0};				//串口1读取RFID和定时器3发送此数据
u8  Arrival_Where[3]	 = {0x21,0,0};	//第一位表示此车号，第二三位表示检测的RFID值(到哪里了)
u16 Target_Seat[3]		 = {0};				//第一位是RFID号，第二位是座位号(语音播报用)

// Serial Port 2
u8  RecPort2_Num = 0;
u8  t2=0;									//串口2计数清空数组用
u8  Safety_Flag = 0;				//安全标志位
u8  RevePort2_Data[11] = {0,0,0,0,0,0,0,0,0,0,0};
u8  Reve_TargetSeat_Flag = 0;

// Serial Port 3
u8  RecPort3_Num = 0;
u8  Echo_Data[2] = {0};
u8  t3 = 0;
u16 distance = 0;
u8  EchoWarning_Flag = 0;

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
  
void uart1_init(u32 bound){
    //GPIO端口设置
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
     //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel 									= USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd 							= ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate   					= bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits 	 					= USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity 		 					= USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3 ;	//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority				= 3;	//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);														//根据指定的参数初始化VIC寄存器
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate 						= bound;               
	USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 						= USART_StopBits_1;
	USART_InitStructure.USART_Parity 							= USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 								= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 

	/* 使能串口2接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* 使能串口2总线空闲中断 */
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}
void uart3_init(u32 bound){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10 
	

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
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

  USART_Init(USART3, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
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
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
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
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}
}
void USART1_IRQHandler(void)				//串口1--读取RFID信息
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//收到数据	
	{				
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);			//清除中断标志位
		RFID_Data[RecPort1_Num++] = USART_ReceiveData(USART1);    //建立缓存区
		
		if((RevePort1_Data[0]==0x24)&&(RevePort1_Data[19]==0x23)){
			for(a1=0;a1<20;a1++)					//如果可以任意改标签信息，则可以不用循环20次写入
				RFID_Data[a1] = RevePort1_Data[a1];
/*			
			if(接收到了弯道前的RFID信息){		//这些可不在串口中断内实现
				处理电机的运转（刹车）;
			}
			if(接收到了弯道后的RFID信息){
				处理电机的运转（加速）;
			}
			if(读取到了目标前两位餐桌的RFID信息){
				处理电机的运转（刹车）;
			}
			if(读取到了目标餐桌的RFID信息){
				CarArrive_Flag = 1;
				TIM_Cmd(TIM2, DISABLE);
			}
			if(接收到了接餐窗口的RFID信息){
				停车;
				告知厨师主机自己是几号AGV已就绪接餐;
				等待厨师主机发送过来位置信息再发车;
			}
*/			
			
/**************************此段为回到厨师位电机处理程序**********************************/
			if((RFID_Data[3] == 0xda)&&(RFID_Data[4] == 0xda)){			//假设第四五位是存储标签号的,0xda表示即将到达厨师位准备刹车
				//刹车程序;
			}
			if((RFID_Data[3] == 0xdd)&&(RFID_Data[4] == 0xdd)){
				//刹死程序：SetCompare(0);
				TIM_Cmd(TIM2, DISABLE);						//失能定时器2
				USART_Cmd(USART3, DISABLE); 			//失能超声波串口 避免人为手干扰超声波导致使能定时器2
				Arrival_Where[1] = 0xdd;					//RFID_Data[3]
				Arrival_Where[2] = 0xdd;
				Master_TO_P(MTP0,Arrival_Where,3);	//点对点告诉主机自己的车号和位置号
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
void USART2_IRQHandler(void)				//串口2--AGV与主机之间通信
{																		//用于Zigbee之间的通信
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//收到数据	
	{				
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);			//清除中断标志位
		RevePort2_Data[RecPort2_Num++] = USART_ReceiveData(USART2);		//建立缓存区
		if((RevePort2_Data[0]==0xAA)&&(RevePort2_Data[10]==0x0A)){
			if(RevePort2_Data[3] == 0x11)			//前1代表AGV代号，后1代表安全信号
				Safety_Flag = 0;
			if(RevePort2_Data[3] == 0x12)			//02代表停车信号
				Safety_Flag = 1;
			SecureTime_Flag = 0;		//归0
//			TIM2->CNT = 0;								//TIM_SetCounter(TIM2,0);	//归0
			if((Arrival_Where[1]==0xdd)&&(Arrival_Where[2]==0xdd)&&(RevePort2_Data[4]==0xdd)){	//表示到达接餐口并接收到主机发来的位置信息
				Target_Seat[0] = RevePort2_Data[4];		//表示接收到主机发来的位置信息
				Target_Seat[1] = RevePort2_Data[5];		//需改：主机端发送的Table_Numble是3位数据数组(或主机端改)
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
/*		Reve_Data[RecNum++] = USART_ReceiveData(USART2);    //建立缓存区
		USART_SendData(USART1, Reve_Data[RecNum-1]);	//发送给串口1 串口2接收到的数据
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
void USART3_IRQHandler(void)		//接收超声波返回数据
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	//收到数据	
	{				
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Echo_Data[RecPort3_Num++] = USART_ReceiveData(USART3);
		
		if(RecPort3_Num>1){
			distance = Echo_Data[0]*256+Echo_Data[1];
			if(distance>100){		//安全,100代表1米
				if(EchoWarning_Flag == 1){
					EchoWarning_Flag = 0;
					TIM_Cmd(TIM2, ENABLE);
				}
			}
			else{
				//SetCompare(0);设置占空比减速刹车
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

