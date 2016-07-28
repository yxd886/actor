#include <string>
#include <iostream>
#include <fstream>

#include "caf/all.hpp"
#include "caf/io/all.hpp"
using std::endl;
using std::string;
#define HEARTBEAT_TIME std::chrono::milliseconds(2000)
#define DELAY_TIME std::chrono::milliseconds(6000)
#define WORKER_INSTANCES 1

using namespace caf;
namespace {
using step_atom = atom_constant<atom("step")>;
using start_atom = atom_constant<atom("start")>;
using heartbeat_atom = atom_constant<atom("heartbeat")>;
using check_atom = atom_constant<atom("check")>;
using reply_atom = atom_constant<atom("reply")>;
using repair_atom = atom_constant<atom("repair")>;

struct log_struct{
  int64_t id;
  char host[40];

};
void do_before_log(int64_t id,string host)
{
   struct log_struct t;
   struct log_struct* p=&t;
   memset(t.host,0,sizeof(t.host));
   strcpy(t.host,host.c_str());
   t.id = id;
   FILE *fp=fopen("/home/sunmmer/actor/actor-framework/examples/before_log","wb");
    if(fp==NULL)
    {
      std::cout<<"open file error"<<endl;
    }
    fwrite(p,sizeof(struct log_struct),1,fp);
    fclose(fp);
}
void do_after_log(int64_t id,string host)
{
  struct log_struct t;
   struct log_struct* p=&t;
   memset(t.host,0,sizeof(t.host));
   strcpy(t.host,host.c_str());
   t.id = id;
   FILE *fp=fopen("/home/sunmmer/actor/actor-framework/examples/after_log","wb");
    if(fp==NULL)
    {
      std::cout<<"open file error"<<endl;
    }
    fwrite(p,sizeof(struct log_struct),1,fp);
    fclose(fp);
}


class master_actor  : public event_based_actor{
public:
    master_actor(actor_config& cfg, const std::map<string,actor>& ssh_actors): event_based_actor(cfg),
   ssh_actors(ssh_actors){
      counter = 0;
    } //this actor should be created first on the service chain
    behavior make_behavior() override {
        //return firewall_fun(this);
        
        return behavior{
    [=](start_atom){
         std::map < string, actor >::iterator iter;
         for (iter=ssh_actors.begin();iter!=ssh_actors.end();iter++) {
      
      std::cout<<"send start cluster message to ssh_actors"<<endl;
      
      for(int i=0;i<WORKER_INSTANCES;i++)
      {
      counter++;
      this->send(iter->second,start_atom::value,iter->first,(int64_t)counter);
      
      do_before_log(counter,iter->first);
      conn_state.insert(std::pair<int64_t,int64_t>(counter,1));
      do_after_log(counter,iter->first);
      
      }
      
      
    
  }
    this->delayed_send(this,DELAY_TIME,check_atom::value);
         
        
    },
    [=](heartbeat_atom,int64_t id,string cpu){
        
         aout(this)<<"receive heartbeat from id:"<<id<<" it's cpu: "<<cpu<<endl;
         std::map < int64_t, int64_t >::iterator it;
      it=conn_state.find(id);
      if(it==conn_state.end())
      {
        aout(this)<<"find unknow id worker,ignore"<<endl;
      }else{
        it->second=0;
      }
        return heartbeat_atom::value;
    },
   [=](check_atom){
        
         std::map < int64_t, int64_t >::iterator it;
         for (it = conn_state.begin(); it != conn_state.end(); it++ ) {
                if(it->second==0)
                {
                  it->second=1;
                }else{

                    aout(this)<<"worker "<<it->first<<" failure,try to restart it!"<<endl;
                    counter ++;
                    do_before_log(counter,ssh_actors.begin()->first);
                    conn_state.insert(std::pair<int64_t,int64_t>(counter,1));
                    do_after_log(counter,ssh_actors.begin()->first);
                    send(ssh_actors.begin()->second,start_atom::value,ssh_actors.begin()->first,counter);
                    conn_state.erase(it);
                     
                }
              
                    
                } 
        this->delayed_send(this,DELAY_TIME,check_atom::value);
    },
    [=](repair_atom,int64_t id){

      conn_state.insert(std::pair<int64_t,int64_t>(id,1));
      counter=(id+1>counter)?id+1:counter;
    }

  };
  }

    std::map<string,actor> ssh_actors;
    int64_t counter;
    std::map<int64_t,int64_t> conn_state;
    std::map<int64_t,string> host_worker;
    //std::regex reg;

    
};
behavior ssh_worker(event_based_actor* self) {
  // return the (initial) actor behavior
  return {
    // a handler for messages containing a single string
    // that replies with a string
    [=](start_atom,string ip,int64_t number) {
      // prints "Hello World!" via aout (thread-safe cout wrapper)
      string t;
      pid_t status;
      t= "ssh sunmmer@"+ip+" nohup /home/sunmmer/actor/build/bin/client -i "+std::to_string(number)+" &";
      const char*a = t.c_str();
      status=std::system(a);
      if (-1 != status&&WIFEXITED(status)&&WEXITSTATUS(status)==0)  
    {  
      //successful
        printf("run shell script successfully!\n");  
    }  
    else  
    {  
      //failure
      aout(self)<<"open failure,try again"<<endl;
      self->delayed_send(self,HEARTBEAT_TIME,start_atom::value,ip,number);
    } 
      // reply "!dlroW olleH"
    }
  };
}


void caf_main(actor_system& system) {
  // Read conf file.
  
  char ip[1000];
  int repair_flag=0;
  std::map<string, actor> ssh_actors;
  std::ifstream fin("../../actor-framework/examples/Master.conf", std::ios::in);
  
  while(fin.getline(ip, sizeof(ip)))
{
  std::cout<<"ip:"<<ip<<endl;
  actor a = system.spawn<detached>(ssh_worker);
  std::cout<<"spawned a ssh_worker_actor"<<endl;
  std::map < string, actor >::iterator iter;
  iter=ssh_actors.find(ip);
  if(iter==ssh_actors.end())
  ssh_actors.insert(std::pair<string,actor>(ip,a));
}
fin.clear();
fin.close();
auto mast_actor=system.spawn<master_actor>(ssh_actors);
std::cout<<"spawned a master_actor"<<endl;

FILE* fq=fopen("/home/sunmmer/actor/actor-framework/examples/before_log","rb");
char ch;
ch = fgetc(fq);
if(ch!=EOF)
{
  std::cout<<"repairing"<<endl;
  repair_flag=1;
  fclose(fq);
FILE* fp=fopen("/home/sunmmer/actor/actor-framework/examples/before_log","rb");
  if(fp==NULL)
  {
     std::cout<<"open file error"<<endl;
  }

struct log_struct t;
struct log_struct* p1=&t;

while(!feof(fp))
{
  fread(p1,sizeof(struct log_struct),1,fp);
  anon_send(mast_actor,repair_atom::value,p1->id);
}
fclose(fp);

}







  // create a new actor that calls 'mirror()'
  // create another actor that calls 'hello_world(mirror_actor)';
  system.middleman().publish(mast_actor, 8888);
  std::cout<<"publish the  master_actor at 8888"<<endl;
  if(repair_flag==0)
  {
    anon_send(mast_actor,start_atom::value);
    std::cout<<"send start cluster message to mast_actor"<<endl;
  }else{
    scoped_actor self{system};
    self->delayed_send(mast_actor,std::chrono::milliseconds(20000),check_atom::value);
  }
  
  
  getchar();
  // system will wait until both actors are destroyed before leaving main
}
}
CAF_MAIN(io::middleman)



