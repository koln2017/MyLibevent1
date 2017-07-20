#ifndef _BUFFEREVENTCLIENT_H_
#define _BUFFEREVENTCLIENT_H_
#include "Header.h"

class CBufferEventClient : public CTcpLibBase
{
public:
	CBufferEventClient(CBFUN_PARAM_T param);
	~CBufferEventClient(void);
public:
	virtual int Init();
	virtual void Start();
	virtual void Stop();
	virtual int Send(void *pSendID, const unsigned char*pBuf, unsigned int nLen);
	void SetTcpNoDelay(evutil_socket_t fd);
	virtual int GetSocketID();

private:
	event_base *m_pBase;
	bufferevent *m_pBufEvent;
};

#endif