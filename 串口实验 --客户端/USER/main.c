#include "main.h"

uint8_t len=7;	
uint16_t times=0;


unsigned char Maddr1[4]		 = "26\r\n";
unsigned char ReSet[10]		 = "AT+RESET\r\n";
unsigned char BroadCast[4] = {0xAA,0xFB,0xFF,0xFF};			//广播前缀，后面跟数据
unsigned char MTP0[4] 		 = {0xAA,0xFB,0x00,0x20};
unsigned char MTP1[4] 		 = {0xAA,0xFB,0x00,0x21};					// point-to-point 点对点前缀，后面跟短地址加数据
unsigned char MTP2[4] 		 = {0xAA,0xFB,0x00,0x22};	
unsigned char MTP3[4] 		 = {0xAA,0xFB,0x00,0x23};	
unsigned char MTP4[4] 		 = {0xAA,0xFB,0x00,0x24};	
unsigned char MTP5[4] 		 = {0xAA,0xFB,0x00,0x25};	
unsigned char MTP6[4] 		 = {0xAA,0xFB,0x00,0x26};	
unsigned char New_Line[2]  = "\r\n";
unsigned char Send_Data[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
unsigned char I_am_go_Flag[3] = {0};

extern u8 RevePort1_Data[20];
extern u8 RevePort2_Data[11];
extern u16 Target_Seat[3];
u8 clear = 0;

extern u8 BUF[5];

void Zigbee_Init()
{
//**************************此处禁止改动***********************************//
	unsigned char NAME[12]     = "AT+NAME123\r\n";					//设置蓝牙名
	unsigned char NETID[22]    = "AT+NETID665544332211\r\n";//设置组网 ID (\r\n算两个)
	unsigned char MADDR[8]     = "AT+MADDR";								//设置组网短地址，后面跟要设置的短地址，要求每个模块所有短地址必须不同
	Usart_SendString(USART2,ReSet,10);	//复位
	delay_ms(200);
	Usart_SendString(USART2,NAME,12);	  //设置NAME
	delay_ms(100);
	Usart_SendString(USART2,NETID,22);	//设置组网ID号
	delay_ms(100);	
	Usart_SendString(USART2,MADDR,8);		//设置组网短地址前缀（禁改）
//**************************************************************************//
	Usart_SendString(USART2,Maddr1,4);	//设置组网短地址后缀（每个模块不同）
	delay_ms(100);
	Usart_SendString(USART2,ReSet,10);	//复位
	delay_ms(300);
}

void Zigbee_SendData(u8 *Data,u8 len)
{
//	Usart_SendString(USART2,&BroadCast[0],4);
//	Usart_SendString(USART2,Send_Data,6);
	Usart_SendString(USART2,Data,len);
	Usart_SendString(USART2,New_Line,2);
}


void Master_TO_P(u8 *AGVx,u8 *Data,u8 len)		//需改：加传Data长度参数
{
	Usart_SendString(USART2,AGVx,4);
	Usart_SendString(USART2,Data,len);
	Usart_SendString(USART2,New_Line,2);
}

/*因与delay函数的结构体冲突，因此无法使用
u16 InsureTime = 0;
void Set_InsureTime(u32 ms)		//设置保险时间(主机超过这个时间没有回应则报发出警告)
{
	InsureTime = ms*SystemCoreClock/8000;		//SystemCoreClock/8000000*1000((系统时钟的1/8)*1000=1ms)
	SysTick->LOAD=InsureTime; 							//时间加载
	SysTick->VAL=0x00;											//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;//开始倒数
}
void Send_Warn()
{
	temp=SysTick->CTRL;											//取时间还剩多少
	if(!(temp&0x01&&!(temp&(1<<16)))){			//代表超时
																					//停车处理代码
	}
}*/

int main(void)
{		
	delay_init();	    	 		 
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart1_init(9600);				 //RFID模块规定串口波特率为9600
	uart2_init(115200);
//	TIM2_Secure_Init(9999,7199);		//超时1s自动报警 //用SecureTime_Flag标志位方法取缔该定时器法
	TIM3_SendLocal_Init(4999,7199);	//每隔0.5s发送一个位置信息
	TIM4_Echo_Init(999,7199);				//每隔0.1s发送超声波指令
 	LED_Init();			     		
	KEY_Init();          		 
	Zigbee_Init();

 	while(1)
	{		
		if(Target_Seat[0]==0xdd){		//还缺：&&上服务端已发出指令	（已改）
			//SetCompare(120);设置占空比加速
			TIM_Cmd(TIM2, ENABLE);						//使能定时器2
			USART_Cmd(USART3, ENABLE);				//使能超声波串口
			Target_Seat[0] = 0;
			Master_TO_P(MTP0, I_am_go_Flag, 3);
		}
		
		if(CarArrive_Flag == 1){
//		根据压力和红外检测餐品是否被拿走
//		是：CarArrive_Flag = 0; TIM_Cmd(TIM2, ENABLE);
		}		
	}	 
}

