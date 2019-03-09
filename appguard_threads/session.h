#pragma once

#include "counter.h"
#include "generator.h"
#include "queue.h"
#include "stats.h"
#include "threads.h"

namespace amichel {

/**
one session context
contains values that define
various session parameters
*/
struct session_context {
  const char *name;
  const int n;
  const int max_value;
  const int generators;
  const int counters;
  const int max_runs;
  const int max_delay_microsec;
};

/**
one complete session run
*/
class session {
private:
  queue q;
  stats s;

  const session_context &ctx;

  // type aliases
  using generators = threads<generator>;
  using counters = threads<counter>;

public:
  session(const session_context &context)
      : ctx(context), q(context.n, context.max_delay_microsec),
        s(context.max_value) {}

  void run() {
    // create and start counter threads
    counters c(
        [&]() { return std::make_shared<counter>(q, s, ctx.max_value, ctx.n); },
        ctx.counters);

    // create and start generator threads
    generators g(
        [&]() { return std::make_shared<generator>(q, ctx.max_value); },
        ctx.generators);

    // wait for all threads to exit
    g.join();
    c.join();
  }

  /** return true if data is consistent -
  1. the number of generated elements
  and the number of elements received by stats are both equal with the
  number of elements sepcified in the context
  in input is the number
  2. the sum of all elements pushed to the queue should equal sum of all
  elements received by stats

  this is a very basic test
  todo: add more consistency tests
  */
  bool check_consistency() const {
    bool check1 = q.count() == ctx.n && s.count() == ctx.n;
    bool check2 = q.total() == s.total();

    if (!check1) {
      amichel::log("element count is inconsistent - expected: ", ctx.n,
                   ", queue: ", q.count(), ", stats: ", s.count());
      return false;
    } else if (!check2) {
      amichel::log("total value of all elements is inconsistent");
      return false;
    } else {
      return true;
    }
  }

  void log() const {
    q.log();
    s.log();
  }
};
} // namespace amichel
