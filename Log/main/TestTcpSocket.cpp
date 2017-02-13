#include "WllTcpSocket.h"
#include <iostream>
using namespace std;
using namespace Wll::System;

int main(int argc,char* argv[])
{
	if(argc != 2)
	{
		cout<<"Usage : <cmd> <host:port> "<<endl;
		return -1;
	}

	TcpClient client;
	int fd = client.Open(argv[1]);
	if(fd < 0)
		cerr<<"connect failed"<<endl;
	DEBUG_LOG("connect success!!");
	char request[]="GET / HTTP/1.0\r\n\r\n";
	write(fd,request,sizeof(request)-1);
	char buffer[1024];
	int n = read(fd,buffer,sizeof(buffer));
	buffer[n]=0;
	cout<<buffer;
	close(fd);
	return 0;
}

