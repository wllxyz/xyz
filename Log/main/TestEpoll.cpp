#include <sys/epoll.h>
#include "WllTcpSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
using namespace std;
using namespace Wll::System;

void do_use_fd(int fd)
{
	static char buf[11];
	static int index = 0;
	int n = read(fd,buf+index,sizeof(buf)-1-index);
	if(n<0)
	{
		cout<<"server : read data error"<<endl;
		close(fd);
	}
	else if(n==0)
	{
		cout<<"server : peer closed connect"<<endl;
		close(fd);
	}
	else
	{
		buf[n]=0;
		index += n;
		if(index == sizeof(buf)-1)
		{
			cout<<"server : received message : "<<buf<<endl;
			write(fd,buf,sizeof(buf)-1);
			index = 0;
		}
	}
}

void setnonblocking(int conn_sock)
{
   	//int fcntl(int filedes, int cmd, ... /* int arg */ );
	fcntl(conn_sock, F_SETFL, fcntl(conn_sock, F_GETFL)|O_NONBLOCK);	
}

int main()
{
	#define MAX_EVENTS 10
	struct epoll_event ev, events[MAX_EVENTS];
	int listen_sock, conn_sock, nfds, epollfd;

	/* Set up listening socket, 'listen_sock' (socket(),
	   bind(), listen()) */
	TcpServer server;
	listen_sock = server.Open("127.0.0.1:9999");
	if(listen_sock<0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	epollfd = epoll_create(10);
	if (epollfd == -1) {
	    perror("epoll_create");
	    exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
	    perror("epoll_ctl: listen_sock");
	    exit(EXIT_FAILURE);
	}

	for (;;) {
	    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
	    if (nfds == -1) {
		perror("epoll_pwait");
		exit(EXIT_FAILURE);
	    }

	    for (int n = 0; n < nfds; ++n) {
			if (events[n].data.fd == listen_sock) {
				conn_sock = server.Accept();
				if (conn_sock == -1) {
					perror("accept");
					exit(EXIT_FAILURE);
				}
				setnonblocking(conn_sock);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_sock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
							&ev) == -1) {
					perror("epoll_ctl: conn_sock");
					exit(EXIT_FAILURE);
				}
			} else {
				do_use_fd(events[n].data.fd);
			}
	    }
	}

	return 0;
}
