// SocketSever.cpp : �������̨Ӧ�ó������ڵ㡣
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

		byte= recv(NewsockConn,revBuf,strlen(revBuf)+1,0);//�������ӿͻ��˽�������
		if (byte<=0)
		{
			break;
		}	

		printf("%s\n",revBuf);
		
		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(NewsockConn);//�ر�socket,һ��ͨ�����
}

//********************************************************************************************************/
//** ������ ** main()
//**  ����  ** ��
//**  ���  ** ��
//**��������** ������
//********************************************************************************************************/
int main()
{

	SOCKADDR_IN addrServer;
	int sockServer; 

	
 	if (SOCKET_ERROR ==SocketInit())
 	{
 		return -1;
 	}
	
	addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);		//htol�������ֽ���long��ת��Ϊ�����ֽ���
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(6666);						//htos�������˿�ת�����ַ���1024���ϵ����ּ���
	
	
	sockServer=socket(AF_INET,SOCK_STREAM,0);				//�������ӵĿɿ��Է���SOCK_STRAM
	bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));//��socket�󶨵���Ӧ��ַ�Ͷ˿���
	listen(sockServer,5);									//�ȴ������е���󳤶�Ϊ5

	printf("Welcome,the Host %s is running!Now Wating for someone comes in!\n",inet_ntoa(addrServer.sin_addr));

	int iSend = 0;
//	char sendBuf[MaxSize];
	char revBuf[MaxSize];

	int len=sizeof(SOCKADDR);

	SOCKADDR_IN addrClient;


	while(1)			
	{
		
		SOCKET NewsockConn=accept(sockServer,(SOCKADDR*)&addrClient,&len);//�������ý���ֱ���µ����ӳ���

		if(NewsockConn == INVALID_SOCKET)
		{	
			printf("Accept Failed!\n");
			continue; //��������
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
