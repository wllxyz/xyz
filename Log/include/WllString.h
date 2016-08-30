#ifndef WLL_STRING_H
#define WLL_STRING_H

#include <string>
#include <vector>

namespace Wll
{
namespace Util
{

void strip(char* line);

const char* split(const char* str, char* field, int field_size,char separator);
void split(const std::string& str, char separator, std::vector<std::string>& fields);

bool Int2String(int i,std::string& str);

bool String2Int(const std::string& str,int& i);

bool Long2String(long l,std::string& str);

bool String2Long(const std::string& str,long& l);

bool Float2String(float f,std::string& str);

bool String2Float(const std::string& str,float& f);

bool Double2String(double d,std::string& str);

bool String2Double(const std::string& str,double& d);

};//end of namespace Util
};//end of namespace Wll

#endif //WLL_STRING_H

