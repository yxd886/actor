/******************************************************************************\
 * This example is a very basic, non-interactive math service implemented     *
 * for both the blocking and the event-based API.                             *
\******************************************************************************/

// Manual refs: lines 19-21, 31-65, 67-101, 134-139 (Actor)

#include <iostream>

#include "caf/all.hpp"

using std::endl;
using std::cout;
using namespace caf;

namespace {

using add_atom = atom_constant<atom("add")>;
using sub_atom = atom_constant<atom("sub")>;

using calculator_actor = typed_actor<replies_to<add_atom, int, int>::with<int>,
                                     replies_to<sub_atom, int, int>::with<int>>;

// prototypes and forward declarations
behavior calculator_fun(event_based_actor* self);
void blocking_calculator_fun(blocking_actor* self);
calculator_actor::behavior_type typed_calculator_fun();
class calculator;
class blocking_calculator;
class typed_calculator;

// function-based, dynamically typed, event-based API
behavior calculator_fun(event_based_actor*self ) {
  return behavior{
    [=](add_atom, int a, int b) {
      aout(self)<<"lalala"<<a+b<<endl;
      return a + b;
    },
    [](sub_atom, int a, int b) {
      return a - b;
    }
  };
}

// function-based, dynamically typed, blocking API
void blocking_calculator_fun(blocking_actor* self) {
  bool running = true;
  self->receive_while(running) (
    [](add_atom, int a, int b) {
      return a + b;
    },
    [](sub_atom, int a, int b) {
      return a - b;
    },
    [&](exit_msg& em) {
      if (em.reason) {
        self->fail_state(std::move(em.reason));
        running = false;
      }
    }
  );
}

// function-based, statically typed, event-based API
calculator_actor::behavior_type typed_calculator_fun() {
  return {
    [](add_atom, int a, int b) {
      return a + b;
    },
    [](sub_atom, int a, int b) {
      return a - b;
    }
  };
}

// class-based, dynamically typed, event-based API
class calculator : public event_based_actor {
public:
  calculator(actor_config& cfg) : event_based_actor(cfg) {
    // nop
  }

  behavior make_behavior() override {
    return calculator_fun(this);
  }
};

// class-based, dynamically typed, blocking API
class blocking_calculator : public blocking_actor {
public:
  blocking_calculator(actor_config& cfg) : blocking_actor(cfg) {
    // nop
  }

  void act() override {
    blocking_calculator_fun(this);
  }
};

// class-based, statically typed, event-based API
class typed_calculator : public calculator_actor::base {
public:
  typed_calculator(actor_config& cfg) : calculator_actor::base(cfg) {
    // nop
  }

  behavior_type make_behavior() override {
    return typed_calculator_fun();
  }
};

void tester(scoped_actor&) {
  // end of recursion
}

// tests a calculator instance
template <class Handle, class... Ts>
void tester(scoped_actor& self, const Handle& hdl, int x, int y, Ts&&... xs) {
  auto handle_err = [&](const error& err) {
    aout(self) << "AUT (actor under test) failed: "
               << self->system().render(err) << endl;
    throw std::runtime_error("AUT responded with an error");
  };
  // first test: x + y = z
  self->request(hdl, infinite, add_atom::value, x, y).receive(
    [&](int res1) {
      aout(self) << x << " + " << y << " = " << res1 << endl;
      // second test: x - y = z
      self->request(hdl, infinite, sub_atom::value, x, y).receive(
        [&](int res2) {
          aout(self) << x << " - " << y << " = " << res2 << endl;
        },
        handle_err
      );
    },
    handle_err
  );
  tester(self, std::forward<Ts>(xs)...);
}

void caf_main(actor_system& system) {
  //auto a1 = system.spawn(blocking_calculator_fun);
  auto a2 = system.spawn(calculator_fun);
  auto a3 = system.spawn(typed_calculator_fun);
//  auto a4 = system.spawn<blocking_calculator>();
 // auto a5 = system.spawn<calculator>();
 auto a6 = system.spawn<typed_calculator>();
 auto f = make_function_view(a6);
 
 
 cout<< "5 + 8 ="  <<f (add_atom::value,5,8)<<endl;

 scoped_actor self{system};
 self->request(a2,std::chrono::seconds(10),add_atom::value,4,5).receive(
[&](int a) {
  aout(self)<<"4+5="<<a<<endl;
},
 [&](const error& err) {
      aout(self) << "*** cannot compute " 
                 << system.render(err) << endl;
    }
  );
 anon_send(a2,add_atom::value,5,8);
}


} // namespace <anonymous>

CAF_MAIN()
