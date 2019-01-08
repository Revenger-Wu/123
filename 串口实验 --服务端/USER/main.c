#include "main.h"

unsigned char Maddr1[4]    = "24\r\n";
unsigned char ReSet[10]		 = "AT+RESET\r\n";
unsigned char BroadCast[10]= {0xAA,0xFB,0xFF,0xFF,0x11,0x22,0x33,0x44,0x55,0x66};			//�㲥ǰ׺�����������
unsigned char MTP1[4] 		 = {0xAA,0xFB,0x00,0x21};					// point-to-point ��Ե�ǰ׺��������̵�ַ������
unsigned char MTP2[4] 		 = {0xAA,0xFB,0x00,0x22};	
unsigned char MTP3[4] 		 = {0xAA,0xFB,0x00,0x23};	
unsigned char MTP4[4] 		 = {0xAA,0xFB,0x00,0x24};	
unsigned char MTP5[4] 		 = {0xAA,0xFB,0x00,0x25};	
unsigned char MTP6[4] 		 = {0xAA,0xFB,0x00,0x26};	
unsigned char New_Line[2]  = "\r\n";
unsigned char Send_Data[6] = {0x11,0x22,0x33,0x44,0x55,0x66};

//char *NAME1="AT+NAME152\r\n";
//char *NETID1="AT+NETID665544332211\r\n";
extern u8 AGV_Here_Flag;
extern u8 BUF[5];
extern u8 ReveTouch_Data[11];
extern u8 RevePort2_Data[11];
extern unsigned char Reve_Data[11];

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

void Zigbee_SendData(u8 *Data,u8 len)				//��ģ��Ӵ�Data���Ȳ���
{
	Usart_SendString(USART2,Data,len);
//	Usart_SendString(USART2,BroadCast,10);
//	Usart_SendString(USART2,Send_Data,6);
	Usart_SendString(USART2,New_Line,2);
}
void Master_TO_P(u8 *AGVx,u8 *Data,u8 len)
{
	Usart_SendString(USART2,AGVx,4);
	Usart_SendString(USART2,Data,len);
	Usart_SendString(USART2,New_Line,2);
}

int main(void)
{		
	delay_init();	    	 		 
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart1_init(115200);	     
	uart2_init(115200);
 	LED_Init();			     		
	KEY_Init();          		 
	Zigbee_Init();
	
 	while(1)
	{
		if(Master_Send_Flag == 1){							//��������������ȷ������
			if(AGV1_SeatData == 0xab)							//�ж���̨AGVλ�ڽӲʹ���
				Master_TO_P(MTP1,Table_Numble,3);		//���͵�ǰ���ţ��ȴ���ӦI_am_go_Flag���ѳ�����־λ
			if(AGV2_SeatData == 0xab)
				Master_TO_P(MTP2,Table_Numble,3);		//����İ취�����߼���ͨ���Ż�
			if(AGV3_SeatData == 0xab)
				Master_TO_P(MTP3,Table_Numble,3);
			if(AGV4_SeatData == 0xab)
				Master_TO_P(MTP4,Table_Numble,3);
			if(AGV5_SeatData == 0xab)
				Master_TO_P(MTP5,Table_Numble,3);
			if(AGV6_SeatData == 0xab)
				Master_TO_P(MTP6,Table_Numble,3);	
			delay_ms(200);											//��ʱ��Ϊ�˷�ֹAGV_Here_Flag����� �պô���2�ٴη������ݸ�������AGV�ڽӲʹ���
			AGV_Here_Flag = 0;
		}
		if(It_is_go_Flag == 1){								//��һ��������AGVȷ���Ѿ��뿪
			It_is_go_Flag = 0;
			Master_Send_Flag = 0;
			Table_Numble[0] = 0;
			Table_Numble[1] = 0;
			AGV1_SeatData = 0;									//����İ취
			AGV2_SeatData = 0;
			AGV3_SeatData = 0;
			AGV4_SeatData = 0;
			AGV5_SeatData = 0;
			AGV6_SeatData = 0;
		}
//		Zigbee_SendData(BroadCast);
//		delay_ms(1000);
//		Usart_SendString(USART2,BroadCast,4);
//		Usart_SendString(USART2,Data,9);
//		Usart_SendString(USART2,New_Line,2);
		
//		Usart_SendString(USART2,NAME,12);	//		printf("AT+NAME123\r\n");
/*		if((Reve_Data[0]==0xAA)&&Reve_Data[7]==0x0A)
		{
			LED=0;
			delay_ms(200);
			LED=1;
			delay_ms(200);
			if(RecNum>7)
			{
				RecNum = 0;	
				for(t=0;t<7;t++)
					Reve_Data[t] = 0x00;
			}
		}*/
	}	 
}

