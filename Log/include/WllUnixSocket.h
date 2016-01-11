#ifndef WLL_UNIX_SOCKET_H
#define WLL_UNIX_SOCKET_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifndef offsetof
#define offsetof(type,member) long(&(((type*)0)->member))
#endif //offsetof

namespace Wll
{
 namespace System
 {
 static const int MAX_LISTEN_QUEUE_LEN=128;

 class UnixTcpServer
 {
 public:
  UnixTcpServer()
  {
   this->listen_fd = -1;
  }
  ~UnixTcpServer()
  {
   this->Close();
  }
  bool Open(const char* name)
  {
   struct sockaddr_un address;
   memset(&address,0,sizeof(address));
   address.sun_family = AF_UNIX;
   strncpy(address.sun_path, name,sizeof(address.sun_path)-1);
   address.sun_path[sizeof(address.sun_path)-1]=0;
   this->listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
   if (this->listen_fd < 0)
   {
    return false;
   }
   int size = offsetof(struct sockaddr_un, sun_path) + strlen(address.sun_path);
   if (bind(this->listen_fd, (struct sockaddr *)&address, size) < 0)
   {
    close(this->listen_fd);
    this->listen_fd = -1;
    return false;
   }
   if (listen(this->listen_fd,MAX_LISTEN_QUEUE_LEN) < 0)
   {
    close(this->listen_fd);
    this->listen_fd = -1;
    return false;
   }
   return true;
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

 class UnixTcpClient
 {
 public:
  UnixTcpClient()
  {
   this->connect_fp = -1;
  }
  ~UnixTcpClient()
  {
   this->Close();
  }
  int Open(const char* name)
  {
   this->connect_fp = socket(AF_UNIX, SOCK_STREAM, 0);
   if( this->connect_fp<0 ) return -1;
   
   struct sockaddr_un address;
   memset(&address,0,sizeof(address));
   address.sun_family = AF_UNIX;
   strncpy(address.sun_path,name,sizeof(address.sun_path));
   address.sun_path[sizeof(address.sun_path)-1]=0;
   int size = offsetof(struct sockaddr_un, sun_path) + strlen(address.sun_path);
   if(connect(this->connect_fp,(struct sockaddr*)&address,size)<0)
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

#endif //WLL_UNIX_SOCKET_H

