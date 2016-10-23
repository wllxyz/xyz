/*------------------------------------------------------*/
/* Reload template Class                                */
/*                                                      */
/* WllReload.cpp                                        */
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

#include "WllReload.h"

#include <iostream>
using namespace std;
#include "WllUnixSocket.h"
#include "WllTrace.h"
//#include "safe_str_lib.h"
using namespace Wll::Tool;


        ReloadServerThread::ReloadServerThread(const char*server_name,RELOAD_METHOD reload_method)
        {
                strncpy(this->server_name,server_name,sizeof(this->server_name)-1);
                this->server_name[sizeof(this->server_name)-1]=0;
                this->reload_method=reload_method;
        }

        void ReloadServerThread::Run(void)
        {
                Wll::System::UnixTcpServer server_socket;

                DEBUG_LOG("openning server_socket ["<<this->server_name<<"]");

                if(server_socket.Open(this->server_name))
                {
                        DEBUG_LOG("open server_socket ["<<this->server_name<<"] success");

                        while(true)
                        {
                                DEBUG_LOG("wait to Accept client connect....");

                                int service_socket=server_socket.Accept();

                                if(service_socket>0)
                                {
                                        DEBUG_LOG("client connect accepted success, open server_socket ["<<service_socket<<"]");
                                       
                                        DEBUG_LOG("reading RELOAD command ...");

                                        char buf[]="RELOAD";
                                        memset(buf,0,sizeof(buf));
                                        if(sizeof(buf)!=read(service_socket,buf,sizeof(buf)))
                                        {
                                                LOCAL_ERROR("read reload command RELOAD error! READ SIZE ERROR");
                                        }

                                        DEBUG_LOG("read reload command RELOAD success");
                                       
                                        DEBUG_LOG("compare reload command with RELOAD string ...");
     					if(strncmp(buf,"RELOAD",sizeof(buf))==0)
                                        {
                                                DEBUG_LOG("check RELOAD command OK!");
                                                DEBUG_LOG("calling registered reload_method ...");
                                                if((*this->reload_method)())
                                                {
                                                    DEBUG_LOG("reload_method return true");
                                                        TERM_INFO("reload success!");
                                                }
                                                else
                                                {
                                                    DEBUG_LOG("reload_method return false");
                                                        LOCAL_ERROR("reload failed!");
                                                }
                                        }
                                        DEBUG_LOG("closing service_socket["<<service_socket<<"]");
                                        close(service_socket);
                                        DEBUG_LOG("service_socket["<<service_socket<<"] closed.");
                                        service_socket=-1;
                                }
                        }
                }
                else
                {
                        LOCAL_ERROR("open reload server ["<<this->server_name<<"] failed!)");
                }
        }


        ReloadClient::ReloadClient(const char* server_name)
        {
                strncpy(this->server_name,server_name,sizeof(this->server_name)-1);
                this->server_name[sizeof(this->server_name)-1]=0;
        }

        bool ReloadClient::Reload(void)
        {
                Wll::System::UnixTcpClient client_socket;
                DEBUG_LOG("openning reload server["<<this->server_name<<"]...");
                int service_socket=client_socket.Open(this->server_name);
                if(service_socket>0)
                {
                    DEBUG_LOG("reload server["<<this->server_name<<"] opened success");
                    
                    DEBUG_LOG("writing RELOAD command ...");
                        char buf[]="RELOAD";
                        if( sizeof(buf)==write(service_socket,buf,sizeof(buf)) )
                         {
                          DEBUG_LOG("write RELOAD command success");
                          return true;
                         }
                        else
                         {
                          DEBUG_LOG("write RELOAD command failed");
                          return false;
                         }
                }
                else
                {
                    DEBUG_LOG("open reload server["<<this->server_name<<"] failed");
                        return false;
                }
        }

