#include <cis_sock.h>
#include <cis_if_net.h>
#include <cis_list.h>
#include <cis_log.h>
#include <cis_if_sys.h>
#include <cis_internals.h>
#include "FreeRTOS.h"
#include "task.h"
#include "task_def.h"
#include "semphr.h"
#define MAX_PACKET_SIZE			(1024)


static void *g_lockPacketlist;
static int prvCreateSocket(uint16_t localPort,int ai_family);

void callbackRecvThread(void* arg);
void callbackDMThread(void* arg);


struct st_net_packet
{
    struct st_net_packet * next;
    uint8_t* buffer;
    uint32_t length;
};


struct st_cisnet_context
{
    int sock;
    char host[128];
    uint16_t port;
    int state;
    void* context;
    int8_t	quit;
	struct st_net_packet* g_packetlist;
};


bool  cisnet_attached_state(void * ctx)
{
    return ((struct st_cis_context *)(ctx))->netAttached;
}

cis_ret_t cisnet_init(void *context,const cisnet_config_t* config,cisnet_callback_t cb)
{

    net_Init();
    LOGI("fall in cisnet_init\r\n");
    memcpy(&((struct st_cis_context *)context)->netConfig,config,sizeof(cisnet_config_t));
    ((struct st_cis_context *)context)->netCallback.onEvent = cb.onEvent;
	 cissys_sleepms(1000);
    ((struct st_cis_context *)context)->netAttached = 1;
    if (g_lockPacketlist == NULL) cissys_lockcreate(&g_lockPacketlist);
    return CIS_RET_OK;
}

cis_ret_t cisnet_create(cisnet_t* netctx,const char* host,void* context)
{
     if(((struct st_cis_context *)context)->netAttached != true)return CIS_RET_ERROR;

    (*netctx) = (cisnet_t)malloc(sizeof(struct st_cisnet_context));
    (*netctx)->sock = 0;
    (*netctx)->port = 5683;
    (*netctx)->state = 0;
    (*netctx)->quit = 0;
	(*netctx)->g_packetlist=NULL;
    (*netctx)->context = context;
    strncpy((*netctx)->host,host,sizeof((*netctx)->host));

    return CIS_RET_OK;
}

void     cisnet_destroy(cisnet_t netctx)
{
    LOGD("cisnet_destroy");
    netctx->quit = 1;
    if (((struct st_cis_context *)(netctx->context))->recv_taskhandle != NULL) {
        cissys_lock(((struct st_cis_context *)(netctx->context))->lockSocket,CIS_CONFIG_LOCK_INFINITY);
        cissys_lock(((struct st_cis_context *)(netctx->context))->lockSocket,CIS_CONFIG_LOCK_INFINITY);
        cissys_unlock(((struct st_cis_context *)(netctx->context))->lockSocket);
    }
    net_Close(netctx->sock);
    cissys_lock(g_lockPacketlist, CIS_CONFIG_LOCK_INFINITY);
    while (netctx->g_packetlist != NULL){
        struct st_net_packet* delNode;
         delNode =netctx->g_packetlist;
         netctx->g_packetlist = netctx->g_packetlist->next;
         cis_free(delNode->buffer);
         cis_free(delNode);
    }
    cissys_unlock(g_lockPacketlist);
    free(netctx);
}


cis_ret_t cisnet_connect(cisnet_t netctx)
{
    int sock = -1;
    sock = prvCreateSocket(0,AF_INET);
    if (sock < 0)
    {
        LOGD("Failed to open socket: %d %s\r\n", errno, strerror(errno));
        return CIS_RET_ERROR;
    }
    netctx->sock = sock;
    netctx->state = 1;
   ((struct st_cis_context *)(netctx->context))->netCallback.onEvent(netctx,cisnet_event_connected,NULL,netctx->context);

    cissys_assert(((struct st_cis_context *)(netctx->context))->recv_taskhandle == NULL);
    if (((struct st_cis_context *)(netctx->context))->isDM) {
        xTaskCreate(callbackDMThread,
                "dm_recv",
                3072 / sizeof(portSTACK_TYPE),
                netctx,
                TASK_PRIORITY_NORMAL,
                &((struct st_cis_context *)(netctx->context))->recv_taskhandle);
    } else {
        xTaskCreate(callbackRecvThread,
                "cis_recv",
                3072 / sizeof(portSTACK_TYPE),
                netctx,
                TASK_PRIORITY_NORMAL,
                &((struct st_cis_context *)(netctx->context))->recv_taskhandle);
    }

    return CIS_RET_OK;
}

cis_ret_t cisnet_disconnect(cisnet_t netctx)
{
    netctx->state = 0;
   ((struct st_cis_context *)(netctx->context))->netCallback.onEvent(netctx,cisnet_event_disconnect,NULL,netctx->context);
    return 1;
}

cis_ret_t cisnet_write(cisnet_t netctx,const uint8_t * buffer,uint32_t  length)
{
    int nbSent;
    size_t addrlen;
    size_t offset;
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET; 
    saddr.sin_port = htons(netctx->port);
    saddr.sin_addr.s_addr = inet_addr(netctx->host);
    
    addrlen = sizeof(saddr);
    offset = 0;
    while (offset != length)
    {
        nbSent = sendto(netctx->sock, (const char*)buffer + offset, length - offset, 0, (struct sockaddr *)&saddr, addrlen);

        if (nbSent == -1){
            LOGE("socket sendto [%s:%d] failed.\r\n",netctx->host,ntohs(saddr.sin_port));
            return -1;
        }else{
            LOGI("socket sendto [%s:%d] %d bytes\r\n",netctx->host,ntohs(saddr.sin_port),nbSent);
        }
        offset += nbSent;
    }

    return CIS_RET_OK;
}

cis_ret_t cisnet_read(cisnet_t netctx,uint8_t** buffer,uint32_t *length)
{
    cissys_lock(g_lockPacketlist, CIS_CONFIG_LOCK_INFINITY);
    if(netctx->g_packetlist != NULL){
        struct st_net_packet* delNode;
        *buffer = netctx->g_packetlist->buffer;
        *length = netctx->g_packetlist->length;
         delNode =netctx->g_packetlist;
         netctx->g_packetlist = netctx->g_packetlist->next;
         cis_free(delNode);
         cissys_unlock(g_lockPacketlist);
         return CIS_RET_OK;
    }

    cissys_unlock(g_lockPacketlist);
    return CIS_RET_ERROR;
}

cis_ret_t cisnet_free(cisnet_t netctx,uint8_t* buffer,uint32_t length)
{
    cis_free(buffer);
    return CIS_RET_ERROR;
}



//////////////////////////////////////////////////////////////////////////

static int prvCreateSocket(uint16_t localPort,int ai_family)
{
    int sock = net_Socket(ai_family,SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0){
        return -1;
    }
    return sock;
}



void callbackRecvThread(void* arg)
{
    
    cisnet_t netctx = (cisnet_t)arg;
	int sock = netctx->sock;
	while(0 == netctx->quit && netctx->state == 1)
	{
		struct timeval tv = {5,0};
		fd_set readfds;
		int result;

		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		/*
			* This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
			* with the precedent function)
			*/
		result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);

		if (result < 0)
		{
			LOGE("Error in select(): %d %s\r\n", errno, strerror(errno));
		}
		else if (result > 0)
		{
			uint8_t buffer[MAX_PACKET_SIZE];
			int numBytes;

			/*
				* If an event happens on the socket
				*/
			if (FD_ISSET(sock, &readfds))
			{
				struct sockaddr_storage addr;
				socklen_t addrLen;

				addrLen = sizeof(addr);

				/*
					* We retrieve the data received
					*/
				numBytes = recvfrom(sock, (char*)buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

				if (numBytes < 0)
				{
					if(errno == 0)continue;
					LOGE("Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
				}
				else if (numBytes > 0)
				{
					char s[INET_ADDRSTRLEN];
					uint16_t  port;
			
					struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
					inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET_ADDRSTRLEN);
					port = saddr->sin_port;

					LOGI("%d bytes received from [%s]:%hu\r\n", numBytes, s, ntohs(port));
					
                    uint8_t* data = (uint8_t*)cissys_malloc(numBytes);
                    memcpy(data,buffer,numBytes);

                    struct st_net_packet *packet = (struct st_net_packet*)cissys_malloc(sizeof(struct st_net_packet));
                    packet->next = NULL;
                    packet->buffer = data;
                    packet->length = numBytes;
                    //packet->netctx = netctx;
                    cissys_lock(g_lockPacketlist, CIS_CONFIG_LOCK_INFINITY);
                    netctx->g_packetlist = (struct st_net_packet*)CIS_LIST_ADD(netctx->g_packetlist,packet);
                    cissys_unlock(g_lockPacketlist);
				}
			}
		}
	}
	LOGE("Error in socket recv thread exit..\n");
    ((struct st_cis_context *)(netctx->context))->recv_taskhandle = NULL;
    cissys_unlock(((struct st_cis_context *)(netctx->context))->lockSocket);
    vTaskDelete(NULL);
}

void callbackDMThread(void* arg)
{
    
    cisnet_t netctx = (cisnet_t)arg;
	int sock = netctx->sock;
	while(0 == netctx->quit && netctx->state == 1)
	{
		struct timeval tv = {5,0};
		fd_set readfds;
		int result;

		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		/*
			* This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
			* with the precedent function)
			*/
		result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);

		if (result < 0)
		{
			LOGE("Error in select(): %d %s\r\n", errno, strerror(errno));
		}
		else if (result > 0)
		{
			uint8_t buffer[MAX_PACKET_SIZE];
			int numBytes;

			/*
				* If an event happens on the socket
				*/
			if (FD_ISSET(sock, &readfds))
			{
				struct sockaddr_storage addr;
				socklen_t addrLen;

				addrLen = sizeof(addr);

				/*
					* We retrieve the data received
					*/
				numBytes = recvfrom(sock, (char*)buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

				if (numBytes < 0)
				{
					if(errno == 0)continue;
					LOGE("Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
				}
				else if (numBytes > 0)
				{
					char s[INET_ADDRSTRLEN];
					uint16_t  port;
			
					struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
					inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET_ADDRSTRLEN);
					port = saddr->sin_port;

					LOGI("%d bytes received from [%s]:%hu\r\n", numBytes, s, ntohs(port));
					
                    uint8_t* data = (uint8_t*)cissys_malloc(numBytes);
                    memcpy(data,buffer,numBytes);

                    struct st_net_packet *packet = (struct st_net_packet*)cissys_malloc(sizeof(struct st_net_packet));
                    packet->next = NULL;
                    packet->buffer = data;
                    packet->length = numBytes;
                    //packet->netctx = netctx;
                    cissys_lock(g_lockPacketlist, CIS_CONFIG_LOCK_INFINITY);
                    netctx->g_packetlist = (struct st_net_packet*)CIS_LIST_ADD(netctx->g_packetlist,packet);
                    cissys_unlock(g_lockPacketlist);
				}
			}
		}
	}
	LOGE("Error in socket recv thread exit..\n");
    ((struct st_cis_context *)(netctx->context))->recv_taskhandle = NULL;
    cissys_unlock(((struct st_cis_context *)(netctx->context))->lockSocket);
    vTaskDelete(NULL);
}

