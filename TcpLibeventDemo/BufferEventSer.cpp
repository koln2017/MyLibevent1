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
	m_vecBufferInfo.clear();
}

CBufferEventSer::~CBufferEventSer(void)
{
	evconnlistener_free(m_pEvlistener);
	event_base_free(m_pBase);
	WSACleanup();
}

int CBufferEventSer::Init()
{
	WSADATA wsaData;
	DWORD dwRet;
	if ((dwRet =WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		return WSAStartup_fail;
	}

	//设置多线程
#ifdef WIN32
	evthread_use_windows_threads();//win上设置
#else
	evthread_use_pthreads();    //unix上设置
#endif

	sockaddr_in addr;
	addr.sin_addr.s_addr = 0;
	addr.sin_port = htons(g_param.nPort);
	addr.sin_family = PF_INET; //=AF_INET
	
	m_pBase = event_base_new();
	if (NULL == m_pBase)
	{
		return event_base_new_fail;
	}
	evthread_make_base_notifiable((event_base *)m_pBase);

	m_pEvlistener = evconnlistener_new_bind(m_pBase, OnAccept, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, MAX_LISTEN_SOCKET_NUM, (sockaddr *)&addr, sizeof(addr));
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

int CBufferEventSer::Send(void *pSendID, const unsigned char*pBuf, unsigned int nLen)
{
	//服务器使用bufferevent_write时，需要使用到bufferevent，所以发送时需要遍历fd，大并发量时需要考虑效率
	 if (NULL == pSendID)
	 {
		 int nSize = m_vecBufferInfo.size();
		 if (nSize > 0)
		 {
			 //没有指定SocketID则给最后连接的Socket客户端发送数据
			return bufferevent_write(m_vecBufferInfo.at(nSize-1).pBufvClient, pBuf, nLen);
			//int nRet = send(m_vecBufferInfo.at(nSize-1).fdClient, (const char*)pBuf, nLen, 0);
			//if (SOCKET_ERROR == nRet)
			//{
			//	return -1;
			//}
		 }
	 }
	 else
	 {
		for (int i = 0; i < m_vecBufferInfo.size(); i++)
		{
			if (m_vecBufferInfo.at(i).pBufvClient == pSendID)
			{
				return bufferevent_write(m_vecBufferInfo.at(i).pBufvClient, pBuf, nLen);
			}
		}
	 }
	 return -1;
}

void OnAccept(evconnlistener *listener, evutil_socket_t fdClient, sockaddr *pAddr, int nSocklen, void *pParam)
{
	if (NULL == pParam || NULL == pAddr)
	{
		return;
	}
	
	CBufferEventSer *pDlg = (CBufferEventSer *)pParam;
	event_base *pBase = evconnlistener_get_base(listener);
	bufferevent *pBufEvent = bufferevent_socket_new(pBase, fdClient, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	if (NULL == pBufEvent)
	{
		return;
	}
	BUFFER_INFO_T bufvInfo;
	bufvInfo.fdClient = fdClient;
	bufvInfo.pBufvClient = pBufEvent;
	pDlg->m_vecBufferInfo.push_back(bufvInfo);
	bufferevent_setcb(pBufEvent, OnRead, NULL, OnEvent, pParam);
	bufferevent_enable(pBufEvent, EV_READ | /*EV_WRITE |*/ EV_PERSIST);
	g_param.cbFun(TCP_SERVER_CONNECT, g_param.pThis, (void*)&fdClient);
}

void OnRead(bufferevent *pBufEvent, void *pParam)
{
	unsigned char szMsg[MAX_READ_MSG_LEN] = {0};

	CBufferEventSer *pDlg = (CBufferEventSer *)pParam;
	int nLen = bufferevent_read(pBufEvent, szMsg, MAX_READ_MSG_LEN);
	if (nLen > 0)
	{
		DATA_PACKAGE_T data;
		data.pSendID = pBufEvent;
		data.nLen = nLen;
		data.pData = szMsg;
		g_param.cbFun(TCP_READ_DATA, g_param.pThis, (void*)&data);
		//bufferevent_write(pBufEvent, szMsg, strlen(szMsg)+1);
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

		CBufferEventSer *pDlg = (CBufferEventSer *)pParam;
		vector<BUFFER_INFO_T>::iterator it = pDlg->m_vecBufferInfo.begin();
		for ( ; it != pDlg->m_vecBufferInfo.end() ; )
		{
			if (it->pBufvClient == pBufEvent)
			{
				g_param.cbFun(TCP_SERVER_DISCONNECT, g_param.pThis, (void*)&it->fdClient);
				it = pDlg->m_vecBufferInfo.erase(it);
				break;
			}
			else
			{
				it++;
			}
		}
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