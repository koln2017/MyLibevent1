#include "TcpLibBase.h"
#include "TcpLibServer.h"
#include "TcpLibClient.h"
#include "BufferEventSer.h"
#include "BufferEventClient.h"

CTcpLibBase *NewTcpLib(int nType, CBFUN_PARAM_T param)
{
	switch (nType)
	{
	case 0:
		{
			//服务端_bufferevent
			return new CBufferEventSer(param);
		}
		break;
	case 1:
		{
			//客户端_bufferevent
			return new CBufferEventClient(param);
		}
		break;
	case 2:
		{
			//服务端_event
			return new CTcpLibServer(param);
		}
		break;
	case 3:
		{
			//客户端_event
			return new CTcpLibClient(param);
		}
		break;
	default:
		break;
	}
	return NULL;
}

int CTcpLibBase::GetSocketID()
{
	return -1;
}
