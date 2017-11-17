#ifndef __BASE_NETWORKHEADER_H__
#define __BASE_NETWORKHEADER_H__

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#ifdef _MMO_SERVER_
#	include <winsock2.h>
#endif
#	include <winsock.h>
//#	include <ws2tcpip.h>
#	include <time.h>
#	pragma comment (lib, "Ws2_32.lib")
#else
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/ioctl.h>
#	include <sys/timeb.h>
#	include <sys/fcntl.h>
#	include <sys/errno.h>
#endif

#endif // __BASE_NETWORKHEADER_H__
