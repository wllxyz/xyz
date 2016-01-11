#include "WllEpoll.h"
#include "WllTcpSocket.h"
#include "WllTrace.h"
#include "WllSingleton.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
using namespace std;
using namespace Wll::System;

void SignalHandler(int signal_no)
{
	DEBUG_LOG("recv signal = "<<signal_no);
	Epoll *epoll = Singleton<Epoll>::GetInstance();
	epoll->EndWaitLoop();
}

int main()
{
	signal(SIGINT, SignalHandler);

	TcpServer server;
	int listen_sock = server.Open("127.0.0.1:9999");
	if(listen_sock<0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	TERM_LOG(Wll::Tool::LogInnerTypes::INFO_TRACE,"open 127.0.0.1:9999 tcp server socket ok");

	Epoll *epoll = Singleton<Epoll>::GetInstance();
	epoll->AddHandler(listen_sock, new TcpListenerEpollHandler(listen_sock, epoll), EPOLLIN);
	TERM_LOG(Wll::Tool::LogInnerTypes::INFO_TRACE,"tcp listener epoll handler added");

	TERM_LOG(Wll::Tool::LogInnerTypes::INFO_TRACE,"start wait and check loop ...");
	epoll->StartWaitLoop();

	return 0;
}
