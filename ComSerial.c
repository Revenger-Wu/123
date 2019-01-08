#include "ComSerial.h"
//
unsigned char SerialSendBuf[5];
unsigned char SerialReceBuf[5];


void* Serial_Recevice_pthread(void*arg)
{
	int i;
	// sizeof(SerialReceBuf)
	while(1)
	{
		if( read( *(int*)arg,SerialReceBuf,sizeof(SerialReceBuf) ) == -1 )
		{	
			perror("Serial recevice the data error!\r\n");
			//return -1;
		}
		else 
		{	
			printf("Serial recevice the data successful!\r\n");
			if( (SerialReceBuf[0] == 0xA1) && (SerialReceBuf[4] == 0x1A) )
			{
				serial_recevice_flag = 1;
				printf("Serial the data is correct!\r\n");
				for(i=0;i<5;i++)
				{
					printf(" serialReceBuf[%d] is %x\r\n",i,SerialReceBuf[i]);
				}
			}
		}
	}
}

int Serial_Send_Data(int serial_fd)
{
	if( write(serial_fd,&SerialSendBuf,sizeof(SerialSendBuf)) == -1 )
	{	
		perror("Serial send data error!\r\n");
		return -1;
	}
	else 
	{
		printf(" Serial send the data successfully\r\n");
	}
}












