#include "WllReload.h"
using namespace Wll::Tool;
#include <iostream>
#include <unistd.h>
using namespace std;

//<usage>WllReloadTool <reload_port>
//send reload command to the <reload_port>
int main(int argc,char** argv)
{
 if(argc!=2)
 {
  cout<<"Usage: "<<argv[0]<<" <reload_port>"<<endl;
  return -1;
 }

 ReloadClient client(argv[1]);
 if(client.Reload())
 {
  cout<<"reload "<<argv[1]<<"command sent"<<endl;
 }
 else
 {
  cout<<"open reload port ["<<argv[1]<<"failed"<<endl;
 }

 return 0;
}
