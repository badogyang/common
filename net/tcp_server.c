#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <malloc.h>
#include "tcp_client.h"
#include "tcp_server.h"

#define FD_SIZE 1024

typedef struct tcp_server
{
    int fd;
    int valid;
    Listener cb;
    TcpClient* client[FD_SIZE];
} Server;

TcpServer* TcpServer_New()
{
    Server* ret = malloc(sizeof(Server));

    if( ret )
    {
        int i = 0;

        ret->fd = -1;
        ret->valid = 0;
        ret->cb = NULL;

        for(i=0; i<FD_SIZE; i++)
        {
            ret->client[i] = NULL;
        }
    }

    return ret;
}

int TcpServer_Start(TcpServer* server, int port, int max)
{
    Server* s = (Server*)server;

    if( s && ! s->valid )
    {
        struct sockaddr_in saddr = {0};

        s->fd = socket(PF_INET, SOCK_STREAM, 0);

        s->valid = (s->fd != -1);

        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons(port);

        s->valid = s->valid && (bind(s->fd, (struct sockaddr*)&saddr, sizeof(saddr)) != -1);
        s->valid - s->valid && (listen(s->fd, max) != -1);
    }

    return s->valid;
}

void TcpServer_Stop(TcpServer* server)
{
    Server* s = (Server*)server;

    if( s )
    {
        int i = 0;

        s->valid = 0;

        close(s->fd);

        for (i = 0; i < FD_SIZE; i++)
        {
            TcpClient_Del(s->client[i]);
            s->client[i] = NULL;
        }
    }
}

void TcpServer_SetListener(TcpServer* server, Listener listener)
{
    Server* s = (Server*)server;

    if( s )
    {
        s->cb = listener;
    }
}

int TcpServer_IsValid(TcpServer* server)
{
    return server ? ((Server*)server)->valid : 0;
}

static int SelectHandler(Server* s, fd_set* rset, fd_set* reads, int num, int max)
{
    int ret = 0;

    return ret;
}

void TcpServer_DoWork(TcpServer* server)
{
    Server* s = (Server*)server;

    if( s && s->valid )
    {
        int max = 0;
        int num = 0;
        fd_set reads = {0};
        fd_set rset = {0};
        struct timeval timeout = {0};

        FD_ZERO(&reads);
        FD_SET(s->fd, &reads);

        max = s->fd;

        while ( s->valid )
        {
            rset = reads;

            timeout.tv_sec = 0;
            timeout.tv_usec = 10000;

            num = select(max+1, &rset, 0, 0, &timeout);

            if( num > 0 )
            {
                max = SelectHandler(s, &rset, &reads, num, max);
            }
        }
        
    }
}

void TcpServer_Del(TcpServer* server)
{
    TcpServer_Stop(server);
    free(server);
}