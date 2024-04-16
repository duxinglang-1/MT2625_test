/*
*
* 版权(c) 2015 红石阳光（北京）科技有限公司
* 版权(c) 2011-2015 红石阳光（北京）科技有限公司版权所有
* 
* 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
* 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
* 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
*
*/


#include "stdio.h"
//#include "at.h"
//#include "sockets.h"

#include "rs_socket.h"
#include "rs_debug.h"
#include "rs_error.h"
#include "rs_std_fun.h"
#include "rs_thread.h"
#include "rs_system.h"


// 平台的头文件
#include "apb_proxy.h"
#include "FreeRTOS.h"
#include "memory_map.h"
#include "hal_flash.h"
#include "string.h"
#include "hal_wdt.h"
#include "timers.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_app_api.h"
#include "stdlib.h"
#include "stdio.h"
#include "mbedtls/md5.h"
#include "httpclient.h"
#include "lwip/sockets.h"
#include "stdio.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/err.h"


struct addrinfo g_hints = {0};
struct addrinfo g_currentIP = {0};



void *g_rs_handle = RS_NULL;
RS_SOCKET_MSG_HANDLER_CALLBACK g_socket_callback = RS_NULL;


rs_s32 rs_socket_extract_ip_addr(rs_s8* szDstBuf, const rs_s8* szSrcURL, rs_u32 *nHostPort )
{
	int ret = 0;
	char portBuf[10];
	char* portPtr;

	// check "http" 
	if ( rs_memcmp(szSrcURL, "http://", 7) == 0 )
	{
		szSrcURL += 7;
	}
	else if ( rs_memcmp(szSrcURL, "https://", 8) == 0 )
	{
		szSrcURL += 8;
		ret = 1;
	}

	// copy data 
	while ( *szSrcURL != 0 && *szSrcURL != ':' && *szSrcURL != '/' )
	{
		*szDstBuf++ = *szSrcURL++;
	}
	*szDstBuf = 0;

	if (*szSrcURL == ':')
	{
		long port = 0;
		portPtr = &portBuf[0];
		szSrcURL++;
		*portPtr++ = *szSrcURL++;
		while ( *szSrcURL != 0 && *szSrcURL != '/' )
		{
			*portPtr++ = *szSrcURL++;
		}
		*portPtr = 0;
		port = rs_atoi(portBuf);
		*nHostPort = port;
	}
	else
	{
		*nHostPort = 80;
	}

	return ret;
}


/*
1 表示已经激活可以使用
0 表示开始激活
-1 表示激活失败
*/
rs_s32 rs_socket_act_pdp()
{
	rs_bool state = RS_TRUE;
	
	state = rs_system_ppp_active();
	if (state)
	{
		return 1;
	}
	else
	{
//		rs_post_message_to_thread_with_code(UA_TASK_CHECK_NETWORK_SIGNAL_CODE,	0);
		return 0;
	}
}



rs_s32 rs_socket_gethostbyname(const rs_s8 *uri)
{
	rs_s8  szHostAddr[128];
	rs_s8  ip_buf[256];
    struct addrinfo *addr_list, *cur;
	rs_u32 nPort = 0;
    char szPort[10] = {0};
	int i;

	memset( &g_hints, 0, sizeof( g_hints ) );
    g_hints.ai_family = AF_UNSPEC;
    g_hints.ai_socktype = SOCK_STREAM;
    g_hints.ai_protocol = IPPROTO_TCP;

	rs_strcpy(ip_buf, uri);
	rs_socket_extract_ip_addr(szHostAddr, ip_buf, &nPort);
	snprintf(szPort, sizeof(szPort), "%d", nPort) ;	
	
	RS_PORITNG_LOG("enter rs_socket_gethostbyname  host: %s\n", szHostAddr);

	if ( getaddrinfo(szHostAddr, szPort , &g_hints, &addr_list ) != 0 ) 
	{
        RS_PORITNG_LOG("getaddrinfo != 0, return HTTPCLIENT_UNRESOLVED_DNS\n");
        return RS_ERR_FAILED;
    }
	
    for ( cur = addr_list; cur != NULL; cur = cur->ai_next )
	{
        rs_memcpy(&g_currentIP, cur, sizeof(struct addrinfo));

		freeaddrinfo( addr_list );
		
		RS_PORITNG_LOG("g_currentIP  rs_socket_gethostbyname  host: %s\n", szHostAddr);
		//for(i =0 ;i < 4 ;i++){
		//	
		//RS_PORITNG_LOG("g_currentIP = %d \n", g_currentIP.ai_addr[i]);
		//}
		return RS_ERR_OK;
    }

    freeaddrinfo( addr_list );

	return RS_ERR_FAILED;
}


rs_s32 rs_socket_init(RS_SOCKET_MSG_HANDLER_CALLBACK callback, void* handle)
{
	g_rs_handle = handle;
	g_socket_callback = callback;
	
	return RS_ERR_OK;
}

rs_s32 rs_socket_getMode()
{
	return 0;
}

rs_socket_handle rs_socket_create()
{
	int  hSocket;
	hSocket = socket(g_currentIP.ai_family, g_currentIP.ai_socktype,
                                        g_currentIP.ai_protocol);
	if ( hSocket < 0 )
	{
		return RS_SOC_INVALID_SOCKET_H;
	}

	RS_PORITNG_LOG("socket create, socket handle = %d\n", hSocket);

	return hSocket;
}

rs_s32 rs_socket_connect(rs_socket_handle  socketHandle)
{
	if ( -1 == connect(socketHandle, g_currentIP.ai_addr, (int)g_currentIP.ai_addrlen))
	{
		//int nError = getlasterr();
		 close(socketHandle);
		// 发送失败的消息
		RS_PORITNG_LOG("socket connect fail\n");

		return -1;
	}
	else
	{

		RS_PORITNG_LOG("connect sucess\n");
		return 1;
	}
}

rs_s32 rs_socket_send(rs_socket_handle sockHndl, const rs_s8 *sendData, rs_s32 size)
{
	int len = 0;

	len = send(sockHndl, sendData, size, 0);
	if (len == size)
	{
		RS_PORITNG_LOG("send sucess, sendData size = %d\n", size);
		RS_PORITNG_LOG("send sucess, sendData data=%s\n", sendData);
	}
	else if(len == -1)
	{
		RS_PORITNG_LOG("send fail\n");
	}
	else
	{
		RS_PORITNG_LOG("send pending\n");
	}

	return len;
}

rs_s32 rs_socket_recv(rs_socket_handle sockHndl, rs_s8 *buffer, rs_s32 bufLen)
{
	rs_s32 current = recv(sockHndl, buffer, bufLen, 0);

	RS_PORITNG_LOG("socket recv data, realy read, read len=%d\n", current);
	
	//RS_PORITNG_LOG("recv sucess, recvData data=%s\n", buffer);

	return current;
}

rs_s32 rs_socket_select(rs_socket_handle sockHndl, rs_s32 nTimeOut)
{
	struct timeval timeout;
	fd_set fd_read, fd_exc;
	int ret;

	timeout.tv_sec  = nTimeOut/1000;
   	timeout.tv_usec = 0;//nTimeOut * 1000;

   	FD_ZERO(&fd_read);
	FD_ZERO(&fd_exc);
   	FD_SET(sockHndl, &fd_read);

	ret = select(sockHndl+1, &fd_read, 0, &fd_exc, &timeout);
	if(0 == ret)
	{
		//select超时
		RS_PORITNG_LOG("socket select timeout\n");
	}
	else if(-1 == ret)
	{
		//select出错
		RS_PORITNG_LOG("socket select error\n");
	}
	else
	{
		// 检查socket异常事件是否发生
		if(FD_ISSET(sockHndl, &fd_exc))
		{
			//异常事件
			ret = -1;
		}
		// 检查socket读事件是否发生
		if (FD_ISSET(sockHndl, &fd_read) )
		{

		}
		RS_PORITNG_LOG("socket select OK.\n");
	}

	return ret;
}

rs_s32 rs_socket_close(rs_socket_handle sockHndl)
{
	RS_PORITNG_LOG("clsoe socket\n");
	close(sockHndl);
	return RS_ERR_OK;
}

// before next select wait moment
void rs_socket_wait_moment()
{
	//usleep(20*1000);
}

void fota_pdp_event_handle(rs_s32 errcode)
{
	RS_PORITNG_LOG(RS_LOG_DEBUG"pdp active errcode = %d\n\r", errcode);
	
	if(errcode == 0)
	{
		if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
		{
			g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 0, 0);
		}
	} 
	else 
	{
		if (g_socket_callback != RS_NULL && g_rs_handle != RS_NULL)
		{
			g_socket_callback(g_rs_handle, SOCKET_MSG_COME, 1, 0);
		}
	}	
}


