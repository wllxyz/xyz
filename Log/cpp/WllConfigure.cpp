#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;
#include "WllConfigure.h"
#include "WllString.h"
//#include "safe_str_lib.h"
#include "WllTrace.h"
using namespace Wll::Tool;
using namespace Wll::Util;

const char Configure::SPACE_STRING[10] = "::";

//load config items from environ variable
bool Configure::Load()
{
 string space_prefix = "[SYSTEM_ENVIRON]";
 for(char** item=environ; *item; item++)
 {
  char key_field[MAX_CONFIG_ITEM_LENGTH]="";
  char value_field[MAX_CONFIG_ITEM_LENGTH]="";

  const char* p=split(*item,key_field,sizeof(key_field),'=');
  if(p==NULL) continue;
  p=split(p,value_field,sizeof(value_field),'\0');
  //if(p==NULL) continue;
  
  this->table[space_prefix + SPACE_STRING + key_field] = string(value_field);
 }
 return true;
}

//load config items from config file
bool Configure::Load(const char* config_file)
{
 ifstream input(config_file);
 if(!input.is_open())
 {
  DEBUG_LOG("Load config_file ["<<config_file<<"] failed!");
  return false;
 }
 DEBUG_LOG("Load config_file["<<config_file<<"] success!");

 string space_prefix="[]";
 do
 {
  char line[1024]="";
  input.getline(line,sizeof(line));

  DEBUG_LOG("get config line ["<<line<<"]");

  if(line[0]=='\0' || line[0]==COMMENTARY_BEGINNING)
  {
   DEBUG_LOG("scaned empty line or commentary line begin with "<<COMMENTARY_BEGINNING);

   continue;
  }
  
  if(line[0]=='[' && line[strlen(line)-1]==']')
  {
   DEBUG_LOG("line["<<line<<"] is a spacename line");

   space_prefix=line;
  }
  else
  {
   DEBUG_LOG("scaned a config item line["<<line<<"]");

   char key_field[MAX_CONFIG_ITEM_LENGTH]="";
   char value_field[MAX_CONFIG_ITEM_LENGTH]="";

   const char* p=split(line,key_field,sizeof(key_field),KEY_FIELD_SEPERATOR);
   if(p==NULL) continue;
   
   DEBUG_LOG("split key field ["<<key_field<<"] with line ["<<line<<"]");

   p=split(p,value_field,sizeof(value_field),VALUE_FIELD_SEPERATOR);
   if(p==NULL) continue;

   DEBUG_LOG("split value field ["<<value_field<<"] with line ["<<line<<"]");

   //skip other fields
   //add key-value to table
   this->table[space_prefix+SPACE_STRING+key_field]=string(value_field);
  }
 }while(!input.eof());
 return true;
}
//save config items to config file
bool Configure::Save(const char* config_file)
{
 ofstream output(config_file);
 if(!output.is_open())
 {
  DEBUG_LOG("open config_file ["<<config_file<<"] to save failed!");
  return false;
 }
 DEBUG_LOG("open config_file ["<<config_file<<"] to save success");
 
 DEBUG_LOG("begin traverse config items ...");

 string space_prefix="[]";
 for(map<string,string>::iterator i=this->table.begin(); i!=this->table.end(); ++i)
 {
  string key=i->first;
  string value=i->second;

  DEBUG_LOG("key=["<<key<<"], value=["<<value<<"]");

  size_t found=key.find(SPACE_STRING);
  if(found==string::npos)
  {
   DEBUG_LOG("not found SPACE_STRING ["<<SPACE_STRING<<"] in key["<<key<<"], continue to scan next item");
   continue;
  }

  string space_field=key.substr(0,found);
  DEBUG_LOG("get space_field["<<space_field<<"] of key["<<key<<"]");
  
  string key_field=key.substr(found+strlen(SPACE_STRING));
  DEBUG_LOG("get key_field["<<key_field<<"] of key["<<key<<"]");

  DEBUG_LOG("last space_prefix["<<space_prefix<<"] current space_field["<<space_field<<"]");
  if(space_field.compare(space_prefix)!=0)
  {
   DEBUG_LOG("["<<space_prefix<<"] != ["<<space_field<<"]");

   space_prefix=space_field;

   DEBUG_LOG("current space_prefix saved as last space_prefix: space_prefix=["<<space_prefix<<"],space_field=["<<space_field<<"]");

   output<<space_prefix<<endl;

   DEBUG_LOG("space_prefix["<<space_prefix<<"] saved in config_file["<<config_file<<"]");
  }
  output<<key_field<<KEY_FIELD_SEPERATOR<<value<<VALUE_FIELD_SEPERATOR<<endl;
  
  DEBUG_LOG("["<<key_field<<KEY_FIELD_SEPERATOR<<value<<VALUE_FIELD_SEPERATOR<<"] saved in config_file["<<config_file<<"]");
 }

 return true;
}
       
bool Configure::Get(const char* space,const char* key,string& value)
{
 map<string,string>::iterator i=this->table.find(string("[")+space+"]"+SPACE_STRING+key);
 if(i==this->table.end())
 {
  return false;
 }
 value=i->second;
 return true;
}

bool Configure::Get(const char* space,const char* key,char* value,size_t size)
{
 string str_val;
 if(!this->Get(space,key,str_val)) return false;
 snprintf(value,size,"%s",str_val.c_str());
 return true;
}

bool Configure::Set(const char* space,const char* key,const string& value)
{
 try{
  this->table[string("[")+space+"]"+SPACE_STRING+key]=value;
 }catch(...){
  return false;
 }
 return true;
}

bool Configure::Set(const char* space,const char* key,const char* value)
{
 return this->Set(space,key,string(value));
}

       
bool Configure::Get(const char* space,const char* key,bool& value)
{
 string str_val;
 if(!this->Get(space,key,str_val)) return false;
 value= (0==strcasecmp(str_val.c_str(),"TRUE"));

 return true;
}
bool Configure::Set(const char* space,const char* key,const bool& value)
{
 string str_val=string(value?"TRUE":"FALSE");
 return this->Set(space,key,str_val);
}
       
bool Configure::Get(const char* space,const char* key,char& value)
{
 string str_val;
 if(!this->Get(space,key,str_val)) return false;
 if(str_val.size()!=1) return false;
 value=str_val[0];
 return true;
}
bool Configure::Set(const char* space,const char* key,const char& value)
{
 string str_val;
 str_val.push_back(value);
 return this->Set(space,key,str_val);
}
       
bool Configure::Get(const char* space,const char* key,int& value)
{
 string str_val;
 if(!this->Get(space,key,str_val)) return false;
 return String2Int(str_val,value);
}

bool Configure::Set(const char* space,const char* key,const int& value)
{
 string str_val;
 if(!Int2String(value,str_val)) return false;
 return this->Set(space,key,str_val);
}
       
bool Configure::Get(const char* space,const char* key,long& value)
{
 string str_val;
 if(!this->Get(space,key,str_val)) return false;
 return String2Long(str_val,value);
}

bool Configure::Set(const char* space,const char* key,const long& value)
{
 string str_val;
 if(!Long2String(value,str_val)) return false;
 return this->Set(space,key,str_val);
}
       
bool Configure::Get(const char* space,const char* key,float& value)
{
 string str_val;
 if(!this->Get(space,key,str_val)) return false;
 return String2Float(str_val,value);
}

bool Configure::Set(const char* space,const char* key,const float& value)
{
 string str_val;
 if(!Float2String(value,str_val)) return false;
 return this->Set(space,key,str_val);
}
       
bool Configure::Get(const char* space,const char* key,double& value)
{
 string str_val;
 if(!this->Get(space,key,str_val)) return false;
 return String2Double(str_val,value);
}

bool Configure::Set(const char* space,const char* key,const double& value)
{
 string str_val;
 if(!Double2String(value,str_val)) return false;
 return this->Set(space,key,str_val);
}


 
