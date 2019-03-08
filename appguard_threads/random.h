#pragma once

namespace amichel {

/**
random integer generator between 0 and max
*/
class random {
private:
  //	static const std::random_device seeder;
  std::mt19937 rng;
  std::uniform_int_distribution<int> dist;

public:
  random(int max) : dist(0, max), rng((unsigned int)time(0)) {}
  ~random() {}

  int generate() { return dist(rng); }
};
} // namespace amichel
