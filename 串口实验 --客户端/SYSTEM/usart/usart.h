#ifndef __USART_H
#define __USART_H

#include "main.h" 

#define USART_REC_LEN  			200  				//�����������ֽ��� 200
#define EN_USART1_RX 			  1						//ʹ�ܣ�1��/��ֹ��0������1����
#define USART_DEBUG		USART1	  	

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         				//����״̬���	
extern u8 RevePort1_Data[20];
extern u8 RevePort2_Data[11];
extern u8 RFID_Data[20];
extern u8 CarArrive_Flag;

void CopeSerial1Data(unsigned char ucData);
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

#endif


