# appguard_threads

## Projects
**appguard_threads** is a Visual Studio 2017 solution for a sample Windows console application that demonstrates the use of various C++ thread primitives and other techniques to achieve the result outlined in the requirements document.

Some of the design and implementation goals:
- modular design, by decoupling invidual different concepts and implementing them in their own classes.
- easy parametrization of different runtime variables such as number of thraeds, number of values, max value, thread delay, or number of repeated runs
- fully portable C++ - all thread primitives are part of the std:: namespace, no Windows specific functions have been used.
- validation using unit test

The solution consists of two projects:
- **appguard_threads**: the main project containing main and all code for the application.
- **appguard_threads_test**: the unit test project that uses the code from **appguard_threads** to run a couple of stress tests.

The **appguard_threads** project contains the following classes
- **queue**: a thread safe wrapper around a std::queue, with cumulative element count limit and allwing synchronization between push and pop (pop will wait if the queue is empty)
- **generator**: class implementing the thread method for the number generator. It pushes these numbers to the queue
- **counter**: class implementing the thread method for the number counter. It reads these numbers from the queue and passes them to the stats class
- **stats**: keeps track of the number of elements with the same value
- **threads**: thread container allowing for easy generation and control of any number of threads of the same type (such as generator or counter)
- **session_context**: contains the parameters that allow configuring a session (see below).
- **session**: class that ties it all together into one run - it instantiates all necessary objects, runs threads, and checks for consistency at the end, all based on the session_context passed at creation.

Additionally it contains the following utility classes:
- **random**: generates a random int between 0 and max, which is passed at construction. Used to generate random numbers by the generator class, and for generating random delay times for tests.
- **log**: diagnostic logging (currently to console only). Using variable number of template arguments allows a very simple implementation for any number of arguments for one log line.

The **main** function runs the *normal* context (as defined in the requirements document) sessions.

## Testing
There is a reasonable amount of logging, although some of it is currently commented out in the code currently to avoid polluting the logs as they generate a large amount of ouptut.

There are four unit tests (or rather integration test) that each run one session repeatedly:
1. runs 20 sessions generating 100,000 values each, with 20 threads for both generators and counters, and random delays between 0 and 1000 microseconds in all threads.
2. runs 20 sessions generating 1,000,000 values each, with 20 threads for both generators and counters, with no thread delays.
3. same as 2, just number of generator and counter threads is set to 100 and 2 respectively
4. same as 2, just number of generator and counter threads is set to 2 and 100 respectively

Tests 3. and 4. are intended to test a high disparity between the number of generator and counter threads.

The test success/failure is determined by:
* obviously not getting any crashes or exceptions, 
* making sure the threads terminate gracefully at the end of each session and of the application
* validating certain assumptions about the behavior of the application and data consistency, in this case:
  * making sure the number of elements generated is the same as the number expected to be generated, and equal to the number of elements received by the stats class.
  * the sum of all numbers generated should be equal to the sum of all numbers received by the stats class
