#ifndef WLL_TRACE_H
#define WLL_TRACE_H

#include <sstream>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>


#include "WllTraceConfig.h"
#include "WllUtil.h"

bool PrintRemoteTrace(int mask,const char*msg);
bool PrintLocalTrace(int mask,const char* msg);
bool PrintTermTrace(int mask,const char* msg);
void PrintDebugLog(const char* msg);

#define REMOTE_TRACE(mask,msg) do{ \
	std::stringstream str; \
	str<<msg; \
	PrintRemoteTrace(mask,str.str().c_str()); \
	}while(0)

#define LOCAL_TRACE(mask,msg) do{ \
	std::stringstream str; \
	str<<msg; \
	PrintLocalTrace(mask,str.str().c_str()); \
	}while(0)

#define TERM_TRACE(mask,msg) do{ \
	std::stringstream str; \
	str<<msg; \
	PrintTermTrace(mask,str.str().c_str()); \
	}while(0)

  
#define TRACE(mask,msg) LOCAL_TRACE(mask,msg)

#define LOG(mask,msg) TRACE(mask,"["<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"] "<<msg)
#define ERROR(msg) LOG(Wll::Tool::LogInnerTypes::ERROR_TRACE, "[ERROR] "<<msg)
#define WARNING(msg) LOG(Wll::Tool::LogInnerTypes::WARNING_TRACE, "[WARNING] "<<msg)
#define INFO(msg) LOG(Wll::Tool::LogInnerTypes::INFO_TRACE, "[INFO] "<<msg)
#define FOOT() struct FootKeeper{ const char* fn; FootKeeper(const char* fname):fn(fname){INFO(">>>"<<"start "<<fn<<"<<<");} ~FootKeeper(){INFO("<<<"<<"end "<<fn<<">>>");} }function_name##auto_foot_keeper(__FUNCTION__)

#define REMOTE_LOG(mask,msg) REMOTE_TRACE(mask,"["<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"] "<<msg)
#define REMOTE_ERROR(msg) REMOTE_LOG(Wll::Tool::LogInnerTypes::ERROR_TRACE, "[ERROR] "<<msg)
#define REMOTE_WARNING(msg) REMOTE_LOG(Wll::Tool::LogInnerTypes::WARNING_TRACE, "[WARNING] "<<msg)
#define REMOTE_INFO(msg) REMOTE_LOG(Wll::Tool::LogInnerTypes::INFO_TRACE, "[INFO] "<<msg)
 
#define LOCAL_LOG(mask,msg) LOCAL_TRACE(mask,"["<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"] "<<msg)
#define LOCAL_ERROR(msg) LOCAL_LOG(Wll::Tool::LogInnerTypes::ERROR_TRACE, "[ERROR] "<<msg)
#define LOCAL_WARNING(msg) LOCAL_LOG(Wll::Tool::LogInnerTypes::WARNING_TRACE, "[WARNING] "<<msg)
#define LOCAL_INFO(msg) LOCAL_LOG(Wll::Tool::LogInnerTypes::INFO_TRACE, "[INFO] "<<msg) 

#define TERM_LOG(mask,msg) TERM_TRACE(mask,"["<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"] "<<msg)
#define TERM_ERROR(msg) TERM_LOG(Wll::Tool::LogInnerTypes::ERROR_TRACE, "[ERROR] "<<msg)
#define TERM_WARNING(msg) TERM_LOG(Wll::Tool::LogInnerTypes::WARNING_TRACE, "[WARNING] "<<msg)
#define TERM_INFO(msg) TERM_LOG(Wll::Tool::LogInnerTypes::INFO_TRACE, "[INFO] "<<msg)

#ifndef DEBUG
#define DEBUG_LOG(msg)
#else
#define DEBUG_LOG(msg) do{ \
 std::stringstream str; \
 char date_time_str[80]=""; \
 str<<"["<<Wll::Util::GetDateTimeStr(date_time_str,sizeof(date_time_str))<<" pid="<<getpid()<<" tid="<<pthread_self()<<"] ["<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"] "<<"[DEBUG]:"<<msg; \
 PrintDebugLog(str.str().c_str()); \
 }while(0)

#endif //DEBUG

#endif //WLL_TRACE_H

