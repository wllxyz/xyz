#ifndef	WLL_SINGLETON_H
#define WLL_SINGLETON_H

#include <new>
#include <cstdio>
#include <pthread.h>

template <typename TYPE> 
TYPE* GetInstance()
{
	static TYPE* instance = NULL;
	static pthread_mutex_t create_instance_lock=PTHREAD_MUTEX_INITIALIZER;
	if(instance==NULL)
	{
		if(0==pthread_mutex_trylock(&create_instance_lock))
		{
			instance = new (std::nothrow) TYPE;
			pthread_mutex_unlock(&create_instance_lock);
		}
		else
		{
			pthread_mutex_lock(&create_instance_lock);
			pthread_mutex_unlock(&create_instance_lock);
		}
	}
	return instance;
}

template <typename TYPE>
class Singleton
{
public:
	static TYPE* GetInstance()
	{
		return ::GetInstance<TYPE>();
	}
};

#endif

