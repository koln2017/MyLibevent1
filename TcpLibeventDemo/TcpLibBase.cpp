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
			//�����_bufferevent
			return new CBufferEventSer(param);
		}
		break;
	case 1:
		{
			//�����_event
			return new CTcpLibServer(param);
		}
		break;
	case 2:
		{
			//�ͻ���_bufferevent
			return new CBufferEventClient(param);
		}
		break;
	case 3:
		{
			//�ͻ���_event
			return new CTcpLibClient(param);
		}
		break;
	default:
		break;
	}
}
