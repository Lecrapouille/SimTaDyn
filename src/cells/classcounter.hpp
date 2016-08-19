#ifndef CLASSCOUNTER_HPP_
#  define CLASSCOUNTER_HPP_

#  include "simtypes.hpp"

// *************************************************************************************************
// Inspired from: http://www.drdobbs.com/cpp/counting-objects-in-c/
//
// This class allows to count the number of created instances. It's safer to use this class than
// using directly a memeber 'static size_t count;' when a failure occured in creator member and also
// easier for heritage.
// *************************************************************************************************
template<typename T> class ClassCounter
{
public:
  ClassCounter() { ++count_; }
  ClassCounter(const ClassCounter&) { ++count_; }
  ~ClassCounter() { --count_; }

  static size_t howMany() { return count_; }
private:
  static size_t count_;
};

template<typename T> size_t ClassCounter<T>::count_ = 0;

#endif /* CLASSCOUNTER_HPP_ */
