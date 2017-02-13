#ifndef	C_LOG_H
#define C_LOG_H

#ifdef	__cplusplus
extern "C" void PrintCLog(int mask, const char* format, ...);
extern "C" void SetLogName(const char* log_name);
#else
void PrintCLog(int mask, const char* format, ...);
void SetLogName(const char* log_name);
#endif

#define CLOG(mask, format, ...) PrintCLog(mask, "[%s:%d:%s] " format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#endif	//C_LOG_H
