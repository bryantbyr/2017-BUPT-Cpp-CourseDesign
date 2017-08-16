#ifndef _SOCKET_H
#define _SOCKET_H
#include <Winsock2.h>
#include <iostream>

#if DUBUG  
	#define AF_INET         2               /* internetwork: UDP, TCP, etc. */
	#define SOCK_STREAM     1               /* stream socket */
	#define SOCK_DGRAM      2               /* datagram socket */
	#define SOCK_RAW        3               /* raw-protocol interface */
	#define SOCK_RDM        4               /* reliably-delivered message */
	#define SOCK_SEQPACKET  5               /* sequenced packet stream */
	/*
	* Socket address, internet style.
	*/
	struct sockaddr_in {
			short   sin_family;
			u_short sin_port;
			struct  in_addr sin_addr;
			char    sin_zero[8];
	};
	typedef struct sockaddr_in SOCKADDR_IN; /* Microsoft Windows Extended data types */
	int bind(
		SOCKET s,
		const struct sockaddr FAR * name,
		int namelen
		);									/* INCL_WINSOCK_API_PROTOTYPES*/
	int listen(
		SOCKET s,
		int backlog
		);
#endif 
int SocketInit();
#define MaxSize 1024

#endif