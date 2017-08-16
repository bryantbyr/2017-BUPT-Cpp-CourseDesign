#include "Socket.h"

#pragma comment(lib,"WS2_32.LIB")

const char *SeverIp = "10.122.229.132";

HANDLE hMutex;

void Send(SOCKET sockClient)
{
	char sendBuf[MaxSize];

	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		//printf("Please input your choice.\n");
		gets_s(sendBuf);
		int byte = 0;
		byte = send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);;//�������ӿͻ��˽�������
		if (byte <= 0)
			break;

		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(sockClient);//�ر�socket,һ��ͨ�����
}

void Rec(SOCKET sockClient)
{

	char revBuf[MaxSize];

	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);

		int byte = 0;
		byte = recv(sockClient, revBuf, MaxSize, 0);//�������ӿͻ��˽�������
		revBuf[byte] = '\0';
		if (byte <= 0)
			break;

		printf("%s", revBuf);

		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(sockClient);//�ر�socket,һ��ͨ�����
}

int main()
{

	if (SOCKET_ERROR == SocketInit())
		return -1;

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(SeverIp);//�趨��Ҫ���ӵķ�������ip��ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6666);//�趨��Ҫ���ӵķ������Ķ˿ڵ�ַ
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//���������������

	while (1){

		HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Rec, (LPVOID)sockClient, 0, 0);

		if (hThread1 != NULL)
		{
			CloseHandle(hThread1);
		}

		HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Send, (LPVOID)sockClient, 0, 0);

		if (hThread2 != NULL)
		{
			CloseHandle(hThread2);
		}

		//Sleep(1000);

	}


	getchar();

	WSACleanup();
	return -1;
}
