#include "main.h"

uint8_t len=7;	
uint16_t times=0;


unsigned char Maddr1[4]		 = "26\r\n";
unsigned char ReSet[10]		 = "AT+RESET\r\n";
unsigned char BroadCast[4] = {0xAA,0xFB,0xFF,0xFF};			//�㲥ǰ׺�����������
unsigned char MTP0[4] 		 = {0xAA,0xFB,0x00,0x20};
unsigned char MTP1[4] 		 = {0xAA,0xFB,0x00,0x21};					// point-to-point ��Ե�ǰ׺��������̵�ַ������
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
//**************************�˴���ֹ�Ķ�***********************************//
	unsigned char NAME[12]     = "AT+NAME123\r\n";					//����������
	unsigned char NETID[22]    = "AT+NETID665544332211\r\n";//�������� ID (\r\n������)
	unsigned char MADDR[8]     = "AT+MADDR";								//���������̵�ַ�������Ҫ���õĶ̵�ַ��Ҫ��ÿ��ģ�����ж̵�ַ���벻ͬ
	Usart_SendString(USART2,ReSet,10);	//��λ
	delay_ms(200);
	Usart_SendString(USART2,NAME,12);	  //����NAME
	delay_ms(100);
	Usart_SendString(USART2,NETID,22);	//��������ID��
	delay_ms(100);	
	Usart_SendString(USART2,MADDR,8);		//���������̵�ַǰ׺�����ģ�
//**************************************************************************//
	Usart_SendString(USART2,Maddr1,4);	//���������̵�ַ��׺��ÿ��ģ�鲻ͬ��
	delay_ms(100);
	Usart_SendString(USART2,ReSet,10);	//��λ
	delay_ms(300);
}

void Zigbee_SendData(u8 *Data,u8 len)
{
//	Usart_SendString(USART2,&BroadCast[0],4);
//	Usart_SendString(USART2,Send_Data,6);
	Usart_SendString(USART2,Data,len);
	Usart_SendString(USART2,New_Line,2);
}


void Master_TO_P(u8 *AGVx,u8 *Data,u8 len)		//��ģ��Ӵ�Data���Ȳ���
{
	Usart_SendString(USART2,AGVx,4);
	Usart_SendString(USART2,Data,len);
	Usart_SendString(USART2,New_Line,2);
}

/*����delay�����Ľṹ���ͻ������޷�ʹ��
u16 InsureTime = 0;
void Set_InsureTime(u32 ms)		//���ñ���ʱ��(�����������ʱ��û�л�Ӧ�򱨷�������)
{
	InsureTime = ms*SystemCoreClock/8000;		//SystemCoreClock/8000000*1000((ϵͳʱ�ӵ�1/8)*1000=1ms)
	SysTick->LOAD=InsureTime; 							//ʱ�����
	SysTick->VAL=0x00;											//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;//��ʼ����
}
void Send_Warn()
{
	temp=SysTick->CTRL;											//ȡʱ�仹ʣ����
	if(!(temp&0x01&&!(temp&(1<<16)))){			//����ʱ
																					//ͣ���������
	}
}*/

int main(void)
{		
	delay_init();	    	 		 
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart1_init(9600);				 //RFIDģ��涨���ڲ�����Ϊ9600
	uart2_init(115200);
//	TIM2_Secure_Init(9999,7199);		//��ʱ1s�Զ����� //��SecureTime_Flag��־λ����ȡ�޸ö�ʱ����
	TIM3_SendLocal_Init(4999,7199);	//ÿ��0.5s����һ��λ����Ϣ
	TIM4_Echo_Init(999,7199);				//ÿ��0.1s���ͳ�����ָ��
 	LED_Init();			     		
	KEY_Init();          		 
	Zigbee_Init();

 	while(1)
	{		
		if(Target_Seat[0]==0xdd){		//��ȱ��&&�Ϸ�����ѷ���ָ��	���Ѹģ�
			//SetCompare(120);����ռ�ձȼ���
			TIM_Cmd(TIM2, ENABLE);						//ʹ�ܶ�ʱ��2
			USART_Cmd(USART3, ENABLE);				//ʹ�ܳ���������
			Target_Seat[0] = 0;
			Master_TO_P(MTP0, I_am_go_Flag, 3);
		}
		
		if(CarArrive_Flag == 1){
//		����ѹ���ͺ������Ʒ�Ƿ�����
//		�ǣ�CarArrive_Flag = 0; TIM_Cmd(TIM2, ENABLE);
		}		
	}	 
}

