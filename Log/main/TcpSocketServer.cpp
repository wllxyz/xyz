#include "WllTcpSocket.h"
#include <iostream>
using namespace std;
#include "WllThread.h"
#include <stdio.h>
using namespace Wll::System;

class TestThread : public Thread
{
public:
	TestThread(int fd)
	{
		this->fd = fd;
	}
	virtual void Run()
	{
		while(true)
		{
			char buf[1024];
			int n = read(this->fd,buf,sizeof(buf)-1);
			if(n<0)
			{
				cout<<"server : read data error"<<endl;
				close(this->fd);
				break;
			}
			else if(n==0)
			{
				cout<<"server : peer closed connect"<<endl;
				close(this->fd);
				break;
			}
			else
			{
				buf[n]=0;
				cout<<"server : received message : "<<buf<<endl;
				write(this->fd,buf,n);
			}
		}
	}
private:
	int fd;
};

int main()
{
	TcpServer server;
	int listen_fd = server.Open("127.0.0.1:9999");
	if(listen_fd<0)
	{
		cout<<"open server failed"<<endl;
		cout<<"error code : "<<listen_fd<<endl;
		return -1;
	}
	cout<<"open server success"<<endl;
	
	while(true)
	{
		int accept_fd = server.Accept();
		if(accept_fd>0)
		{
			TestThread* thread = new TestThread(accept_fd);
			thread->EnableAutoDestroy(true);
			thread->Start();
		}
	}
}

