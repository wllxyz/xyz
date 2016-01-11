/*------------------------------------------------------*/
/* Logic group files to be considered as one simple file*/
/* to read,write and append                             */
/* WllGroupFile.cpp                                     */
/*                                                      */
/* Copyright (C) WebEx Communications Inc.              */
/* All rights reserved                                  */
/*                                                      */
/* Author                                               */
/*      Shine Wang (shinew@hz.webex.com)                */
/*                                                      */
/* History                                              */
/*    07/30/2010                                        */
/*                                                      */
/*                                                      */
/*------------------------------------------------------*/

#include "WllGroupFile.h"

#include <new>
#include <string.h>
#include <unistd.h>

#include "WllConfigure.h"
#include "WllTrace.h"
using namespace Wll::Tool;
using namespace std;

//constructor
//filename_template format: "filename_prefix_%d_filename_suffix"= "filename_prefix_1_filename_suffix"... "filename_prefix_n_filename_suffix"
GroupFile::GroupFile(void)
{
 this->buffer=NULL;
 this->current_opened_file=NULL;
 this->current_file_no=-1;
 this->over_write_flag=false;
}

GroupFile::GroupFile(const char* filename_template, FileOpenMode open_mode,
 int max_file_number /*=DEFAULT_MAX_FILE_NUMBER*/,
 long max_file_size /*=DEFAULT_MAX_FILE_SIZE*/,
 int max_buffer_size /*=DEFAULT_MAX_BUFFER_SIZE*/ )
{
 this->buffer=NULL;
 this->current_opened_file=NULL;
 this->current_file_no=-1;
 this->over_write_flag=false;
 this->Open(filename_template,open_mode,max_file_number,max_file_size,max_buffer_size);
}

GroupFile::~GroupFile()
{
 this->Close();
}

bool GroupFile::Open(const char* filename_template, FileOpenMode open_mode,
 int max_file_number /*=DEFAULT_MAX_FILE_NUMBER*/,
 long max_file_size /*=DEFAULT_MAX_FILE_SIZE*/,
 int max_buffer_size /*=DEFAULT_MAX_BUFFER_SIZE*/)
{
 //parameters check
 if(max_file_number<0 || max_file_size<=0 || max_buffer_size<=0 || !CheckFileNameTemplate(filename_template))
 {
  DEBUG_LOG("open parameters[max_file_number="<<max_file_number<<",max_file_size="<<max_file_size<<"max_buffer_size="<<max_buffer_size<<"] invalid, open group file ["<<filename_template<<"] failed");
  return false;
 }

 if(this->IsOpened())
 {
  DEBUG_LOG("group file ["<<filename_template<<"] is opened already,close to reopen");
  this->Close();
 }

 strncpy(this->filename_template,filename_template,sizeof(this->filename_template)-1);
 this->filename_template[sizeof(this->filename_template)-1]=0;
 
 this->open_mode=open_mode;
 
 DEBUG_LOG("open_mode=["<<this->open_mode<<"]");

 switch(this->open_mode)
 {
 case OPEN_READ:
  //TODO: TO EXTENDS THE READ MODE
  DEBUG_LOG("open_mode=[READ MODE]");

 case OPEN_WRITE:
  DEBUG_LOG("open_mode=[WRITE MODE]");

  if(!this->OpenFirstFile())
  {
   return false;
  }
  break;

 case OPEN_APPEND:
  DEBUG_LOG("open_mode=[APPEND MODE]");  

  if(!this->OpenLastFile())
  {
   return false;
  }
  break;
 default:
  break;
 }
 
 DEBUG_LOG("configure group file with [max_file_number="<<max_file_number<<",max_file_size="<<max_file_size<<",max_buffer_size="<<max_buffer_size<<"]");
 this->Config(max_file_number,max_file_size,max_buffer_size);
 DEBUG_LOG("configure complete");

 return true;
}

void GroupFile::Close(void)
{
 DEBUG_LOG("check file opened or not");
 if(this->current_opened_file!=NULL)
 {
  DEBUG_LOG("file opened, current_file_no=["<<this->current_file_no<<"]");
  DEBUG_LOG("closing current opened file ...");
  fclose(this->current_opened_file);
  DEBUG_LOG("current opened file closed");
  this->current_opened_file=NULL;
  DEBUG_LOG("writing index file...");
  this->WriteIndex();
  DEBUG_LOG("index file writed");
 }
}


bool GroupFile::IsOpened(void)
{
 return this->current_opened_file!=NULL;
}

//configure
//NOTE: the configure is not threadsafe with Read/Write method, should use mutex lock to keep sync
//the buffer data of read/write maybe lost if multi read/write threads not sync by locks with one config thread
void GroupFile::Config(int max_file_number /*=DEFAULT_MAX_FILE_NUMBER*/, long max_file_size /*=DEFAULT_MAX_FILE_SIZE*/, int max_buffer_size /*=DEFAULT_MAX_BUFFER_SIZE*/)
{
 //parameters check
 if(max_file_number<0 || max_file_size<=0 || max_buffer_size<=0)
 {
  DEBUG_LOG("config parameter [max_file_number="<<max_file_number<<",max_file_size="<<max_file_size<<",max_buffer_size="<<max_buffer_size<<"] invalid");
  return;
 }

 //
 this->max_file_number=max_file_number;
 this->max_file_size=max_file_size;
 
 DEBUG_LOG("config max_buffer_size...");

 if(this->IsOpened())
 {
  DEBUG_LOG("check found group file is opened");

  if(this->max_buffer_size!=max_buffer_size)
  {
   DEBUG_LOG("max_buffer_size["<<max_buffer_size<<"] != this->max_buffer_size["<<this->max_buffer_size<<"], need switch buffer");
   //flush datas
   fflush(this->current_opened_file);
  
   DEBUG_LOG("current opened file's data is flushed");
  
   DEBUG_LOG("malloc buffer memory...");
   //malloc buffer memory
   char* buffer=NULL; 
   if(max_buffer_size!=0)
    buffer=new(nothrow) char[max_buffer_size];
   DEBUG_LOG("buffer memory malloced");
   
   DEBUG_LOG("test malloc success or try to set no buffer: buffer="<<(buffer)<<",max_buffer_size="<<max_buffer_size);
   //test malloc success or try to set no buffer
   if(buffer!=NULL || buffer==NULL && max_buffer_size==0)
   {
    DEBUG_LOG("buffer is not null or try to set no buffer");
    //try to set buffer
    if(setvbuf(this->current_opened_file,buffer,_IOFBF,max_buffer_size)==0)
    {
     DEBUG_LOG("setvbuf successed,try to release old buffer memory...");
     //release old buffer memory
     if(this->buffer!=NULL)
     {
      delete []this->buffer;
      this->buffer=NULL;
      DEBUG_LOG("old buffer memory released");
     }
     //shift to new buffer
     this->buffer=buffer;
     this->max_buffer_size=max_buffer_size;
     DEBUG_LOG("switched to new buffer");
    }
    //set buffer fail,keep the old buffer,clean new buffer
    else
    {
     DEBUG_LOG("setvbuf failed, try to release the new malloced memory");
     if(buffer!=NULL)
     {
      delete []buffer;
      buffer=NULL;
      DEBUG_LOG("buffer memory released");
     }
    }
   }
  }
 }
 else
 {
  DEBUG_LOG("group file is not opened, the buffer not malloc,just set the max_buffer_size");
  this->max_buffer_size=max_buffer_size;
 }
}


//read log data
size_t GroupFile::Read(const char* buf,size_t size)
{
 //TODO:
 return 0;  
}

//write log data
//the write method is not multi-thread-safe
//in the OPEN_WRITE mode, there would occur overwrite error when mulit-thread write without lock and sync
//in the OPEN_APPEND mode, there would occur split message print error and max_file_size exceed config error when message print is oversize of the configed max_file_size
//the OPEN_APPEND mode error maybe tolerant in the case of printing logs
size_t GroupFile::Write(const char* buf,size_t size)
{
 if(this->IsOpened())
 {
  DEBUG_LOG("check found that group file is opened");

  //get the offset of current_opened_file
  off_t offset=ftello(this->current_opened_file);

  DEBUG_LOG("get offset of current data file is [offset="<<offset<<"]");

  DEBUG_LOG("check whether offset["<<offset<<"] +size["<<size<<"] <= max_file_size["<<max_file_size<<"]");
  //if the offset+size <= max_file_size
  if(static_cast<long>(offset+size) <= static_cast<long>(this->max_file_size))
  {
   DEBUG_LOG("this write will not full, direct write size bytes");
   //then write size bytes to the current opened file 
   //return bytes writed
   return fwrite(buf,1,size,this->current_opened_file);
  }
  //else offset+size >max_file_size
  else
  {
   DEBUG_LOG("this write will cause data file full");
   //then write max_file_size-offset bytes to current_opened_file
   size_t writed_bytes=0;

   DEBUG_LOG("test whether the current date file is already full");
   
   if(offset<this->max_file_size)
   {
    DEBUG_LOG("not full: offset["<<offset<<"] < this->max_file_size["<<this->max_file_size<<"]");

    size_t to_writed_bytes=this->max_file_size-offset;
    writed_bytes=fwrite(buf,1,to_writed_bytes,this->current_opened_file);

    if(writed_bytes!=to_writed_bytes)
    {
     DEBUG_LOG("write date error, returned writed_bytes["<<writed_bytes<<"]");
     return writed_bytes;
    }
   }

   DEBUG_LOG("try open the next data file...");
   //close the current file,and open the next file
   if(!this->OpenNextFile())
   {
    //write error log
    DEBUG_LOG("open next date file error,return writed_bytes["<<writed_bytes<<"]");
    //return error
    return writed_bytes;
   }
   DEBUG_LOG("the next data file opend successful");

   DEBUG_LOG("Recurse call Write("<<(buf+writed_bytes)<<","<<(size-(max_file_size-offset))<<")");

   //recurse call the Write(buf+writed,size-(max_file_size-offset))
   size_t n=this->Write(buf+writed_bytes,size-writed_bytes);

   DEBUG_LOG("return total writed bytes["<<(writed_bytes+n)<<"]");
   //return the sum of writed bytes of two Read calls.
   return writed_bytes+n;
  }
 } 
 else
 {
  DEBUG_LOG("group file is not opened, return 0 bytes writed");
  return 0;
 }
}

//open the first file of group
bool GroupFile::OpenFirstFile(void)
{
 char filename[MAX_FILE_NAME_LENGTH]="";
 if(!this->GetFirstFileName(filename,sizeof(filename)))
 {
  DEBUG_LOG("GetFirstFileName failed,OpenFirstFile return false");
  return false;
 }

 DEBUG_LOG( "GetFirstFileName(filename=[" << filename << "])" );

 const char* p_mode_str=NULL;
 switch(this->open_mode)
 {
 case OPEN_READ:
  p_mode_str="r";
  break;
 case OPEN_WRITE:
  p_mode_str="w";
  break;
 case OPEN_APPEND:
  p_mode_str="a";
  break;
 default:
  return false;
 }

 DEBUG_LOG( "open_mode_str=["<<p_mode_str<<"]" );

 this->current_opened_file=fopen(filename,p_mode_str);
 return this->current_opened_file!=NULL;
}

//open the next file of group
bool GroupFile::OpenNextFile(void)
{
 if(this->current_opened_file!=NULL)
 {
  DEBUG_LOG( "closing the last data file..." );
  this->Close();
  DEBUG_LOG( "last data file closed" );
 }

 char filename[MAX_FILE_NAME_LENGTH]="";
 if(!this->GetNextFileName(filename,sizeof(filename)))
 {
  DEBUG_LOG( "GetNextFileName failed, OpenNextFile return false");
  return false;
 }
 DEBUG_LOG( "GetNextFileName success, filename=["<<filename<<"]" );
 
 //this->current_opened_file=fopen(filename,this->open_mode==OPEN_READ?"r":this->open_mode==OPEN_WRITE?"w":"a");

        const char* p_mode_str=NULL;
        switch(this->open_mode)
        {
        case OPEN_READ:
                p_mode_str="r";
                break;
        case OPEN_WRITE:
                p_mode_str="w";
                break;
        case OPEN_APPEND:
                p_mode_str="a";
                break;
        default:
                return false;
        }

 DEBUG_LOG( "check whether open mode ["<<this->open_mode<<"] is OPEN_APPEND" );
 if(this->open_mode==OPEN_APPEND)
 {
  DEBUG_LOG( "this->open_mode["<<this->open_mode<<"] == OPEN_APPEND, truncate file["<<filename<<"]" );
  truncate(filename,0);
  DEBUG_LOG( "next group data file["<<filename<<"] is truncated to be null" );
 }

 DEBUG_LOG( "openning filename ["<<filename<<"] with mode["<<p_mode_str<<"]" );

        this->current_opened_file=fopen(filename,p_mode_str);

 return this->current_opened_file!=NULL;
}

//open the last(final) file of group(for append mode use)
bool GroupFile::OpenLastFile(void)
{
        char filename[MAX_FILE_NAME_LENGTH]="";
        if(!this->GetLastFileName(filename,sizeof(filename)))
        {
  DEBUG_LOG( "GetLastFileName failed, return false" );
                return false;
        }

        const char* p_mode_str=NULL;
        switch(this->open_mode)
        {
        case OPEN_READ:
                p_mode_str="r";
                break;
        case OPEN_WRITE:
                p_mode_str="w";
                break;
        case OPEN_APPEND:
                p_mode_str="a";
                break;
        default:
                return false;
        }

 DEBUG_LOG( "open filename["<<filename<<"] with mode["<<p_mode_str<<"]" );

        this->current_opened_file=fopen(filename,p_mode_str);
        return this->current_opened_file!=NULL;
}
//get the first file name
bool GroupFile::GetFirstFileName(char* filename,size_t buf_size)
{
 this->current_file_no=0;

 DEBUG_LOG( "this->current_file_no=["<<this->current_file_no<<"]" );

 snprintf(filename,buf_size,this->filename_template,this->current_file_no);

 DEBUG_LOG( "filename=["<<filename<<"]" );

 return true; 
}

//get the next file name
bool GroupFile::GetNextFileName(char* filename,size_t buf_size)
{
 this->current_file_no++;

 if(this->max_file_number != 0)
 {
	 if(this->current_file_no>=this->max_file_number)
	 {
	  DEBUG_LOG( "rewind data file index : current_file_no["<<this->current_file_no<<"] >= max_file_number["<<this->max_file_number<<"]" );
	  this->current_file_no=0;
	  this->over_write_flag=true;
	 }
 }
 
 DEBUG_LOG( "this->current_file_no=["<<this->current_file_no<<"]" );

 snprintf(filename,buf_size,this->filename_template,this->current_file_no);

 DEBUG_LOG( "filename=["<<filename<<"]" );
 
 return true; 
}

//get the next file name
bool GroupFile::GetLastFileName(char* filename,size_t buf_size)
{
 if(!this->ReadIndex())
 {
  DEBUG_LOG( "ReadIndex failed, set current_file_no=0" );
  this->current_file_no=0;
 }

 DEBUG_LOG( "current_file_no=["<<this->current_file_no<<"]" );

        snprintf(filename,buf_size,this->filename_template,this->current_file_no);

 DEBUG_LOG( "LastFileName=["<<filename<<"]" );

        return true; 
}

//use a index file to record file name/size
bool GroupFile::GetIndexFileName(char* filename, size_t buf_size)
{
 //replace the "%d" as "%s" in the filename_template
 char tmp_buf[MAX_FILE_NAME_LENGTH]="";
 
 DEBUG_LOG("copying file_template["<<this->filename_template<<"] to tmp_buf");
 strncpy(tmp_buf,this->filename_template,sizeof(tmp_buf)-1);
 tmp_buf[sizeof(tmp_buf)-1]=0;
  
 DEBUG_LOG("tmp_buf=["<<tmp_buf<<"]");
 char* p=strstr(tmp_buf,"%d");
 
 if(p==NULL)
  {
   DEBUG_LOG("not found %d in tmp_buf["<<tmp_buf<<"], return false");
   return false;
  }

 DEBUG_LOG("replace %d as %s in tmp_buf["<<tmp_buf<<"]");
 *(p+1)='s';
 DEBUG_LOG("tmp_buf["<<tmp_buf<<"] is replaced %d with %s");

 DEBUG_LOG("format tmp_buf["<<tmp_buf<<"] with %s replaced as index");
 snprintf(filename,buf_size,tmp_buf,"index");
 DEBUG_LOG("tmp_buf["<<tmp_buf<<"] formated by replace %s as index");
 
 return true;
}

/*

bool GroupFile::WriteIndex(void)
{
 //max_file_size=m
 //max_file_number=n
 //current_file_no=i
 //over_write_flag=0/1
 char index_filename[MAX_FILE_NAME_LENGTH]="";
 if(!this->GetIndexFileName(index_filename,sizeof(index_filename)))
 {
  return false;
 }
 
 FILE* fp=fopen(index_filename,"w");
 if(fp==NULL) return false;
 
 fprintf(fp,"max_file_size=%ld\n", this->max_file_size);
 fprintf(fp,"max_file_number=%d\n",this->max_file_number);
 fprintf(fp,"current_file_no=%d\n",this->current_file_no);
 fprintf(fp,"over_write_flag=%d\n",this->over_write_flag);
 
 fclose(fp);

 return true;
}

*/

bool GroupFile::WriteIndex(void)
{
        //max_file_size=m
        //max_file_number=n
        //current_file_no=i
        //over_write_flag=0/1
        DEBUG_LOG("calling GetIndexFileName to get index file name...");
        char index_filename[MAX_FILE_NAME_LENGTH]="";
        if(!this->GetIndexFileName(index_filename,sizeof(index_filename)))
        {
         DEBUG_LOG("failed to GetIndexFileName, return false");
                return false;
        }
        DEBUG_LOG("index_filename=["<<index_filename<<"]");

        //FILE* fp=fopen(index_filename,"w");
        //if(fp==NULL) return false;

 Configure config;

        //fprintf(fp,"max_file_size=%ld\n", this->max_file_size);
        //fprintf(fp,"max_file_number=%d\n",this->max_file_number);
        //fprintf(fp,"current_file_no=%d\n",this->current_file_no);
        //fprintf(fp,"over_write_flag=%d\n",this->over_write_flag);

 config.Set("","max_file_size",this->max_file_size);
 config.Set("","max_file_number",this->max_file_number);
 config.Set("","current_file_no",this->current_file_no);
 config.Set("","over_write_flag",this->over_write_flag);

        //fclose(fp);
       
        DEBUG_LOG("values before save: max_file_size=[" \
         <<this->max_file_size<<"] max_file_number=[" \
         <<this->max_file_number<<"]current_file_no=[" \
         <<this->current_file_no<<"] over_write_flag=[" \
         <<this->over_write_flag<<"]");
         
        DEBUG_LOG("calling Save the configure...");

        return config.Save(index_filename);
}


/*

bool GroupFile::ReadIndex(void)
{
        //max_file_size=m
        //max_file_number=n
        //current_file_no=i
        //over_write_flag=0/1
        char index_filename[MAX_FILE_NAME_LENGTH]="";
        if(!this->GetIndexFileName(index_filename,sizeof(index_filename)))
        {
                return false;
        }

        FILE* fp=fopen(index_filename,"r");
        if(fp==NULL) return false;

 //fsacnf maybe dangerious!!!
        fscanf(fp,"max_file_size=%ld\n", &this->max_file_size);
        fscanf(fp,"max_file_number=%d\n",&this->max_file_number);
        fscanf(fp,"current_file_no=%d\n",&this->current_file_no);
        fscanf(fp,"over_write_flag=%d\n",&this->over_write_flag);

        fclose(fp);

        return true;
}

*/


bool GroupFile::ReadIndex(void)
{
        //max_file_size=m
        //max_file_number=n
        //current_file_no=i
        //over_write_flag=0/1
        DEBUG_LOG("calling GetIndexFileName to get index file name");
        char index_filename[MAX_FILE_NAME_LENGTH]="";
        if(!this->GetIndexFileName(index_filename,sizeof(index_filename)))
        {
         DEBUG_LOG("GetIndexFileName failed, return false");
                return false;
        }
        DEBUG_LOG("index_filename=["<<index_filename<<"]");

        //FILE* fp=fopen(index_filename,"r");
        //if(fp==NULL) return false;

 Configure config;
 DEBUG_LOG("calling Load to get configs for index_filename["<<index_filename<<"] ...");
 if(!config.Load(index_filename))
  {
   DEBUG_LOG("Load index_filename["<<index_filename<<"] failed, return false");
   return false;
  }

        //fsacnf maybe dangerious!!!
        //fscanf(fp,"max_file_size=%ld\n", &this->max_file_size);
        //fscanf(fp,"max_file_number=%d\n",&this->max_file_number);
        //fscanf(fp,"current_file_no=%d\n",&this->current_file_no);
        //fscanf(fp,"over_write_flag=%d\n",&this->over_write_flag);

 config.Get("","max_file_size",this->max_file_size);
 config.Get("","max_file_number",this->max_file_number);
 config.Get("","current_file_no",this->current_file_no);
 config.Get("","over_write_flag",this->over_write_flag);
 
 DEBUG_LOG("values after get configs from index_filename["<<index_filename<<"]:");
 DEBUG_LOG("max_file_size=["<<this->max_file_size<<"]");
 DEBUG_LOG("max_file_number=["<<this->max_file_number<<"]");
 DEBUG_LOG("current_file_no=["<<this->current_file_no<<"]");
 DEBUG_LOG("over_write_flag=["<<this->over_write_flag<<"]");
        //fclose(fp);

        return true;
}


bool GroupFile::CheckFileNameTemplate(const char* filename_template) const
{
 //try to find %d
 const char* p = strstr(filename_template, "%d");
 if(p==NULL) return false;
 
 //the filename_template only has one %, that's "%d"
 //p=strstr(filename_template,"%");
 //p=strstr(p+1,"%");
 DEBUG_LOG("try to found the first % in filename_template["<<filename_template<<"] ...");
        p=strchr(filename_template,'%');
        DEBUG_LOG("try to found the second % in filename_template["<<filename_template<<"] ...");
        p=strchr(p+1,'%');
        DEBUG_LOG("if can only find the one %d in filename_template["<<filename_template<<"], then it's ok");
 return p==NULL;
}

//Flush written data to disk
bool GroupFile::Flush()
 {
  if(this->IsOpened())
   {
    DEBUG_LOG("GroupFile["<<this->filename_template<<"] is opened, call fflush to flush data to disk...");
    return 0==fflush(this->current_opened_file);
   }
  else
   {
    DEBUG_LOG("GroupFile["<<this->filename_template<<"] is not opened, return false when flush");
    return false;
   }
 }

