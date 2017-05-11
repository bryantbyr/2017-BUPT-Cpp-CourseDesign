// SocketSever.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "Socket.h"
#include<windows.h>

HANDLE hMutex;

void ThreadProc1(SOCKET NewsockConn)
{
	char revBuf[MaxSize];
	int byte = 0;

	while(1)
	{
		WaitForSingleObject(hMutex, INFINITE);

		byte= recv(NewsockConn,revBuf,strlen(revBuf)+1,0);//服务器从客户端接受数据
		if (byte<=0)
		{
			break;
		}	

		printf("%s\n",revBuf);
		
		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(NewsockConn);//关闭socket,一次通信完毕
}

//********************************************************************************************************/
//** 函数名 ** main()
//**  输入  ** 无
//**  输出  ** 无
//**函数描述** 主函数
//********************************************************************************************************/
int main()
{

	SOCKADDR_IN addrServer;
	int sockServer; 

	
 	if (SOCKET_ERROR ==SocketInit())
 	{
 		return -1;
 	}
	
	addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);		//htol将主机字节序long型转换为网络字节序
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(6666);						//htos用来将端口转换成字符，1024以上的数字即可
	
	
	sockServer=socket(AF_INET,SOCK_STREAM,0);				//面向连接的可靠性服务SOCK_STRAM
	bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));//将socket绑定到相应地址和端口上
	listen(sockServer,5);									//等待队列中的最大长度为5

	printf("Welcome,the Host %s is running!Now Wating for someone comes in!\n",inet_ntoa(addrServer.sin_addr));

	int iSend = 0;
//	char sendBuf[MaxSize];
	char revBuf[MaxSize];

	int len=sizeof(SOCKADDR);

	SOCKADDR_IN addrClient;


	while(1)			
	{
		
		SOCKET NewsockConn=accept(sockServer,(SOCKADDR*)&addrClient,&len);//阻塞调用进程直至新的连接出现

		if(NewsockConn == INVALID_SOCKET)
		{	
			printf("Accept Failed!\n");
			continue; //继续监听
		}
		HANDLE hThread1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc1,(LPVOID)NewsockConn,0,0);

		if(hThread1!=NULL)
		{
			CloseHandle(hThread1);
		}
		Sleep(1000);	
	}

	getchar();
	return 0;
}
