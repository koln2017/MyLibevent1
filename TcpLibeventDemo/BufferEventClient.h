#ifndef _BUFFEREVENTCLIENT_H_
#define _BUFFEREVENTCLIENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <iostream>
#include <vector>
#include <process.h> 
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

class CBufferEventClient : public CTcpLibBase
{
public:
	CBufferEventClient(CBFUN_PARAM_T param);
	~CBufferEventClient(void);
public:
	virtual int Init(char *pIP, int nPort);
	virtual void Start();
	virtual void Stop();
	virtual void Send(const unsigned char*pBuf, unsigned int nLen);
	void SetTcpNoDelay(evutil_socket_t fd);


private:
	bufferevent *m_pBufEvent;
	event_base *m_pBase;
};

#endif