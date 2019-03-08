#pragma once

#include "random.h"

namespace amichel {

/**
thread safe queue class used to communicate data between generator and
counter processors.

it is implementation of the counted_queue_ts "interface"

limits the total number of elements pushed, which when reached will signal
calling threads to stop work
*/
class queue {
private:
  // mutex used to synchronize generators and counters
  mutable std::mutex qm;
  // mutex used to serialize access to queue between different threads
  mutable std::mutex x;
  // condition variable used to synchronize generators and counters
  std::condition_variable empty;

  // random number generator
  random rnd;
  // max delay in microseconds
  const int mdm;

  // sum of all numbers pushed to the queue
  // used for data consistency check
  int sum = 0;

  std::queue<int> q;
  // total number of items pushed to the queue (different from the queue size)
  int ct = 0;
  // max number of items after which the entire processing stops
  int mc;

public:
  /**
  max_delay_microsec parameter is the upper bound of the random delays
  generated inside push and pop (used for testing)
  */
  queue(int max_count, int max_delay_microsec = 0)
      : mc(max_count), rnd(max_delay_microsec), mdm(max_delay_microsec) {}

  // pushes one element "t" into the queue
  bool push(int t) {
    // random delay used for tests
    if (mdm > 0)
      std::this_thread::sleep_for(std::chrono::microseconds(rnd.generate()));

    // serializes access to queue operations
    std::lock_guard<std::mutex> lock(qm);

    if (!done()) {
      {
        std::lock_guard<std::mutex> lck(x);
        q.push(t);
        ++ct;
        sum += t;
        // this just display dots in the console to show progress, one dot per
        // 500 values
        if (ct % 500 == 0)
          std::cout << ".";
      }
      //      log("push - ", "count: ", ct, " size: ", q.size(), ", t: ", t);
      //	  log("notify one");

      // new data has been pushed to queue, so wakes one thread waiting for new
      // data
      empty.notify_one();
      return true;
    } else {
      // this is called when the queue has reached the maximum total number of
      // elements which means that processing is done
      // wakes all waiting threads to be sure they exit upon completion
      //		log("notify all");
      empty.notify_all();
      return false;
    }
  }

  /**
  pops and removes one element from the queue and returns it for processing
  returns an std::optional that is false to signal termination, and has a
  valid when processing is still ongoing

  note: this implementation pops one element at a time per thread
  */
  std::optional<int> pop() {
    // test random delay
    if (mdm > 0)
      std::this_thread::sleep_for(std::chrono::microseconds(rnd.generate()));

    if (!done()) {
      // if not done wait for a data element to become available
      std::unique_lock<std::mutex> lk(x);

      // log("pop waiting");
      empty.wait(lk);
      //  log("pop resuming");
    }

    // serializing underlying queue access
    std::lock_guard<std::mutex> lock(qm);

    // popping the data from the front of the queue
    if (!q.empty()) {
      int n = q.front();
      q.pop();
      //      log("pop - ", "count: ", ct, " size: ", q.size());
      // return the data for processing
      return n;
    } else {
      //     amichel::log("job done");
      // can only get here if it was woken with the queue empty,
      // which means it's time to exit, so signal by returning an
      // invalid value optional
      return std::optional<int>();
    }
  }

  operator bool() const { return ct < mc; }
  bool done() const { return ct >= mc; }

  // sends queue info to log
  void log() const {
    // todo: this should be in log class,
    // hack for now
    std::cout << std::endl;
    amichel::log("*** queue ****");
    amichel::log("count: ", ct);
    amichel::log("size: ", q.size());
    amichel::log();
  }

  int count() const { return ct; }
  int total() const { return sum; }
};

} // namespace amichel
