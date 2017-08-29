#ifndef	INTEPRET_EXCEPTION_H
#define	INTEPRET_EXCEPTION_H

#include "Exception.h"
#include <string>

class IntepretException : public Exception
{
public:
	IntepretException(const std::string& msg):Exception("IntepretException",msg){}
};

#endif

