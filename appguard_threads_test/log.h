#pragma once

namespace amichel {

extern std::mutex log_mx;

/**
simple thread safe function
for now just sends stuff to cout

format:
[thread_id] t1t2...tn
where  t1, t2, tn are constructor arguments
*/
// takes an arbitrary number of arguments of any
// type supported by count::operator<<
template <typename... T> void log(T... t) {
  std::lock_guard lock(log_mx);

  std::ostringstream s;

  s << "[" << std::this_thread::get_id() << "] ";
  ((s << std::forward<T>(t)), ...);

  Logger::WriteMessage(s.str().c_str());
}
} // namespace amichel
