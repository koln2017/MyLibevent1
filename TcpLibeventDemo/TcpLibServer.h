#ifndef _TCPLIBSERVER_H_
#define _TCPLIBSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <iostream>
#include <vector>
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

#include "TcpLibBase.h"

class CTcpLibServer : public CTcpLibBase
{
public:
	CTcpLibServer(CBFUN_PARAM_T param);
	~CTcpLibServer(void);

public:
	virtual int Init(char *pIP, int nPort);
	virtual void Start();
	virtual void Stop();
	virtual void Send(const unsigned char*pBuf, unsigned int nLen);

	void SetTcpNoDelay(evutil_socket_t fd);


public:
	event_base *m_pBase;
	event *m_pEvent;
	unsigned m_threadID;
	evutil_socket_t m_listenfd;
};

#endif
