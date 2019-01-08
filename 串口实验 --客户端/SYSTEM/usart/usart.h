#ifndef __USART_H
#define __USART_H

#include "main.h" 

#define USART_REC_LEN  			200  				//定义最大接收字节数 200
#define EN_USART1_RX 			  1						//使能（1）/禁止（0）串口1接收
#define USART_DEBUG		USART1	  	

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         				//接收状态标记	
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


