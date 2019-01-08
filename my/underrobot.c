#include "ComSocketSer.h"
#include "ComSerial.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>

extern unsigned char SocketSendBuffer[5];
extern unsigned char SocketRecebuffer[5];
extern unsigned char SerialSendBuf[5];
extern unsigned char SerialReceBuf[5];

 int main()
 {
	 // socket的文件描述符
	int serverfd,clientfd;
	  // serial的文件描述符
	int serial_fd; 
	
	int i;
	int con_flag;
		
	// open the serial ttyAMA0 fd	
	if( (serial_fd = serialOpen("/dev/ttyAMA0",115200)) == -1 )
	{
		fprintf(stderr,"Unable to open serial device: %s\n",strerror(errno) );
		return -1;
	}
	
	// serial test
    //serialPuts(serial_fd, "uart send test ,just by launcher\r\n");
	printf("serial set successfully\r\n");
	
	 // 创建客户端服务器的地址
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	 
	 // 建立套接字
	if ( (serverfd = socket(AF_INET,SOCK_STREAM,0)) == -1 )
	{
		perror("socket create failed!\r\n");
		return -1;
	}
	
	// 绑定端口地址
	serveraddr.sin_family      = AF_INET;
	serveraddr.sin_port        = htons(SERVPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(serveraddr.sin_zero),8);
	if (bind(serverfd, (struct sockaddr*)&serveraddr , sizeof(struct sockaddr)) == -1)
	{
		perror("bind error!\r\n");
		return -1;
	}
	
	// 监听端口
	if ( listen(serverfd,BACKLOG) == -1)
	{
		perror("listen error\r\n");
		return -1;
	}
						  
	while(1)
	{
		// 等待客户端连接进来
		int clientaddrsize = sizeof(struct sockaddr_in);

		clientfd = accept(   serverfd, 
                             (struct sockaddr*)&clientaddr,
                             (socklen_t*)&clientaddrsize
                          );			
		con_flag = 1;					  
		printf("socket set successfully\r\n");
		
		// Serial Recevice Data
		pthread_create(&serial_rec_pid,
					NULL,
					Serial_Recevice_pthread,
					&serial_fd);
					

		// Rocket Recevice Data
		pthread_create(&socket_rec_pid,
				    NULL,
				    Socket_Recevice_pthread,
				    &clientfd);
		
		
		while(con_flag)
		{
			// socket send data
			if(socket_recevice_flag == 1)
			{
				//memcpy(&SerialSendBuf,&SocketRecebuffer,sizeof(SocketRecebuffer));
				for(i=0;i<5;i++)
					SerialSendBuf[i] = SocketRecebuffer[i];
				Serial_Send_Data(serial_fd);
				socket_recevice_flag = 0;
			}
			
			// serial send data
			if(serial_recevice_flag == 1)
			{
				//memcpy(&SocketSendBuffer,&SerialReceBuf,sizeof(SerialReceBuf));
				for(i=0;i<5;i++)
					SocketSendBuffer[i] = SerialReceBuf[i];
				Socket_Send_Data(clientfd);
				serial_recevice_flag = 0;
			}
			
		}// while(con_flag)
		close(clientfd);
		


	}//while

	close(serial_fd);

	
	return 0;
 }// main
 
