#ifndef WLL_CONFIGURE_H
#define WLL_CONFIGURE_H

#include <string>
#include <map>

namespace Wll
{
namespace Tool
{

class Configure
{
public:
 static const int MAX_CONFIG_ITEM_LENGTH=512;

 static const char SPACE_STRING[10];
 static const char COMMENTARY_BEGINNING = '#';
 static const char KEY_FIELD_SEPERATOR = '=';
 static const char VALUE_FIELD_SEPERATOR = ';';
public:
 Configure(){}
 Configure(const char* config_file)
 {
  this->Load(config_file);
 }
 //load config items from environ variable
 bool Load();
 //load config items from config file
 bool Load(const char* config_file);
 //save config items to config file
 bool Save(const char* config_file);

 bool Get(const char* space,const char* key, std::string& value);
 bool Set(const char* space,const char* key,const std::string& value);

 bool Get(const char* space,const char* key,char* value,size_t size);
 bool Set(const char* space,const char* key,const char* value);

 bool Get(const char* space,const char* key,bool& value);
 bool Set(const char* space,const char* key,const bool& value);

 bool Get(const char* space,const char* key,char& value);
 bool Set(const char* space,const char* key,const char& value);

 bool Get(const char* space,const char* key,int& value);
 bool Set(const char* space,const char* key,const int& value);

 bool Get(const char* space,const char* key,long& value);
 bool Set(const char* space,const char* key,const long& value);

 bool Get(const char* space,const char* key,float& value);
 bool Set(const char* space,const char* key,const float& value);

 bool Get(const char* space,const char* key,double& value);
 bool Set(const char* space,const char* key,const double& value);
private:
 std::map<std::string,std::string> table;
};//end of Configure

};//end of namespace Tool
};//end of namespace Wll

#endif //WLL_CONFIGURE_H

