#include "WllTraceConfig.h"
#include "WllLocalTrace.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
using namespace std;
#include "WllConfigure.h"
#include "WllUtil.h"
//#include "safe_str_lib.h"
using namespace Wll::Tool;
using namespace Wll::Util;

using namespace Wll::Tool::LogInnerTypes;


LogPrintConfig* LogPrintConfig::instance=NULL;

//control the log print configs
 LogPrintConfig* LogPrintConfig::GetInstance(void)
 {
  if(instance==NULL)
         {
   instance=new (nothrow) LogPrintConfig(GetLogConfigFile());
  }
  return instance;
 }

 LogPrintConfig::LogPrintConfig()
{
	this->log_level = INFO_TRACE;
	this->max_log_file_number=DEFAULT_MAX_LOG_FILE_NUMBER;
	this->max_log_file_size=DEFAULT_MAX_LOG_FILE_SIZE;
	this->max_log_buffered_lines=DEFAULT_MAX_LOG_BUFFERED_LINES;
}

LogPrintConfig::LogPrintConfig(const char* config_file)
{
	this->log_level = INFO_TRACE;
	this->max_log_file_number=DEFAULT_MAX_LOG_FILE_NUMBER;
	this->max_log_file_size=DEFAULT_MAX_LOG_FILE_SIZE;
	this->max_log_buffered_lines=DEFAULT_MAX_LOG_BUFFERED_LINES;

	this->Load(config_file);
}

bool LogPrintConfig::operator== (const LogPrintConfig& that)const
{
	return memcmp(this,&that,sizeof(LogPrintConfig))==0;
}

bool LogPrintConfig::Load(const char* config_file)
{
	Configure config;
	if(!config.Load(config_file))
	{
		//std::cerr<<"load config file ["<<config_file<<"] failed\n";
		return false;
	}

	char* log_name = LocalTrace::GetInstance()->GetLogName();
	
         if(!config.Get(log_name,"log_level",this->log_level))
		 	config.Get("Trace","log_level",this->log_level);
		 	
	if(!config.Get(log_name, "max_log_file_number",this->max_log_file_number))
		config.Get("Trace","max_log_file_number",this->max_log_file_number);
	
	if(!config.Get(log_name,"max_log_file_size",*((long*)(&this->max_log_file_size)) ))
		config.Get("Trace", "max_log_file_size", *((long*)(&this->max_log_file_size)) );
	

	if(!config.Get(log_name,"max_log_buffered_lines",this->max_log_buffered_lines))
		config.Get("Trace","max_log_buffered_lines",this->max_log_buffered_lines);
	
	return true;

}


//configs transmit from client to server
RemoteTraceConfig::RemoteTraceConfig()
{
	this->process_id = getpid();
	GetProcessName(this->process_name, sizeof(this->process_name));
}

RemoteTraceConfig::RemoteTraceConfig(int32_t process_id, const char* process_name, const LogPrintConfig& print_config)
{
	this->process_id=process_id;
	strncpy(this->process_name,process_name,sizeof(this->process_name)-1);
	this->process_name[sizeof(this->process_name)-1]=0;
	this->print_config=print_config;
}

//write a RemoteTraceConfig obj to socket
bool RemoteTraceConfig::Write(int socket_id)
{
	return(sizeof(RemoteTraceConfig)==write(socket_id,this,sizeof(RemoteTraceConfig)));
}
//read a RemoteTraceConfig obj from socket
bool RemoteTraceConfig::Read(int socket_id)
{
	return(sizeof(RemoteTraceConfig)==read(socket_id,this,sizeof(RemoteTraceConfig)));
}

//log message transmit from client to server

//write a RemoteTraceMessage from client to server
bool RemoteTraceMessage::Write(int socket_id)
{
	return (sizeof(RemoteTraceMessage)==write(socket_id,this,sizeof(RemoteTraceMessage)));
}
//read a RemoteTraceMessage from client to server
bool RemoteTraceMessage::Read(int socket_id)
{
	memset((void*)this,0,sizeof(RemoteTraceMessage));
	return (sizeof(RemoteTraceMessage)==read(socket_id,this,sizeof(RemoteTraceMessage)));
}


