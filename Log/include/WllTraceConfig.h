#ifndef TRACE_CONFIG_H
#define TRACE_CONFIG_H

#include <inttypes.h>



namespace Wll
{
namespace Tool
{
namespace LogInnerTypes
{

//default remote log server configurations
//static const char* DEFAULT_REMOTE_LOG_SERVER_IP="127.0.0.1";
//static const CM_SOCKET DEFAULT_REMOTE_LOG_SERVER_PORT=6666;
static const char* DEFAULT_REMOTE_LOG_SERVER_NAME="WLL_LOG_SERVER";

//default log configure: these configure should only effect on UNIX server
static const long DEFAULT_MAX_LOG_FILE_SIZE=20*1024*1024;
static const int DEFAULT_MAX_LOG_FILE_NUMBER=0;
static const int DEFAULT_MAX_LOG_BUFFERED_LINES=0;

//buffer size limits
static const int MAX_CONFIG_LENGTH=512;
static const int MAX_PROCESS_NAME=512;  
static const int MAX_LOG_LENGTH=8192;  
static const int MAX_FILE_NAME_LENGTH=512; 
static const int DEFAULT_SERVER_LISTEN_QUEUE_LENGTH=128;

//other limits
//static const int MAX_KEEP_ALIVE_WAIT_SECONDS=15*60;

static const int ERROR_TRACE=1;
static const int WARNING_TRACE=2;
static const int INFO_TRACE=3;
static const int DEBUG_TRACE=4;

static const char REMARK_MESSAGE=0;
static const char REMARK_CONFIG=-1;
//control the log print configs
struct LogPrintConfig
{
public:
	static LogPrintConfig* instance;
	static LogPrintConfig* GetInstance(void);
public:
	int log_level;
	int32_t max_log_file_number;
	int64_t max_log_file_size;
	int32_t max_log_buffered_lines;
public:
	LogPrintConfig();

	LogPrintConfig(const char* config_file);
	//test whether configure content changed
	bool operator== (const LogPrintConfig& that)const;

	bool Load(const char* config_file);

};//end of LogPrintConfig

//configs transmit from client to server
struct RemoteTraceConfig
{
public:
	RemoteTraceConfig();

	RemoteTraceConfig(int32_t process_id, const char* process_name, const LogPrintConfig& print_config);
public:
	LogPrintConfig print_config;
	int32_t process_id;
	char process_name[MAX_PROCESS_NAME];
public:
	//write a RemoteTraceConfig obj to socket
	bool Write(int socket_id);

	//read a RemoteTraceConfig obj from socket
	bool Read(int socket_id);

};//end of struct RemoteTraceConfig

//log message transmit from client to server
struct RemoteTraceMessage
{
public:
	int32_t log_mask;  //log type control which log file to print
	char log_text[MAX_LOG_LENGTH]; //log text formated by client
public:
	//write a RemoteTraceMessage from client to server
	bool Write(int socket_id);

	//read a RemoteTraceMessage from client to server
	bool Read(int socket_id);

};//end of struct RemoteTraceMessage

};//namespace LogInnerTypes
};//namespace Tool
};//namespace Wll

#endif //TRACE_CONFIG_H


 
