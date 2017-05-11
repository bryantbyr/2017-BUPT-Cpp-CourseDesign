
#include "stdafx.h"
#include "Socket.h"

#pragma comment(lib,"WS2_32.LIB")

const char *SeverIp = "10.122.229.132";

HANDLE hMutex;

void Send(SOCKET sockClient)
{
	char sendBuf[MaxSize];

	while(1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		//printf("Please input your choice.\n");
		gets_s(sendBuf);
		int byte = 0;
		byte= send(sockClient,sendBuf,strlen(sendBuf)+1,0);;//服务器从客户端接受数据
		if (byte<=0)
			break;

		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(sockClient);//关闭socket,一次通信完毕
}

void Rec(SOCKET sockClient)
{

	char revBuf[MaxSize];

	while(1)
	{
		WaitForSingleObject(hMutex, INFINITE);

		int byte = 0;
		byte= recv(sockClient,revBuf,MaxSize,0);//服务器从客户端接受数据
		revBuf[byte]='\0';
		if (byte<=0)
			break;

		printf("%s",revBuf);
		//for (auto& c : revBuf) { c = NULL; }

		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(sockClient);//关闭socket,一次通信完毕
}

int main()
{

	if (SOCKET_ERROR ==SocketInit())
		return -1;

	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(SeverIp);//设定需要连接的服务器的ip地址
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6666);//设定需要连接的服务器的端口地址
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//与服务器进行连接

	while(1){

		HANDLE hThread1 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Rec,(LPVOID)sockClient,0,0);

		if(hThread1!=NULL)
		{
			CloseHandle(hThread1);
		}

		HANDLE hThread2 = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Send,(LPVOID)sockClient,0,0);

		if(hThread2!=NULL)
		{
			CloseHandle(hThread2);
		}

		//Sleep(1000);

	}


	getchar();

    WSACleanup();
	return -1;
}
