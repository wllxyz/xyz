#ifndef WLL_TCP_SOCKET_H
#define WLL_TCP_SOCKET_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "WllString.h"
#include "WllTrace.h"
using namespace Wll::Util;

#ifndef offsetof
#define offsetof(type,member) (int)(&((type*)0)->member)
#endif //offsetof

namespace Wll
{
 namespace System
 {
 static const int MAX_LISTEN_QUEUE_LEN=128;

 class TcpServer
 {
 public:
  TcpServer()
  {
   this->listen_fd = -1;
  }
  ~TcpServer()
  {
   this->Close();
  }
  int Open(const char* name)
  {
   char host[20];
   short int port = 80;
   const char* p = split(name,host,sizeof(host),':');
   if(p!=NULL)
   {
	int n;
	String2Int(p,n);
	port = n;
   }
   struct sockaddr_in address;
   memset(&address,0,sizeof(address));
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(host);
   address.sin_port = htons(port);
   this->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (this->listen_fd < 0)
   {
    return -1;
   }
   if (bind(this->listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
   {
    close(this->listen_fd);
    this->listen_fd = -1;
    return -2;
   }
   if (listen(this->listen_fd,MAX_LISTEN_QUEUE_LEN) < 0)
   {
    close(this->listen_fd);
    this->listen_fd = -1;
    return -3;
   }
   return this->listen_fd;
  }
  void Close()
  {
   if(this->listen_fd > 0)
   {
    close(this->listen_fd);
    this->listen_fd = -1;
   }
  }
  int Accept()
  {
   return accept(this->listen_fd,NULL,NULL);
  }
 private:
  int listen_fd;
 };

 class TcpClient
 {
 public:
  TcpClient()
  {
   this->connect_fp = -1;
  }
  ~TcpClient()
  {
   this->Close();
  }
  int Open(const char* name)
  {
   this->connect_fp = socket(AF_INET, SOCK_STREAM, 0);
   if( this->connect_fp<0 ) return -1;
   
   char host[20];
   short int port = 80;
   const char* p = split(name,host,sizeof(host),':');
   if(p!=NULL)
   {
	int n;
	String2Int(p,n);
	port = n;
   }
   DEBUG_LOG("host="<<host<<",port="<<port);

   struct sockaddr_in address;
   memset(&address,0,sizeof(address));
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(host);
   address.sin_port = htons(port);
   if(connect(this->connect_fp,(struct sockaddr*)&address,sizeof(address))<0)
   {
    close(this->connect_fp);
    this->connect_fp = -1;
    return -1;
   }
   return this->connect_fp;
  }
  void Close()
  {
   if(this->connect_fp>0)
   {
    close(this->connect_fp);
    this->connect_fp = -1;
   }
  }
 private:
  int connect_fp;
 };
 };//end of namespace System
};//end of namespace Wll

#endif //WLL_TCP_SOCKET_H

