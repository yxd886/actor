#include <iostream>
#include <regex>

#include "caf/all.hpp"
using std::endl;
using std::cout;
using std::string;
using namespace caf;
using std::chrono::seconds;
namespace {
  using step_atom = atom_constant<atom("step")>;
struct packet{
    uint64_t ipv6_src_addr_0; //first 64 bits of ipv6 src address
    uint64_t ipv6_src_addr_1; //second 64 bit4s of ipv6 src address
    uint64_t ipv6_dst_addr_0;
    uint64_t ipv6_dst_addr_1;
    uint32_t ipv4_src_addr;   //this field is used to store ipv4 src address
                              //after passing stateful_64
    uint32_t ipv4_dst_addr;   //this field is used to store ipv4 dst address
                              //after passing stateful_64

    uint16_t transport_port;  //transport layer port number

    char payload[128] ;        //packet payload
};
//template <class Inspector>
//typename Inspector::result_type inspect(Inspector& f,packet& x){
 //   return f(meta::type_name("packet"),x.ipv6_src_addr_0,x.ipv6_src_addr_1,
 //       x.ipv6_dst_addr_0,x.ipv6_dst_addr_1,x.ipv4_src_addr,x.ipv4_dst_addr,
  //      x.transport_port,x.payload);
//}
union ip{
    struct {
        uint8_t x3;
        uint8_t x2;
        uint8_t x1;
        uint8_t x0;
    };
   uint32_t ipv4;
};
string ip_tostring(uint32_t i)
{
   union ip *a=new ip;
   a->ipv4 = i;
   std::string t;
   t = t+std::to_string(a->x0)+":"+std::to_string(a->x1)+":"+std::to_string(a->x2)+":"+std::to_string(a->x3);
   return t;


}



bool firewall_rule( string ip)
{
  const std::regex rule1("(\\d{1,3}):(\\d{1,3}):(\\d{1,3}):4");
  
  bool a = std:: regex_match(ip,rule1);
  
  return a;
}
bool firewall_rule( uint32_t i)
{
  
  union ip *a=new ip;
   a->ipv4 = i;
   uint8_t x3=a->x0;
   uint8_t x2=a->x1;
   uint8_t x1=a->x2;
   uint8_t x0=a->x3;
   if(x0==1
    ||x0==2
    ||x0==4
    ||x0==6
    ||x0==12)
   {
      return false;
   }
  
  return true;
}






uint32_t iarray[]={0xc0a80101,0xc0a80102,0xc0a80103,0xc0a80104,0xc0a80105,0xc0a80106
,0xc0a80107,0xc0a80108,0xc0a80109,0xc0a8010a,0xc0a8010b,0xc0a8010c,0xc0a8010d,0xc0a8010e
,0xc0a8010f,0xc0a80110,0xc0a80111};
class firewall;
class stateful_64;
class flow_monitoring;
//behavior firewall_fun(event_based_actor*self ) {
//  return behavior{
  //  [=](uint64_t ppa){
        //.....
  //      aout(self)<<"firewall processing"<<endl; 
        //send(firewall_actor, p);
  //  }
 // };
//}

//behavior stateful_64_fun(event_based_actor*self,const actor& fireware) {
  //return behavior{
  //  [=](uint64_t ppa){
        //.....
    //    aout(self)<<"stateful_64 processing"<<endl; 
   //     self->send(fireware, ppa);
   // }
 // };
//}
//behavior flow_monitoring_fun(event_based_actor*self,const actor& stateful_64) {
 // return behavior{
  //  [=](uint64_t ppa){
  //      packet *pa = reinterpret_cast<packet*>(ppa);
  //      aout(self)<<"flow_monitoring processing"<<endl; 
  //      self->num_byte+=sizeof(pa);
  //      self->num_packet++;
  //      aout(self)<<"packet number:"<<self->num_packet<<endl; 
  //      aout(self)<<"byte number:"<<self->num_byte<<endl; 
  //      self->send(stateful_64, ppa);
 //   }
 // };
//}

class firewall  : public event_based_actor{
public:
    firewall(actor_config& cfg,const int& pass_counter,const int& nopass_counter,const bool& pass): event_based_actor(cfg),
    pass_counter(pass_counter),nopass_counter(nopass_counter),pass(pass){
        //nop
    } //this actor should be created first on the service chain
    behavior make_behavior() override {
        //return firewall_fun(this);
        
        return behavior{
    [=](uint64_t ppa){
        packet *pa = reinterpret_cast<packet*>(ppa); 
        aout(this)<<"firewall processing"<<endl; 

        bool pass =firewall_rule(pa->ipv4_src_addr);
        if(pass)
        {
            pass_counter++;
            pass = true;
            aout(this)<<"packet pass "<<endl<<endl<<endl;
        }
        else
        {
            nopass_counter++;
            pass= false;
            aout(this)<<"packet drop"<<endl<<endl<<endl;
        }
         
        
    }
  };
  }

    int pass_counter;
    int nopass_counter;
    bool pass;
    //std::regex reg;

    
};

class stateful_64 : public event_based_actor{
public:
    stateful_64(actor_config& cfg,const actor& firewall_actor,
        const uint64_t& ipv6_src_addr_0,
        const uint64_t& ipv6_src_addr_1,
        const uint64_t& ipv6_dst_addr_0,
        const uint64_t& ipv6_dst_addr_1,
        const uint32_t& ipv4_src_addr,
        const uint32_t& ipv4_dst_addr ): event_based_actor(cfg),firewall_actor(firewall_actor),ipv6_src_addr_0(ipv6_src_addr_0),
        ipv6_src_addr_1(ipv6_src_addr_1),ipv6_dst_addr_0(ipv6_dst_addr_0),ipv6_dst_addr_1(ipv6_dst_addr_1),
        ipv4_src_addr(ipv4_src_addr),ipv4_dst_addr(ipv4_dst_addr)
    {
        //nop
    } 
    behavior make_behavior() override {
        //return stateful_64_fun(this,firewall_actor);
        return behavior{
    [=](uint64_t ppa){
        packet *pa = reinterpret_cast<packet*>(ppa);
        aout(this)<<"stateful_64 processing"<<endl; 
        this->ipv6_src_addr_0=pa->ipv6_src_addr_0;
        this->ipv6_src_addr_1=pa->ipv6_src_addr_1;
        this->ipv6_dst_addr_1=pa->ipv6_dst_addr_1;
        this->ipv6_dst_addr_0=pa->ipv6_dst_addr_0;
        this->ipv4_src_addr=iarray[(pa->ipv6_src_addr_0+pa->ipv6_src_addr_1)%16];
        this->ipv4_dst_addr=iarray[(pa->ipv6_dst_addr_0+pa->ipv6_dst_addr_1)%16];
        pa->ipv4_src_addr=this->ipv4_src_addr;
        pa->ipv4_dst_addr=this->ipv4_dst_addr;
        aout(this)<<ip_tostring(this->ipv4_src_addr)<<endl; 
        this->send(firewall_actor, ppa);
    }
  };
  }


    actor firewall_actor;
    uint64_t ipv6_src_addr_0; //first 64 bits of ipv6 src address
    uint64_t ipv6_src_addr_1; //second 64 bit4s of ipv6 src address
    uint64_t ipv6_dst_addr_0;
    uint64_t ipv6_dst_addr_1;

    uint32_t ipv4_src_addr;
    uint32_t ipv4_dst_addr;

    //this actor is created after firwall actor

    
};

class flow_monitoring : public event_based_actor{
public:
    flow_monitoring(actor_config& cfg,const actor& stateful_64,const int& num_byte,const int& num_packet): event_based_actor(cfg),stateful_64(stateful_64),
    num_byte(num_byte),num_packet(num_packet) {
    // nop
  } //this actor is created after stateful_64 actor

  behavior make_behavior() override {
    //return flow_monitoring_fun(this,stateful_64);
    return behavior{
    [=](uint64_t ppa){
        packet *pa = reinterpret_cast<packet*>(ppa);
        aout(this)<<"flow_monitoring processing"<<endl; 
        this->num_byte+=sizeof(pa);
        this->num_packet++;
        aout(this)<<"packet number:"<<this->num_packet<<endl; 
        aout(this)<<"byte number:"<<this->num_byte<<endl; 
        this->send(stateful_64, ppa);
    }
  };
  }

    actor stateful_64;
    int num_byte;
    int num_packet;

    
};
//class config:public actor_system_config{
//public:
//    config()
 //   {
 //       add_message_type<packet>("packet");
 //   }
//};
void begin(event_based_actor* self,const actor&flow) {
  // let's get it started
  struct packet *pa = new packet;
  self->send(self, step_atom::value, size_t{1});
  self->become (
    [=](step_atom, size_t step) {
      if (step == 20) {
        cout << endl;
        self->quit();
        return;
      }
      // print given step
    pa->ipv6_src_addr_0=step; 
    pa->ipv6_src_addr_1=step+1; 
    pa->ipv6_dst_addr_0=step+2;
    pa->ipv6_dst_addr_1=step+3;  
    pa->transport_port=8080;  
    strcpy(pa->payload,"hello"); 
  uint64_t ppa=reinterpret_cast<uint64_t>(pa);
  //anon_send(flow,ppa);

  self->send(flow,ppa);
      // animate next step in 2s
      self->delayed_send(self, std::chrono::milliseconds(2000),
                         step_atom::value, step + 1);
    }
  );
}
void caf_main(actor_system& system) {
  
  
  auto fire = system.spawn<firewall>(0,0,true);
  auto stateful = system.spawn<stateful_64>(fire,0,0,0,0,0,0);
  auto flow = system.spawn<flow_monitoring>(stateful,0,0);
  //scoped_actor self{system};
  //struct packet *pa = new packet;
  //for(int i=0;i<20;i++)
  //{
  
  //  pa->ipv6_src_addr_0=i; 
   // pa->ipv6_src_addr_1=i+1; 
   // pa->ipv6_dst_addr_0=i+2;
   // pa->ipv6_dst_addr_1=i+3;  
   // pa->transport_port=8080;  
   // strcpy(pa->payload,"hello"); 
  //uint64_t ppa=reinterpret_cast<uint64_t>(pa);
  //anon_send(flow,ppa);

  //self->delayed_send(flow,seconds(10),ppa);
 // }
 system.spawn(begin,flow);


}

} // namespace <anonymous>

CAF_MAIN()