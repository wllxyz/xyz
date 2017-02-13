/*------------------------------------------------------*/
/* String convent to other basic type methods           */
/*                                                      */
/* WllString.cpp                                        */
/*                                                      */
/* Copyright (C) WebEx Communications Inc.              */
/* All rights reserved                                  */
/*                                                      */
/* Author                                               */
/*      Shine Wang (shinew@hz.webex.com)                */
/*                                                      */
/* History                                              */
/*    07/30/2010  ADD TO PLUG CmTraceFromT120.h         */
/*                                                      */
/*                                                      */
/*------------------------------------------------------*/

#include "WllString.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

void Wll::Util::strip(char* line)
{
 assert(line!=NULL);

 char* p=line;
 while(*p != '\0') p++;
 for(--p; *p == '\n' && p >= line; --p) *p='\0';
}

const char* Wll::Util::split(const char* str, char* field, int field_size,char separator)
{
 assert(str!=NULL);

 int n=0;
 const char* p=NULL;
 for(p=str; *p!='\0' && *p!=separator; p++)
 {
  if(n<field_size-1)
  {
   field[n++]=*p;
  }
 }
 field[n]='\0';
 
 return *p=='\0'?NULL:++p;
}

void Wll::Util::split(const std::string& str, char separator, std::vector<std::string>& fields)
{
	fields.clear();

	std::string field;
	for(std::string::const_iterator i = str.begin(); i != str.end(); ++i)
	{
		if(*i == separator)
		{
			fields.push_back(field);
			field.clear();
		}
		else
		{
			field.push_back(*i);
		}
	}
	fields.push_back(field);
}


bool Wll::Util::Int2String(int i,string& str)
{
 //strstream s;
 //s<<i;
 //str=s.str();
 char buf[80]="";
 snprintf(buf,sizeof(buf),"%d",i);
 str=string(buf);
 return true;
}

bool Wll::Util::String2Int(const string& str,int& i)
{
 if(str.size()==0) return false;
 i=atoi(str.c_str());
 return (!(i==0 && str[0]!='0'));
}

bool Wll::Util::Long2String(long l,string& str)
{
        //strstream s;
        //s<<l;
        //str=s.str();
 char buf[80]="";
 snprintf(buf,sizeof(buf),"%ld",l);
 str=string(buf);
        return true;
}

bool Wll::Util::String2Long(const string& str,long& l)
{
 if(str.size()==0) return false;
        l=atol(str.c_str());
        return (!(l==0 && str[0]!='0'));

}

bool Wll::Util::Float2String(float f,string& str)
{
        //strstream s;
        //s<<f;
        //str=s.str();
 char buf[80]="";
 snprintf(buf,sizeof(buf),"%f",f);
 str=string(buf);
        return true;
}

bool Wll::Util::String2Float(const string& str,float& f)
{
        if(str.size()==0) return false;
 char* p_end=NULL;
        f=strtod(str.c_str(),&p_end);
   return *p_end=='\0';
}

bool Wll::Util::Double2String(double d,string& str)
{
        //strstream s;
        //s<<d;
        //str=s.str();
 char buf[80]="";
 snprintf(buf,sizeof(buf),"%lf",d);
 str=string(buf);
        return true;
}

bool Wll::Util::String2Double(const string& str,double& d)
{
        if(str.size()==0) return false;
        char* p_end=NULL;
        d=strtod(str.c_str(),&p_end);
        return *p_end=='\0';
}


 
