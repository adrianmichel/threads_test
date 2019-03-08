#pragma once

namespace amichel {

/**
generates session stats
*/
class stats {
private:
  std::mutex mx;
  std::vector<int> s;
  // sum of all values counted
  // used for data consistency check
  int sum = 0;

public:
  // will count numbers between 0 and max value
  stats(int max_value) : s(max_value + 1) {}
  virtual ~stats() {}

  // updates stats for current value n
  void count(int n) {
    // throw exception if the received value is higher than expected
    if ((unsigned int)n > s.size())
      throw std::exception("unexpected value");

    // serialize access to stats vector
    std::lock_guard<std::mutex> lock(mx);
    ++s[n];
    sum += n;
  }

  // sends current stats to log
  void log() const {
    amichel::log("**** Stats ****");
    for (size_t n = 0; n < s.size(); ++n) {
      amichel::log(n, ": ", s[n]);
    }

    amichel::log("total count: ", count());
    // statistically speaking the total value should be around max_value / 2 *
    // count
    amichel::log("total value: ", total());
    amichel::log();
  }

  // returns total number of elements received
  int count() const { return std::accumulate(s.begin(), s.end(), 0); }
  int total() const { return sum; }
};

} // namespace amichel
