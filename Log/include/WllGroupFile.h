#ifndef WLL_GROUP_FILE_H
#define WLL_GROUP_FILE_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

//index file format:
//max_file_number=n
//max_file_size=m
//current_file_no=i
//over_write_flag=1/0

//index file were written by OPEN_WRITE AND OPEN_APPEND MODE
//the OPEN_WRITE MODE CREATE INDEX FILE EVERY TIME
//the OPEN_APPEND MODE APPEND WHEN EXIST, CREATE WHEN NOT EXIST


namespace Wll
{
 namespace Tool
 {
                class GroupFile
                {
                public:
   enum FileOpenMode
   {
    OPEN_READ,
    OPEN_WRITE, //TRUNC MODE
    OPEN_APPEND //APPEND MODE
   };//end of enum FileOpenMode
   
   static const int MAX_FILE_NAME_LENGTH=512;
   static const long DEFAULT_MAX_FILE_SIZE=0x4FFFFFFF; //1G
   static const int DEFAULT_MAX_FILE_NUMBER=1024;  //1K
   static const int DEFAULT_MAX_BUFFER_SIZE=4096;  //4K
  public:  
                        //constructor
   GroupFile();
   //filename_template format: "filename_prefix_%n_filename_suffix"= "filename_prefix_1_filename_suffix"... "filename_prefix_n_filename_suffix"
                        GroupFile(const char* filename_template, FileOpenMode open_mode,
                                int max_file_number=DEFAULT_MAX_FILE_NUMBER,
                                long max_file_size=DEFAULT_MAX_FILE_SIZE,
                                int max_buffered_lines=DEFAULT_MAX_BUFFER_SIZE);
                        ~GroupFile();

   //open file
   bool Open(const char* filename_template, FileOpenMode open_mode,
                                int max_file_number=DEFAULT_MAX_FILE_NUMBER,
                                long max_file_size=DEFAULT_MAX_FILE_SIZE,
                                int max_buffer_size=DEFAULT_MAX_BUFFER_SIZE);
   //close
   void Close(void);
   //test file opened
   bool IsOpened(void);
   //configure
   void Config(int max_file_number=DEFAULT_MAX_FILE_NUMBER, long max_file_size=DEFAULT_MAX_FILE_SIZE, int max_buffer_size=DEFAULT_MAX_BUFFER_SIZE);
   //read log data
   size_t Read(const char* buf,size_t size);
                        //write log data
                       size_t Write(const char* buf,size_t size);
      //Flush written data to disk
      bool Flush();                 
  private:
   //open the first file of group
   bool OpenFirstFile(void);
   //open the next file of group
   bool OpenNextFile(void);
   //open the last(final) file of group(for append mode use)
   bool OpenLastFile(void); 
   //get the first file name
   bool GetFirstFileName(char* filename,size_t buf_size);
   //get the next file name
   bool GetNextFileName(char* filename,size_t buf_size);
   //get the next file name
   bool GetLastFileName(char* filename,size_t buf_size);
   //use a index file to record file name/size
   bool GetIndexFileName(char* filename, size_t buf_size); 
   //write info to index file
   bool WriteIndex(void);
   //read info from index file
   bool ReadIndex(void);
   //check whether the filename_template is valid
   bool CheckFileNameTemplate(const char* filename_template)const;
  private:
   int current_file_no;   //current opened file no
   FILE* current_opened_file;  //current opened file pointer
   int max_file_number;   //max file number n(start from 1 to n: when read the number should be the max file number fit the filename_template pattern)
   long max_file_size;   //max file size for each file
   int max_buffer_size;   //max file buffer size for read/write
   char* buffer;    //buffer for read/write
   FileOpenMode open_mode;   //file open mode
   char filename_template[MAX_FILE_NAME_LENGTH]; //file name template : format "<prefix>%d<suffix>"
   bool over_write_flag;   //flag indict whether the group files have been over write
                };//end of class GroupFile
 };//end of namespace Tool
};//namespace Wll

#endif //WLL_GROUP_FILE_H

