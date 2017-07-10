#include "BufferEventSer.h"

void OnAccept(evconnlistener *listener, evutil_socket_t fdClient, sockaddr *pAddr, int nSocklen, void *pParam);
void OnRead(bufferevent *pBufEvent, void *pParam);
void OnWrite(bufferevent *pBufEvent, void *pParam);
void OnEvent(bufferevent *pBufEvent, short nEventType, void *pParam);

static CBFUN_PARAM_T g_param;

CBufferEventSer::CBufferEventSer(CBFUN_PARAM_T param)
	: m_pEvlistener(NULL),
	m_pBase(NULL)
{
	g_param = param;
}

CBufferEventSer::~CBufferEventSer(void)
{
	evconnlistener_free(m_pEvlistener);
	event_base_free(m_pBase);
}

int CBufferEventSer::Init(char *pIP, int nPort)
{
	WSADATA wsaData;
	DWORD dwRet;
	if ((dwRet =WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		return WSAStartup_fail;
	}

	sockaddr_in addr;
	addr.sin_addr.s_addr = 0;
	addr.sin_port = htons(nPort);
	addr.sin_family = PF_INET; //=AF_INET
	
	m_pBase = event_base_new();
	if (NULL == m_pBase)
	{
		return event_base_new_fail;
	}

	m_pEvlistener = evconnlistener_new_bind(m_pBase, OnAccept, m_pBase, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, MAX_LISTEN_SOCKET_NUM, (sockaddr *)&addr, sizeof(addr));
	if (NULL == m_pEvlistener)
	{
		event_base_free(m_pBase);
		return evconnlistener_new_bind_fail;
	}

	return success;
}

static unsigned __stdcall EventLoopThread(void *pParam)
{
	event_base *pBase = (event_base *)pParam;
	if (NULL != pBase)
	{
		int nRet = event_base_dispatch(pBase);
	}
	return 0;
}

void CBufferEventSer::Start()
{
	unsigned threadID;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &EventLoopThread, m_pBase, 0, &threadID);
	CloseHandle(hThread);
}

void CBufferEventSer::Stop()
{
	event_base_loopexit(m_pBase, NULL);
}

void CBufferEventSer::Send(const unsigned char*pBuf, unsigned int nLen)
{
	//bufferevent_write(m_pBev, pBuf, nLen);
}

void OnAccept(evconnlistener *listener, evutil_socket_t fdClient, sockaddr *pAddr, int nSocklen, void *pParam)
{
	if (NULL == pParam || NULL == pAddr)
	{
		return;
	}

	event_base *pBase = (event_base*)pParam;
	bufferevent *pBufEvent = bufferevent_socket_new(pBase, fdClient, BEV_OPT_CLOSE_ON_FREE);
	if (NULL == pBufEvent)
	{
		return;
	}
	bufferevent_setcb(pBufEvent, OnRead, OnWrite, OnEvent, NULL);
	bufferevent_enable(pBufEvent, EV_READ | EV_WRITE | EV_PERSIST);
}

void OnRead(bufferevent *pBufEvent, void *pParam)
{
	char szMsg[MAX_READ_MSG_LEN] = {0};

	int nLen = bufferevent_read(pBufEvent, szMsg, MAX_READ_MSG_LEN);
	if (nLen > 0)
	{
		g_param.cbFun(g_param.pThis, szMsg);
		bufferevent_write(pBufEvent, szMsg, strlen(szMsg)+1);
	}
}

void OnWrite(bufferevent *pBufEvent, void *pParam)
{

}

void OnEvent(bufferevent *pBufEvent, short nEventType, void *pParam)
{
	if (nEventType & BEV_EVENT_EOF)
	{
		//OnEvent connect closed
		bufferevent_free(pBufEvent);
	}
	else if (nEventType & BEV_EVENT_CONNECTED)
	{
		//OnEvent connect to server success
	}
	else if (nEventType & BEV_EVENT_ERROR)
	{
		//OnEvent meet some other error
	}
	else
	{

	}
}