#pragma once

#include "queue.h"
#include "stats.h"

namespace amichel {
/**
 counting thread processor
 reads the next available element from the queue
 and sends it to the stats class for processing
*/
class counter {
private:
  amichel::queue &queue;
  stats &ss;
  const int mv;
  const int vc;

public:
  /**
  called by the thread to do the work
  */
  void run() {
    // loop as long as the element count is lower than max and
    // or there are still elements in the queue
    for (auto n = queue.pop(); n; n = queue.pop()) {
      // pass the element to the stats object
      ss.count(n.value());
    }
  }

public:
  counter(amichel::queue &q, stats &s, int maxValue, int valueCount)
      : queue(q), mv(maxValue), ss(s), vc(valueCount) {}
  virtual ~counter() {}
};

} // namespace amichel
