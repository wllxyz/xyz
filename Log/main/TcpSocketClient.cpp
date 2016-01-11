#include "WllTcpSocket.h"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string>
using namespace Wll::System;

int main()
{
	TcpClient client;
	int fd = client.Open("127.0.0.1:9999");
	if(fd<0)
	{
		cout<<"open client failed"<<endl;
	}
	cout<<"open client success"<<endl;
	
	while(true)
	{
		string input;
		cin>>input;
		if(input=="bye") break;
		write(fd,input.c_str(),input.length());
		char buf[1024];
		int n = read(fd,buf,sizeof(buf)-1);
		if(n<0)
		{
			cout<<"client : read data error"<<endl;
			break;
		}
		else if(n==0)
		{
			cout<<"client : peer closed connect"<<endl;
			break;
		}
		else
		{
			buf[n] = 0;
			cout<<"client : reply from server:"<<buf<<endl;
		}
	}
	return 0;
}

