#ifndef WLL_TRACE_INTERFACE_H
#define WLL_TRACE_INTERFACE_H

#include "WllTraceConfig.h"

namespace Wll
{
namespace Tool
{

class TraceInterface
{
public:
 //write log
 virtual bool Log(int mask,const char* msg)=0;
 //config log print at runtimes
 virtual void Config(const Wll::Tool::LogInnerTypes::LogPrintConfig& configs)=0;
};//end of class TraceInterface

};//end of namespace Tool
};//end of namespace Wll

#endif //WLL_TRACE_INTERFACE_H
 

