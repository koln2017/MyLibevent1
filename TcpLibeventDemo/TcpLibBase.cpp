#include "TcpLibBase.h"
#include "TcpLibServer.h"
#include "TcpLibClient.h"

CTcpLibBase *NewTcpLib(int nType, CBFUN_PARAM_T param)	//0-����� 1-�ͻ���
{
	if (0 == nType)
	{
		return new CTcpLibServer(param);
	}
	else
	{
		return new CTcpLibClient(param);
	}
}
