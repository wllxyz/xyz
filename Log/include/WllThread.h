#ifndef WLL_THREAD_H
#define WLL_THREAD_H

#include <pthread.h>
#include <stdio.h>

namespace Wll //provide oganization scope
{
 namespace System //provide category scope
 {
  class Thread
  {
  public:
   //enable self destroy flag to delete thread obj when thread_exit
   void EnableAutoDestroy(bool auto_destroy_flag)
   {
    this->self_destroy_flag=auto_destroy_flag;
   }
   //start thread function running
   bool Start(void)
   {
    return pthread_create(&this->thread,NULL,Thread::_run, this)==0;
   }
   //join to the calling thread, the calling thread would block
   bool Join(void)
   {
    return pthread_join(this->thread,NULL)==0;
   }
  protected:
   //shell to run thread function
   static void* _run(void* parameters)
   {
    Thread* p_thread=static_cast<Thread*>(parameters);
    p_thread->Run();
    if(p_thread->self_destroy_flag) { delete p_thread; }
    return NULL;
   }
   //need to override this method
   virtual void Run(void)=0;
   //constructor
   Thread(bool self_destroy_flag=false)
   {
    this->thread=0;
    this->self_destroy_flag=self_destroy_flag;
   }
   virtual ~Thread(){ }
  private:
   pthread_t thread; //thread handle to manage thread
   bool self_destroy_flag; //when use new to create a thread, the self_destroy_flag should set be TRUE to destroy the thread object when the thread finish.
  };//end of Thread
 };//end of namespace System
};//end of namespace Wll

#endif //WLL_THREAD_H

