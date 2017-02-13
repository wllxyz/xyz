#include "WllLocalTrace.h"

#include "WllTrace.h"
#include "WllUtil.h"
#include "WllGroupFile.h"
#include "WllTraceConfig.h"
#include "WllTraceInterface.h"
using namespace Wll::Util;
using namespace Wll::Tool;
using namespace Wll::Tool::LogInnerTypes;

#include <cstring>
#include <pthread.h>
#include <new>
using namespace std;
#include <sys/stat.h>
#include <sys/types.h>

bool InitalizeReload(void);


//implement local trace print manage

LocalTrace* LocalTrace::instance=NULL;

LocalTrace* LocalTrace::GetInstance(void)
{
	if(LocalTrace::instance==NULL)
	{
		DEBUG_LOG("LocalTrace::instance==NULL, try to lock create_instance_lock ...");
		static pthread_mutex_t create_instance_lock=PTHREAD_MUTEX_INITIALIZER;
		if(0==pthread_mutex_trylock(&create_instance_lock))
		{
			//do the instance create
			DEBUG_LOG("create_instance_lock locked,creating instance of LocalTrace ...");

			LocalTrace::instance=new(nothrow) LocalTrace();

			DEBUG_LOG("Initalizing Reload ...");
			if(!InitalizeReload())
			{
				DEBUG_LOG("InitalizeReload for local trace failed!");
			}
			DEBUG_LOG("Reload initalized.");

			//release lock
			pthread_mutex_unlock(&create_instance_lock);
			DEBUG_LOG("create_instance_lock unlocked.");
		}
		else
		{
			DEBUG_LOG("try create_instance_lock failed,waiting the first locked thread complete the instance creatation");
			//wait the first locked thread complete the instance creatation
			pthread_mutex_lock(&create_instance_lock);      //block to wait
			DEBUG_LOG("create_instance_lock LOCKED");
			DEBUG_LOG("do nothing...");
			//do nothing
			pthread_mutex_unlock(&create_instance_lock);
			DEBUG_LOG("create_instance_lock UNLOCKED");
		}
	}

	DEBUG_LOG( ((LocalTrace::instance==NULL)?"LocalTrace::instance==NULL":"LocalTrace::instance!=NULL") );

	return LocalTrace::instance;
}


//constructor  
LocalTrace::LocalTrace()
{
	this->info_trace_buffered_lines=0;
	this->is_opened=false;
	GetProcessName(this->log_name, sizeof(this->log_name));
	this->process_id=getpid();
}

LocalTrace::~LocalTrace()
{
	this->Close();
}

//open a trace obj to write local disk log
//format of filename_template : "<prefix>%s<suffix>%d" = "<prefix>info<suffix>%d" "<prefix>warning<suffix>%d" "<prefix>error<suffix>%d"
//the %s will be substitute by "info" "warning" and "error"
//the %d MUST BE AFTER THE %s
bool LocalTrace::Open()
{
	//we not use auto-close when open another file when not closed
	DEBUG_LOG("check whether LocalTrace is opened, this->is_opened=["<<this->is_opened<<"]");
	if(this->is_opened)
	{
		DEBUG_LOG("LocalTrace is already opened, return true directly, not openning new trace files");
		return true;
	}
	DEBUG_LOG("LocalTrace not opened, try open it...");

	char date_buf[80]="";
	char log_path[MAX_FILE_NAME_LENGTH]="";
	GetDateStr(date_buf,sizeof(date_buf));
	GetLogPath(log_path,sizeof(log_path));
	DEBUG_LOG("date["<<date_buf<<"],log_path["<<log_path<<"]");

	mkdir(log_path,0777);
	DEBUG_LOG("composing info_trace_filename ...");
	//get all_trace_file name
	string info_trace_dirname = string(log_path) + "/" + this->log_name;
	DEBUG_LOG("info_trace_dirname["<<info_trace_dirname<<"] composed.");
	mkdir(info_trace_dirname.c_str(),0777);
	info_trace_dirname += "/" + string(date_buf);
	mkdir(info_trace_dirname.c_str(),0777);

	//get all_trace_file name
	char info_trace_filename[MAX_FILE_NAME_LENGTH]="";
	snprintf(info_trace_filename, sizeof(info_trace_filename), "%s/%s.%d.%%d.log", info_trace_dirname.c_str(), this->log_name, this->process_id);
	DEBUG_LOG("info_trace_filename["<<info_trace_filename<<"] composed.");

	DEBUG_LOG("Open info_trace_file with [info_trace_filename="<<info_trace_filename<<",open_mode="<<GroupFile::OPEN_APPEND \
	<<",max_log_file_number="<<this->log_print_config.max_log_file_number<<",max_log_file_size="<<this->log_print_config.max_log_file_size \
	<<",max_log_buffered_bytes="<<this->log_print_config.max_log_buffered_lines*MAX_LOG_LENGTH<<"...");
	//open all_trace_file
	if(!this->info_trace_file.Open(info_trace_filename,GroupFile::OPEN_APPEND,this->log_print_config.max_log_file_number, this->log_print_config.max_log_file_size, this->log_print_config.max_log_buffered_lines*MAX_LOG_LENGTH))
	{
		TERM_ERROR("open info_trace_file ["<<info_trace_filename<<"] failed, return false");
		return false;
	}
	DEBUG_LOG("info_trace_file["<<info_trace_filename<<"] opened success");

	//If all right, return true,else return false
	this->is_opened=true;
	DEBUG_LOG("Mark this->is_opened=["<<this->is_opened<<"]");

	return true;
}

//close all opened trace files 
void LocalTrace::Close() 
{
	if(this->is_opened)
	{
		//close all_trace_file
		this->info_trace_file.Close();
		this->is_opened = false;
	}
}  

//print log message on disk file 
bool LocalTrace::WriteLog(int mask,const char* message,size_t len)
{
	const char* msg=message;
	char buf[MAX_LOG_LENGTH]="";

	DEBUG_LOG("Test LocalTrace is opened or not ...");
	if(!this->is_opened)
	{
		DEBUG_LOG("LocalTrace is NOT opened, return false");
		return false;
	}
	DEBUG_LOG("LocalTrace is opened, this->is_opened=["<<this->is_opened<<"]");

	DEBUG_LOG("Test the message["<<message<<"] is truncated or not ...");
	//when truncate string, to keep string end by '\n'
	if(message[len-1]!='\n')
	{
		DEBUG_LOG("message["<<message<<":len="<<len<<"] not end with new line");
		DEBUG_LOG("copying message to buf ...");
		memcpy(buf,message,len);
		buf[sizeof(buf)-1]=0;
		DEBUG_LOG("after copy : buf=["<<buf<<"]");

		DEBUG_LOG("replace the last character of message with new line");
		buf[len-1]='\n';
		DEBUG_LOG("buf["<<buf<<"] with new line at tail");

		DEBUG_LOG("switch msg to point buf ...");
		DEBUG_LOG("before switch : msg["<<msg<<"]==message["<<message<<"]");
		msg=buf;
		DEBUG_LOG("after switch : msg["<<msg<<"]==buf["<<buf<<"]");
	}

	bool ret=true;

	DEBUG_LOG("Test info trace show or not ... maks="<<mask<<", conf.log_level="<<this->log_print_config.log_level);
	//Write all_trace_file message
	if(mask <= this->log_print_config.log_level)
	{
		DEBUG_LOG("info show enabled, showing info trace ...");
		ret = ret && (len == this->info_trace_file.Write(msg,len));

		DEBUG_LOG("this->info_trace_buffered_lines=["<<this->info_trace_buffered_lines<<"]");
		this->info_trace_buffered_lines++;
		DEBUG_LOG("this->info_trace_buffered_lines=["<<this->info_trace_buffered_lines<<"]");

		if(this->info_trace_buffered_lines>=this->log_print_config.max_log_buffered_lines)
		{
			DEBUG_LOG("max buffered info lines reached,info_trace_buffered_lines["<<this->info_trace_buffered_lines<<"]>=max_log_buffered_lines["<<this->log_print_config.max_log_buffered_lines<<"]");

			DEBUG_LOG("flushing info_trace_file ...");
			this->info_trace_file.Flush();
			DEBUG_LOG("info_trace_file flushed.");

			DEBUG_LOG("reset info_trace_buffered_lines=0...");
			this->info_trace_buffered_lines=0;
			DEBUG_LOG("this->info_trace_buffered_lines=["<<this->info_trace_buffered_lines<<"]");
		}
	}
	DEBUG_LOG("return value="<<ret);
	return ret;
}

bool LocalTrace::Log(int mask,const char* msg)
{
	if(!this->is_opened)
	{
		DEBUG_LOG("LocalTrace is not opened, return false");
		return false;
	}
	DEBUG_LOG("confirmed that LocalTrace is opened before do logging");

	if(mask > this->log_print_config.log_level)
	{
		DEBUG_LOG("all log print disabled,NOT print anything,return true. mask="<<mask<<", config.log_level="<<this->log_print_config.log_level);
		return true;
	}

	static char old_date[80]="";
	char new_date[80]="";

	char log_text[MAX_LOG_LENGTH]="";
	char date_time_str[80]="";

	DEBUG_LOG("check inital variables : log_text["<<log_text<<"],date_time_str["<<date_time_str<<"]");

	DEBUG_LOG("GetDateStr for new_date...");
	GetDateStr(new_date,sizeof(new_date));
	DEBUG_LOG("new_date["<<new_date<<"] got");

	DEBUG_LOG("GetDateTimeStr for date_time_str ...");
	GetDateTimeStr(date_time_str,sizeof(date_time_str));
	DEBUG_LOG("date_time_str["<<date_time_str<<"] got");

	DEBUG_LOG("composing log_text["<<log_text<<"] with msg["<<msg<<"]...");
	snprintf(log_text,sizeof(log_text),"[%s pid=%ld tid=%ld] %s\n",date_time_str, long(getpid()), long(pthread_self()), msg);
	DEBUG_LOG("log_text["<<log_text<<"] composed.");

	bool ret=true; //record error status

	static pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&lock);

	DEBUG_LOG("write log lock locked");

	DEBUG_LOG("Testing new_date["<<new_date<<"] and old_date["<<old_date<<"] ...");
	if(strncmp(new_date,old_date,80)!=0)
	{
		DEBUG_LOG("new_date["<<new_date<<"] != old_date["<<old_date<<"]");
		DEBUG_LOG("closing LocalTrace ...");
		this->Close();
		DEBUG_LOG("LocalTrace closed.");

		ret = ret && this->Open();
		DEBUG_LOG( (ret?"open success":"open failed") );
		strncpy(old_date,new_date,sizeof(old_date));
		old_date[sizeof(old_date)-1]=0;
	}
	DEBUG_LOG("config log print config ...");
	this->Config( *LogPrintConfig::GetInstance() );
	DEBUG_LOG("log print configed.");

	DEBUG_LOG("writing log["<<log_text<<"] to disk files ...");
	ret = ret && this->WriteLog(mask,log_text,strnlen(log_text,sizeof(log_text)));
	DEBUG_LOG( "ret=["<<ret<<"]" );

	pthread_mutex_unlock(&lock);
	DEBUG_LOG("write log lock unlocked.");

	return ret;
}


//configure log print enable/disable   
void LocalTrace::Config(const LogPrintConfig& configs)
{
	DEBUG_LOG("Testing config is new or not ...");
	DEBUG_LOG("configs : max_log_file_number="<<configs.max_log_file_number \
			<<",max_log_file_size="<<configs.max_log_file_size \
			<<",max_log_buffered_lines="<<configs.max_log_buffered_lines \
			<<",log_level="<<configs.log_level);
	DEBUG_LOG("this->log_print_config : max_log_file_number="<<this->log_print_config.max_log_file_number \
			<<",max_log_file_size="<<this->log_print_config.max_log_file_size \
			<<",max_log_buffered_lines="<<this->log_print_config.max_log_buffered_lines \
			<<",log_level="<<this->log_print_config.log_level);
	if(!(this->log_print_config==configs))
	{
		DEBUG_LOG("configs is new,updating current used log_print_config ...");
		DEBUG_LOG("switching LocalTrace::Config...");

		//copy the configure items about enable/disable log print
		this->log_print_config=configs;
		DEBUG_LOG("config info_trace_file ...");
		//config all_trace_file's files/size/buffer configure
		this->info_trace_file.Config(this->log_print_config.max_log_file_number, this->log_print_config.max_log_file_size,this->log_print_config.max_log_buffered_lines*MAX_LOG_LENGTH);

		DEBUG_LOG("LocalTrace::Config log_print_config switched");
	}
}


void LocalTrace::SetLogName(const char* LogName)
{
	strncpy(this->log_name, LogName, sizeof(this->log_name));
	this->log_name[sizeof(this->log_name)-1] = 0;
}


void LocalTrace::SetProcessId(unsigned int process_id)
{
	this->process_id = process_id;
}

char * LocalTrace::GetLogName()
{
	return this->log_name;
}
 
