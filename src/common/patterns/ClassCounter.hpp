#ifndef CLASSCOUNTER_HPP_
#  define CLASSCOUNTER_HPP_

#  include <cstddef>

typedef uint32_t Key;

// *************************************************************************************************
//! \brief This class allows to count the number of created
//! instances. It's safer to use this class than using directly a
//! memeber 'static size_t count;' when a failure occured in creator
//! member the number of instances is correct and also for derived
//! classes.
//!
//! Inspired from: http://www.drdobbs.com/cpp/counting-objects-in-c/
// *************************************************************************************************
template<class T> class ClassCounter // FIXME: not thread safe and separate into two classes
{
public:
  //! \brief Empty constructor. Increase the number of instances.
  ClassCounter() { ++m_how_many; ++m_count; }
  //! \brief Constructor by copy. Increase the number of instances.
  ClassCounter(const ClassCounter&) { ++m_how_many; ++m_count; }
  //! \brief Destructor. Decrease the number of instances.
  ~ClassCounter() { --m_how_many; --m_count; }
  //! \brief Static member. Reset the number of instances.
  static void reset() { m_count = 0U; }
  //!
  static Key count() { return m_count; }
  //! \brief Static member. Return the number of instances.
  static Key howMany() { return m_how_many; }

private:
  //! \brief Static member saving the number of instances.
  static Key m_count;
  static Key m_how_many;
};

template<class T> Key ClassCounter<T>::m_count = 0U;
template<class T> Key ClassCounter<T>::m_how_many = 0U;

#endif /* CLASSCOUNTER_HPP_ */
