#ifndef	WLL_EPOLL_H
#define	WLL_EPOLL_H

#include <stdint.h>
#include <sys/epoll.h>
#include <map>

namespace Wll
{
	namespace System
	{
		typedef	uint32_t EventStatus;
		static const int DEFAULT_EPOLL_SIZE = 1024;

		class EpollHandler
		{
		public:
			EpollHandler(int fd) { this->fd = fd; }
			virtual void HandleEvent(int fd, EventStatus event_status)=0;
			int GetFd() { return fd; }
		protected:
			int fd;
		};

		class Epoll
		{
		public:
			Epoll(size_t poll_size = DEFAULT_EPOLL_SIZE);
			~Epoll();
			bool AddHandler(int fd, EpollHandler* handler, EventStatus event_status = (EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET));
			bool ModifyHandler(int fd, EpollHandler* handler, EventStatus event_status);
			bool DeleteHandler(int fd);
			bool WaitAndCheck(int milliseconds);
			void StartWaitLoop();
			void EndWaitLoop();
		private:
			int epoll_fd;
			int epoll_size;				//max events poll size, will remalloc when epoll_length>=epoll_size
			epoll_event* epoll_events;
			bool loop;
			std::map<int,EpollHandler*> handler_map;
		};//end of class Epoll

		class TcpListenerEpollHandler : public EpollHandler
		{
		public:
			TcpListenerEpollHandler(int fd, Epoll* epoll);
			virtual void HandleEvent(int fd, EventStatus event_status);
		private:
			Epoll* epoll;
		};

		class IOEpollHandler : public EpollHandler
		{
		public:
			IOEpollHandler(int fd, Epoll* epoll);
			virtual void HandleEvent(int fd, EventStatus event_status);
		private:
			Epoll* epoll;
		};
	}//end of namespace System
}//end of namespace Wll

#endif	//WLL_EPOLL_H
