#ifndef _TCPLIBCLIENT_H_
#define _TCPLIBCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <iostream>
using namespace std;

#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/thread.h"
#include "event2/util.h"

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
//#include <signal.h>
#include <io.h>

#include "TcpLibBase.h"

class CTcpLibClient : public CTcpLibBase
{
public:
	CTcpLibClient(CBFUN_PARAM_T param);
	~CTcpLibClient(void);

	virtual int Init(char *pIP, int nPort);
	virtual void Start();
	virtual void Stop();
	virtual void Send(const unsigned char*pBuf, unsigned int nLen);

public:
	evutil_socket_t m_sockfd;
	event_base *m_pBase;
	event *m_pEvent;
	bufferevent *m_pBev;

};

#endif
