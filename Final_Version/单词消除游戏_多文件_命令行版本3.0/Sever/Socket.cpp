#include "common.h"
#include "Socket.h"

#pragma comment(lib,"WS2_32.LIB")


//********************************************************************************************************/
//** 函数名 ** SocketInit()
//**  输入  ** 无
//**  输出  ** 无
//**函数描述** 加载套接字
//********************************************************************************************************/
int SocketInit()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested=MAKEWORD(2,2);
	err = WSAStartup(wVersionRequested,&wsaData); //返回0，成功，否则就是错误码

	if (err!=0)
	{
		printf("WinSock DLL版本不足要求n");
		return 0;
	}

	if (LOBYTE(wsaData.wVersion)!=2||
		HIBYTE(wsaData.wVersion)!=2)
	{
		WSACleanup();
		return 0;
	}

	return 1;
}
/**
* [sendToClient description]:向客户端发送数据
* @param sockClient [description]：客户端
* @param s          [description]：消息
*/
void sendToClient(SOCKET sockClient, const char s[MaxSize])
{
	char sendBuf[MaxSize];
	strcpy_s(sendBuf, s);
	int byte = send(sockClient, sendBuf, strlen(sendBuf), 0);
	if (byte <= 0)
		cout << "send error\n";
}