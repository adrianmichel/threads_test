#pragma once

#include "counter.h"
#include "random.h"

namespace amichel {

/**
generator processor
generates random numbers between 0 and max value and
pushes them to the queue
*/
class generator {
private:
  amichel::queue &queue;
  const int mv;

  // random number generator
  random rnd;

public:
  generator(amichel::queue &q, int maxValue)
      : queue(q), mv(maxValue), rnd(maxValue) {}
  virtual ~generator() {}

  // called by the thread to do the work
  void run() {
    while (!queue.done())
      queue.push(rnd.generate());
  }
};

} // namespace amichel
