#include "stdafx.h"
#include "Socket.h"

#pragma comment(lib,"WS2_32.LIB")


//********************************************************************************************************/
//** ������ ** SocketInit()
//**  ����  ** ��
//**  ���  ** ��
//**��������** �����׽���
//********************************************************************************************************/
int SocketInit()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested=MAKEWORD(2,2);
	err = WSAStartup(wVersionRequested,&wsaData); //����0���ɹ���������Ǵ�����

	if (err!=0)
	{
		printf("WinSock DLL�汾����Ҫ��n");
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