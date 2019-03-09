#include "CppUnitTest.h"
#include "stdafx.h"

using namespace amichel;

const session_context test1 = {"test1", 100000, 9, 20, 20, 20, 1000};
const session_context test2 = {"test2", 1000000, 9, 20, 20, 20, 0};
const session_context test3 = {"test3", 1000000, 9, 100, 2, 20, 0};
const session_context test4 = {"test4", 1000000, 9, 2, 100, 20, 0};

namespace appguard_threads_test {

/**
runs a session the number of times specified in the context
*/
void run(const session_context &context) {
  int run_count = 0;

  amichel::log("*****");
  amichel::log("running context: ", context.name);
  while (++run_count <= context.max_runs) {
    amichel::log("run ", run_count);
    session s(context);
    s.run();
    s.log();


    Assert::IsTrue(s.check_consistency());
    amichel::log();
    amichel::log("run ", run_count, " completed successfully");
  }
}

TEST_CLASS(UnitTest){
public : 
  TEST_METHOD(Test1){
    run(test1);
    amichel::log("all runs completed successfully");
  } // namespace appguard_threads_test

  TEST_METHOD(Test2) {
    run(test2);
    amichel::log("all runs completed successfully");
  }

  TEST_METHOD(Test3) {
    run(test3);
    amichel::log("all runs completed successfully");
  }

    TEST_METHOD(Test4) {
    run(test4);
    amichel::log("all runs completed successfully");
  }
};
}
