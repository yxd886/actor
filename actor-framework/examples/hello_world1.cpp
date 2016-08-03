#include <stdio.h>
#include <fstream>
#include <iostream>
#include <linux/unistd.h>     /* 包含调用 _syscallX 宏等相关信息*/
#include <linux/kernel.h>
#include <unistd.h>


#include "caf/all.hpp"
#include "caf/io/all.hpp"
#define _LINE_LENGTH 300
using std::endl;
using std::string;
using namespace caf;
namespace {

void hello_world(event_based_actor* self, const actor& buddy) {
  // send "Hello World!" to our buddy ...
  self->request(buddy, std::chrono::seconds(10), "Hello World!").then(
    // ... wait up to 10s for a response ...
    [=](const string& what) {
      // ... and print it
      aout(self) << what << endl;
    }
  );
}
struct log_struct{
  int64_t id;
  char host[40];

};

bool GetCpuMem(float &cpu,size_t &mem, int pid,int tid = -1) 
{ 
    bool ret = false; 
    char cmdline[100]; 
    sprintf(cmdline, "ps -o %%cpu,rss,%%mem,pid,tid -mp %d", pid); 
    FILE *file; 
    file = popen(cmdline, "r"); 
    if (file == NULL)  
    { 
        printf("file == NULL\n"); 
        return false; 
    } 
 
    char line[_LINE_LENGTH]; 
    float l_cpuPrec=0; 
    int l_mem=0; 
    float l_memPrec=0; 
    int l_pid=0; 
    int l_tid=0; 
    if (fgets(line, _LINE_LENGTH, file) != NULL)  
    { 
    //  printf("1st line:%s",line); 
        if (fgets(line, _LINE_LENGTH, file) != NULL)  
        { 
    //      printf("2nd line:%s",line); 
            sscanf( line, "%f %d %f %d -", &l_cpuPrec, &l_mem, &l_memPrec, &l_pid ); 
            cpu = l_cpuPrec; 
            mem = l_mem/1024; 
            if( tid == -1 ) 
                ret = true; 
            else 
            { 
                while( fgets(line, _LINE_LENGTH, file) != NULL ) 
                { 
                    sscanf( line, "%f - - - %d", &l_cpuPrec, &l_tid ); 
    //              printf("other line:%s",line); 
    //              cout<<l_cpuPrec<<'\t'<<l_tid<<endl; 
                    if( l_tid == tid ) 
                    { 
                        printf("cpuVal is tid:%d\n",tid); 
                        cpu = l_cpuPrec; 
                        ret = true; 
                        break; 
                    } 
                } 
                if( l_tid != tid ) 
                    printf("TID not exist\n"); 
            } 
        } 
        else 
            printf("PID not exist\n"); 
    } 
    else 
        printf("Command or Parameter wrong\n"); 
    pclose(file); 
    return ret; 
}
string trim(std::string s) {
  auto not_space = [](char c) { return ! isspace(c); };
  // trim left
  s.erase(s.begin(), find_if(s.begin(), s.end(), not_space));
  // trim right
  s.erase(find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
  return s;
}

string getcpu()
{
    int pid = getpid();
    string t;
    t="ps -p "+std::to_string(pid)+" -o %cpu --no-headers"; 
    const char*s = t.c_str();
    FILE *pp = popen(s, "r");
    char tmp[1024]; //设置一个合适的长度，以存储每一行输出
    fgets(tmp, sizeof(tmp), pp);
        if (tmp[strlen(tmp) - 1] == '\n') {
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        }
        string mem(tmp);
        mem=trim(mem);
       
        pclose(pp); //关闭管道
        return mem;
        
    
    
}

behavior mirror(event_based_actor* self) {
  // return the (initial) actor behavior
  return {


};
}


void caf_main(actor_system& system) {
  // our CAF environment
     /* 包含sysinfo结构体信息*/
 
     //if( GetCpuMem( cpu, mem, pid, tid ) ) 
     //   { 
     //       printf("%%CPU:%.1f\tMEM:%dMB\n", cpu, mem); 
     //   } 
 /*   pid = getpid();
    printf("pid=%d\n",pid);
    string t;
    t="ps -p "+std::to_string(pid)+" -o %cpu --no-headers"; 
    std::cout<<t<<endl;
    getchar();
    const char*s = t.c_str();
    FILE *pp = popen(s, "r");
        char tmp[1024]; //设置一个合适的长度，以存储每一行输出
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        if (tmp[strlen(tmp) - 1] == '\n') {
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        }
        string mem(tmp);
        mem=trim(mem);

        std::cout<<stof(mem)<<endl;
    }
    pclose(pp); //关闭管道
    */

std::fstream in;
std::string command = "close";
int argc = 0;
in.open("/home/sunmmer/actor/actor-framework/examples/command.txt");
if (!in)
{
std::cout << "打开文件失败！" << endl;

}
while(!in.eof())
{
in << command <<endl;
std::cout << command << " " << argc << endl;
if(command=="close")
{
    std::cout<<"OK"<<std::endl;
}
}
in.close();


}
}
CAF_MAIN(io::middleman)
