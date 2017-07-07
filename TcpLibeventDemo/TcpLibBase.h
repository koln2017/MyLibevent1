#ifndef _TCPLIBBASE_H_
#define _TCPLIBBASE_H_

#define bzero(argc, len)		memset(argc, 0, len)
#define MAX_LINE		1024
#define LISTEN_BACKLOG	32
typedef void (*ReadCbFun)(void *argc, void *argv);

enum STATUS_EN
{
	success = 0,
	WSAStartup_fail,
	socket_create_fail,
	bind_fail,
	listen_fail,
	event_base_new_fail,
	inet_ntop_fail,
	connect_fail,
};

struct CBFUN_PARAM_T
{
	void *pThis;
	ReadCbFun cbFun;
};

class CTcpLibBase
{
public:
	virtual int Init(char *pIP, int nPort) = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Send(const unsigned char*pBuf, unsigned int nLen) = 0;
};

CTcpLibBase *NewTcpLib(int nType, CBFUN_PARAM_T param);	//0-服务端 1-客户端

#endif