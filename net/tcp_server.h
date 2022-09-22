#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "tcp_client.h"

typedef void  TcpServer;
typedef void (*Listener)(TcpClient*, int);

enum
{
    EVT_CONN,    //连接
    EVT_DATA,    //数据
    EVT_CLOSE    //关闭
};

TcpServer* TcpServer_New();
int TcpServer_Start(TcpServer* server, int port, int max);
void TcpServer_Stop(TcpServer* server);
void TcpServer_SetListener(TcpServer* server, Listener listener);
int TcpServer_IsValid(TcpServer* server);
void TcpServer_DoWork(TcpServer* server);
void TcpServer_Del(TcpServer* server);

#endif
