#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#include "WllRemoteTrace.h"
#include "WllTraceConfig.h"
#include "WllTrace.h"
#include "WllUtil.h"
using namespace Wll::Util;
using namespace Wll::Tool;
using namespace Wll::Tool::LogInnerTypes;

bool RegisterSignalPipeHandler(void);
bool InitalizeReload(void);

void RemoveListenPortFile(int signal_no)
{
	char log_home[MAX_FILE_NAME_LENGTH]="";
	char log_server_name[MAX_FILE_NAME_LENGTH]="";
	GetLogHome(log_home,sizeof(log_home));
	snprintf(log_server_name,sizeof(log_server_name),"%s/%s",log_home,DEFAULT_REMOTE_LOG_SERVER_NAME);

	if( 0 == unlink(log_server_name) )
	{
		DEBUG_LOG( "unlink remote_log_server_file ["<<log_server_name<<"] success" );
	}
	else
	{
		DEBUG_LOG( "unlink remote_log_server_file ["<<log_server_name<<"] failed" );
	}

	//call exit to flush buffer to disk
	exit(0);
}

int main()
{
 //set file access mask mode
 umask(S_IWGRP|S_IWOTH);

 if(!RegisterSignalPipeHandler())
 {
  DEBUG_LOG("RegisterSingalPipeHandler failed in WllRemoteTraceServer!");
 }

 if(!InitalizeReload())
 {
  DEBUG_LOG("InitalizeReload failed in WllRemoteTraceServer!");
 }

 signal(SIGTERM, RemoveListenPortFile);
 signal(SIGINT, RemoveListenPortFile);
 signal(SIGPWR, RemoveListenPortFile);

 char log_home[MAX_FILE_NAME_LENGTH]="";
 char log_server_name[MAX_FILE_NAME_LENGTH]="";
 GetLogHome(log_home,sizeof(log_home));
 snprintf(log_server_name,sizeof(log_server_name),"%s/%s",log_home,DEFAULT_REMOTE_LOG_SERVER_NAME);

 RemoteTraceServer server;
 server.Run(log_server_name);

 return 0;
}
