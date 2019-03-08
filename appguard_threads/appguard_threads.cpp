// appguard_threads.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//

#include "counter.h"
#include "generator.h"
#include "pch.h"
#include "queue.h"
#include "session.h"
#include "stats.h"
#include "threads.h"

using namespace amichel;

/**
a couple of test session contexts, and a "normal one
corresponding to the original requirements
*/

session_context context = {"normal", 100, 9, 2, 2, 1, 0};

/**
runs one session the specified number of times
*/
void run(const session_context &context) {
  amichel::log("*****");
  amichel::log("running context: ", context.name);

  session s(context);

  s.run();
  s.log();

  if (s.check_consistency()) {
    amichel::log();
    amichel::log("run completed successfully");
  } else {
    amichel::log();
    amichel::log("run failed, see log for details ");
    throw std::exception("session failed");
  }
}

int main() {
  try {
    // run all the sessions specified in the context vector
    run(context);
  } catch (std::exception &e) {
    amichel::log("application error, aborting: ", e.what());
  }
}
