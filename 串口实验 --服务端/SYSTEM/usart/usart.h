#ifndef __USART_H
#define __USART_H
#include "main.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USART_DEBUG		USART1	  	

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         				//����״̬���	
extern u8  BUF[5];
extern u8  ReveTouch_Data[11];
extern u8  RevePort2_Data[11];
extern u8  Rfid_Data[20];

extern unsigned char MTP1[4];					// point-to-point ��Ե�ǰ׺��������̵�ַ������
extern unsigned char MTP2[4];	
extern unsigned char MTP3[4];	
extern unsigned char MTP4[4];	
extern unsigned char MTP5[4];	
extern unsigned char MTP6[4];	
extern u8	 Table_Numble[3];			//����������������

extern u32 AGV1_SeatData;			//��AGVС����ǰ��λ����Ϣ
extern u32 AGV2_SeatData;
extern u32 AGV3_SeatData;
extern u32 AGV4_SeatData;
extern u32 AGV5_SeatData;
extern u32 AGV6_SeatData;
extern u8  Master_Send_Flag;
extern u8  It_is_go_Flag;			//AGV�ѳ�����־λ

void CopeSerial1Data(unsigned char ucData);
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);
void Usart_SendString(USART_TypeDef *USARTx, u8 *str, unsigned short len);
void test_Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

#endif


