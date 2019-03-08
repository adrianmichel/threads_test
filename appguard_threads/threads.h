#pragma once

namespace amichel {

/**
instantiates and starts threads for thread processors types
each thread processor requires a factory for instantiations
and a run method that will do the work
*/
template <typename T> class threads {
private:
  // todo: this could be implemented as a vector of pair< thread, processor >,
  // maybe cleaner
  std::vector<std::shared_ptr<T>> v;
  std::vector<std::thread> ths;

public:
  // takes processor factory which must return a shared ptr of the processor
  // and the number of threads to create
  threads(std::function<std::shared_ptr<T>()> factory, int count) {
    while (--count >= 0) {
      // create a processor instance
      v.push_back(factory());
      // create and start a thread for that processor
      ths.push_back(std::thread(&T::run, *v.back()));
    };
  }

  // wait for all threads to exit
  void join() {
    for (std::thread &th : ths) {
      th.join();
    }
  }
};

} // namespace amichel
