#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include <stdio.h>

#include <linux/unistd.h>     /* 包含调用 _syscallX 宏等相关信息*/
#include <linux/kernel.h>
using std::endl;
using std::string;
#define CONNECT_PORT 8888
#define CONNECT 1
#define NOT_CONNECT 0

using namespace caf;
namespace {
using step_atom = atom_constant<atom("step")>;
using start_atom = atom_constant<atom("start")>;
using heartbeat_atom = atom_constant<atom("heartbeat")>;
using reconect_atom = atom_constant<atom("reconect")>;
using rebind_atom = atom_constant<atom("rebind")>;

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

class client_actor : public event_based_actor{
public:
    client_actor(actor_config& cfg, const int& id,const uint16_t& port_,const string& host_,const actor& master_actor): event_based_actor(cfg),
   id(id),port_(port_),host_(host_),master_actor(master_actor){

  set_default_handler(skip);
  conn_state=CONNECT;
    }
    client_actor(actor_config& cfg, const int& id,const uint16_t& port_,const string& host_): event_based_actor(cfg),
   id(id),port_(port_),host_(host_),master_actor(unsafe_actor_handle_init){

  set_default_handler(skip);
  conn_state=CONNECT;
    }

     //this actor should be created first on the service chain
    behavior make_behavior() override {
        //return firewall_fun(this);
     // send(this, step_atom::value);
    // philosophers start to think after receiving {think}
     // become(normal_task());
    //  become(keep_behavior, reconnecting());
    return behavior{

      [=](step_atom) {
        if(CONNECT==conn_state)
        {
          aout(this)<<"connection normal"<<endl;
          this->delayed_send(this, std::chrono::milliseconds(2000), step_atom::value);
          aout(this)<<"step message sent"<<endl;
          string cpu=getcpu();
          this->send(master_actor,heartbeat_atom::value,id,cpu);
          aout(this)<<"heartbeat message sent"<<endl;
          conn_state=NOT_CONNECT;
        }else{
          aout(this)<<"try to reconnect"<<endl;
          this->send(this,reconect_atom::value);
          this->delayed_send(this, std::chrono::milliseconds(2000), step_atom::value);
        }

      },
      [=](heartbeat_atom) {
        aout(this)<<"receive heartbeat from master"<<endl;
        conn_state=CONNECT;
        
      },
      [=](reconect_atom) {
        aout(this)<<"reconnecting"<<endl;
        auto mm = system().middleman().actor_handle();
        send(mm, connect_atom::value, host_, port_);
        aout(this)<<"reconnecting message sent  "<<endl;
        
      },
      [=](ok_atom, node_id&, strong_actor_ptr& new_server, std::set<std::string>&) {
        if (! new_server) {
          aout(this) << "*** received invalid remote actor" << endl;
          return;
        }
        aout(this) << "*** connection succeeded" << endl;
        master_actor = actor_cast<actor>(new_server);
        conn_state=CONNECT;
      },
      [=](const error& err) {
        aout(this) << "*** could not connect to master"
                   << " at port " << port_
                   << ": " << system().render(err)
                   << " [try again in 2s]"
                   << endl;
        this->delayed_send(this,std::chrono::milliseconds(2000),reconect_atom::value);
      }
    };
    
  
  
}

/*
  behavior normal_task() {
    send(this,step_atom::value);
    return {
      [=](step_atom) {
        if(1==conn_state)
        {
          aout(this)<<"connection normal"<<endl;
          this->delayed_send(this, std::chrono::milliseconds(2000), step_atom::value);
          aout(this)<<"step message sent"<<endl;
          string cpu=getcpu();
          this->send(master_actor,heartbeat_atom::value,id,cpu);
          aout(this)<<"heartbeat message sent"<<endl;
          conn_state=0;
        }else{
          aout(this)<<"try to reconnect"<<endl;
          become(keep_behavior, reconnecting());
        }

      },
      [=](heartbeat_atom) {
        aout(this)<<"receive heartbeat from master"<<endl;
        conn_state=1;
        
      }
    };
  }


    behavior reconnecting(std::function<void()> continuation = nullptr) {
    using std::chrono::seconds;
    auto mm = system().middleman().actor_handle();
    aout(this)<<"host: "<<host_<<" port: "<<port_<<endl;
    send(mm, connect_atom::value, host_, port_);
    return {
      [=](ok_atom, node_id&, strong_actor_ptr& new_server, std::set<std::string>&) {
        if (! new_server) {
          aout(this) << "*** received invalid remote actor" << endl;
          return;
        }
        aout(this) << "*** connection succeeded, awaiting tasks" << endl;
        master_actor = actor_cast<actor>(new_server);
        // return to previous behavior
        if (continuation) {
          continuation();
        }
        unbecome();
      },
      [=](const error& err) {
        aout(this) << "*** could not connect to " << host_
                   << " at port " << port_
                   << ": " << system().render(err)
                   << " [try again in 3s]"
                   << endl;
        delayed_send(mm, seconds(3), connect_atom::value, host_, port_);
      },
      [=](rebind_atom, string& nhost, uint16_t nport) {
        aout(this) << "*** rebind to " << nhost << ":" << nport << endl;
        using std::swap;
        swap(host_, nhost);
        swap(port_, nport);
        auto send_mm = [=] {
          unbecome();
          send(mm, connect_atom::value, host_, port_);
        };
        // await pending ok/error message first, then send new request to MM
        become(
          keep_behavior,
          [=](ok_atom&, actor_addr&) {
            send_mm();
          },
          [=](const error&) {
            send_mm();
          }
        );
      }
    };
  }
*/
    int64_t id;
    int conn_state;
    uint16_t port_;
    string host_;
    actor master_actor;



    
    //std::regex reg;

    
};

class monitorx  : public event_based_actor{
public:
    monitorx(actor_config& cfg, const int64_t& id,const string& host,const actor& master_actor): event_based_actor(cfg),worker(unsafe_actor_handle_init), 
    id(id),host(host),master_actor(master_actor){
        //nop
    } //this actor should be created first on the service chain
    behavior make_behavior() override {
      auto restart = [=](down_msg&){
        aout(this)<<"received down message"<<endl;
        auto worke_actor=this->spawn<client_actor>(id,CONNECT_PORT,host,master_actor);
        this->monitor(worke_actor);
        this->send(worke_actor,step_atom::value);
        worker = worke_actor;
      };
      //return monitor_fun(this);
      set_down_handler(restart);
      auto worker_actor=this->spawn<client_actor>(id,CONNECT_PORT,host,master_actor);
      this->monitor(worker_actor);
      this->send(worker_actor,step_atom::value);
      worker = worker_actor;
      return behavior{
        [=](uint64_t ppa){
              aout(this)<<endl;
        }     
      };
    }

  

actor worker;
int64_t id;
string host;
actor master_actor;
    //std::regex reg;

    
};

class config : public actor_system_config {
public:
  int64_t id = 0;

  config() {
    opt_group{custom_options_, "global"}
    .add(id, "id,i", "id number");
  }
};



void caf_main(actor_system& system,const config& cfg) {
  // Read conf file.
  
  char ip[1000];
 std::ifstream fin("/home/sunmmer/actor/actor-framework/examples/worker.conf", std::ios::in);
  fin.getline(ip, sizeof(ip));

  std::cout<<"ip:"<<ip<<endl;

  // create a new actor that calls 'mirror()'
  // create another actor that calls 'hello_world(mirror_actor)';

  auto master_actor_ptr=system.middleman().remote_actor(ip, CONNECT_PORT);
  std::cout<<"get remote actor"<<endl;
  auto master_actor=*master_actor_ptr;
   //scoped_actor self{system};
  //auto worker_actor=self->spawn<client_actor>(cfg.id,1,CONNECT_PORT,"localhost",master_actor);
  //self->monitor(worker_actor);
  //self->send(worker_actor,step_atom::value);
  auto monit=system.spawn<monitorx>(cfg.id,ip,master_actor);
  fin.clear();
  fin.close();
  getchar();
  getchar();
  getchar();
  getchar();
  std::cout<<"exit:"<<endl;
  getchar();
  while(1)
    {
       getchar();
    };
  // system will wait until both actors are destroyed before leaving main
}
}
CAF_MAIN(io::middleman)
