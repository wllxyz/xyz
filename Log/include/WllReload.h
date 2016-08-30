#ifndef WLL_RELOAD_H
#define WLL_RELOAD_H

#include "WllThread.h"

namespace Wll
{
namespace Tool
{

class ReloadServerThread : public Wll::System::Thread
{
public:
 static const int MAX_SERVER_NAME_LENGTH=1024;
 typedef bool (*RELOAD_METHOD)(void);
public:
 ReloadServerThread(const char*server_name,RELOAD_METHOD reload_method);
private:
 virtual void Run(void);
private:
 char server_name[MAX_SERVER_NAME_LENGTH];
 RELOAD_METHOD reload_method;
};//end of class ReloadServerThread

class ReloadClient
{
public:
 static const int MAX_SERVER_NAME_LENGTH=1024;
public:
 ReloadClient(const char* server_name);
 bool Reload(void);
private:
 char server_name[MAX_SERVER_NAME_LENGTH];
};//end of class ReloadClient

};//end of namespace Tool
};//end of namespace Wll

#endif //WLL_RELOAD_H

