#include "WllEpoll.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include "WllTrace.h"
#include <assert.h>
#include <sys/socket.h>

using namespace Wll::System;

Epoll::Epoll(size_t size)
{
	assert(size>0);

	this->epoll_size = size;
	this->epoll_events = new epoll_event[this->epoll_size];
	if(this->epoll_events == NULL)
	{
		ERROR("malloc epoll events memory failed");
		return;
	}
	this->epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	if(this->epoll_fd == -1)
	{
		switch(errno)
		{
			case EINVAL:
				ERROR("this would not happen");
				break;
			case EMFILE:
				ERROR("The per-user limit on the number of epoll instances imposed by  /proc/sys/fs/epoll/max_user_instances  was  encountered.");
				break;
			case ENFILE:
				ERROR("The system limit on the total number of open files has been reached.");
				break;
			case ENOMEM:
				ERROR("There was insufficient memory to create the kernel object.");
				break;
			default:
				ERROR("Unknow errno = "<<errno);
				break;
		}
	}
}

Epoll::~Epoll()
{
	if(this->epoll_fd>0)
	{
		close(this->epoll_fd);
		this->epoll_fd = -1;
	}

	if(this->epoll_events != NULL)
	{
		delete [] this->epoll_events;
		this->epoll_events = NULL;
	}

	for(std::map<int,EpollHandler*>::iterator i = this->handler_map.begin(); i != this->handler_map.end(); ++i)
	{
		EpollHandler* handler = i->second;
		if(handler != NULL)
		{
			delete handler;
		}
	}
}

bool Epoll::AddHandler(int fd, EpollHandler* handler, EventStatus event_status)
{
	struct epoll_event event;
	event.data.ptr = handler;
	event.events = event_status;
	if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		switch(errno)
		{
		case EBADF:
			ERROR("epoll_fd["<<this->epoll_fd<<"] is not a valid file descriptor.");
			break;
		case EEXIST:
			ERROR("op was EPOLL_CTL_ADD, and the supplied file descriptor fd["<<fd<<"] is already registered with this epoll instance.");
			break;
		case EINVAL:
			ERROR("maybe fd["<<fd<<"] is the same as epfd["<<this->epoll_fd<<"]");
			break;
		case ENOMEM:
			ERROR("There was insufficient memory to handle the requested EPOLL_CTL_ADD control operation.");
			break;
		case ENOSPC:
			ERROR("The  limit  imposed by /proc/sys/fs/epoll/max_user_watches was encountered while trying to register (EPOLL_CTL_ADD) a new file descriptor on an epoll instance.  See epoll(7) for further details.");
			break;
		case EPERM:
			ERROR("The target file fd does not support epoll.");
			break;
		default:
			ERROR("Unknow errno="<<errno);
			break;
		}
		return false;
	}
	this->handler_map[fd] = handler;
	return true;
}

bool Epoll::ModifyHandler(int fd, EpollHandler* handler, EventStatus event_status)
{
	struct epoll_event event;
	event.data.ptr = handler;
	event.events = event_status;
	if(epoll_ctl(this->epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1)
	{
		switch(errno)
		{
		case EBADF:
			ERROR("epoll_fd["<<this->epoll_fd<<"] is not a valid file descriptor.");
			break;
		case EEXIST:
			ERROR("op was EPOLL_CTL_ADD, and the supplied file descriptor fd["<<fd<<"] is already registered with this epoll instance.");
			break;
		case EINVAL:
			ERROR("maybe fd["<<fd<<"] is the same as epfd["<<this->epoll_fd<<"]");
			break;
		case ENOMEM:
			ERROR("There was insufficient memory to handle the requested EPOLL_CTL_ADD control operation.");
			break;
		case ENOSPC:
			ERROR("The  limit  imposed by /proc/sys/fs/epoll/max_user_watches was encountered while trying to register (EPOLL_CTL_ADD) a new file descriptor on an epoll instance.  See epoll(7) for further details.");
			break;
		case EPERM:
			ERROR("The target file fd does not support epoll.");
			break;
		case ENOENT:
			ERROR("op was EPOLL_CTL_MOD or EPOLL_CTL_DEL, and fd["<<fd<<"] is not registered with this epoll instance.");
			break;
		default:
			ERROR("Unknow errno="<<errno);
			break;
		}
		return false;
	}
	this->handler_map[fd] = handler;
	return true;
}

bool Epoll::DeleteHandler(int fd)
{
	if(epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		switch(errno)
		{
		case EBADF:
			ERROR("epoll_fd["<<this->epoll_fd<<"] is not a valid file descriptor.");
			break;
		case EEXIST:
			ERROR("op was EPOLL_CTL_ADD, and the supplied file descriptor fd["<<fd<<"] is already registered with this epoll instance.");
			break;
		case EINVAL:
			ERROR("maybe fd["<<fd<<"] is the same as epfd["<<this->epoll_fd<<"]");
			break;
		case ENOMEM:
			ERROR("There was insufficient memory to handle the requested EPOLL_CTL_ADD control operation.");
			break;
		case ENOSPC:
			ERROR("The  limit  imposed by /proc/sys/fs/epoll/max_user_watches was encountered while trying to register (EPOLL_CTL_ADD) a new file descriptor on an epoll instance.  See epoll(7) for further details.");
			break;
		case EPERM:
			ERROR("The target file fd does not support epoll.");
			break;
		case ENOENT:
			ERROR("op was EPOLL_CTL_MOD or EPOLL_CTL_DEL, and fd["<<fd<<"] is not registered with this epoll instance.");
			break;
		default:
			ERROR("Unknow errno="<<errno);
			break;
		}
		return false;
	}
	this->handler_map.erase(fd);
	return true;
}

bool Epoll::WaitAndCheck(int milliseconds)
{
    int nfds = epoll_pwait(this->epoll_fd, this->epoll_events, this->epoll_size, milliseconds, NULL);
    if (nfds == -1)
    {
		switch(errno)
		{
		case EBADF:
			ERROR("epfd["<<this->epoll_fd<<"] is not a valid file descriptor.");
			break;
		case EFAULT:
			ERROR("The memory area pointed to by events is not accessible with write permissions.");
			break;
		case EINTR:
			ERROR("The call was interrupted by a signal handler before either any of the requested events occurred or the  timeout  expired;  see signal(7).");
			break;
		case EINVAL:
			ERROR("epfd["<<this->epoll_fd<<"] is not an epoll file descriptor, or maxevents["<<this->epoll_size<<"] is less than or equal to zero.");
			break;
		default:
			ERROR("Unknow errno="<<errno);
			break;
		}
		return false;
    }

    for (int n = 0; n < nfds; ++n)
    {
    	EpollHandler* handler = static_cast<EpollHandler*>(this->epoll_events[n].data.ptr);
		if (handler != NULL)
		{
			handler->HandleEvent(handler->GetFd(), this->epoll_events[n].events);
		}
    }
}

void Epoll::StartWaitLoop()
{
	this->loop = true;
	while(this->loop)
	{
		this->WaitAndCheck(-1);
	}
}

void Epoll::EndWaitLoop()
{
	this->loop = false;
}

TcpListenerEpollHandler::TcpListenerEpollHandler(int fd, Epoll* epoll) : EpollHandler(fd)
{
	this->epoll = epoll;
}

void TcpListenerEpollHandler::HandleEvent(int fd, EventStatus event_status)
{
	int connect_fd = accept(fd, NULL, NULL);
	if(connect_fd > 0)
	{
		DEBUG_LOG("listener fd["<<fd<<"] accept connect fd="<<connect_fd);
		fcntl(connect_fd, F_SETFL, fcntl(connect_fd, F_GETFL)|O_NONBLOCK);
		this->epoll->AddHandler(connect_fd, new IOEpollHandler(connect_fd,this->epoll));
	}
}

IOEpollHandler::IOEpollHandler(int fd, Epoll* epoll) : EpollHandler(fd)
{
	this->epoll = epoll;
}

void IOEpollHandler::HandleEvent(int fd, EventStatus event_status)
{
	if(event_status & EPOLLERR)
	{
		DEBUG_LOG("fd["<<fd<<"] error now");
	}
	if(event_status & EPOLLIN)
	{
		DEBUG_LOG("fd["<<fd<<"] can read now");
	}
	if(event_status & EPOLLOUT)
	{
		DEBUG_LOG("fd["<<fd<<"] can write now");
	}
}

