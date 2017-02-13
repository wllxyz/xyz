#ifndef LOCAL_TRACE_H
#define LOCAL_TRACE_H

#include "WllGroupFile.h"
#include "WllTraceConfig.h"
#include "WllTraceInterface.h"
#include "WllUtil.h"

namespace Wll
{
	namespace Tool
	{
		//implement local trace print manage
		class LocalTrace : public TraceInterface
		{
		public:
			//get static instance
			static LocalTrace* GetInstance(void);
			//Initialize
			void SetLogName(const char* LogName=Wll::Util::GetProcessName());
			void SetProcessId(unsigned int process_id);
			char *GetLogName();
			//constructor
			LocalTrace();
			//destructor
			virtual ~LocalTrace();
			//open a trace obj to write local disk log
			bool Open();
			//close
			void Close(void);
			//print log message on disk file
			bool WriteLog(int mask,const char* msg,size_t len);
			//configure log print enable/disable
			virtual void Config(const Wll::Tool::LogInnerTypes::LogPrintConfig& configs);
			//format log message and output
			virtual bool Log(int mask,const char* msg);
		private:
			static LocalTrace* instance;  //static instance
			GroupFile info_trace_file;  //file to print info trace
			int info_trace_buffered_lines;   //record info_trace_file buffered log lines

			char log_name[Wll::Tool::LogInnerTypes::MAX_PROCESS_NAME];
			int process_id;

			Wll::Tool::LogInnerTypes::LogPrintConfig log_print_config; //config the log print or not
			bool is_opened;    //show the trace files are opened
		};//end of class LocalTrace

	};//end of namespace Tool
};//end of namespace Wll

#endif //LOCAL_TRACE_H


 
