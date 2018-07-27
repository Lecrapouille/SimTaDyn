// -*- c++ -*- Coloration Syntaxique pour Emacs
#ifndef CLASS_COUNTER_TPP_
#  define CLASS_COUNTER_TPP_

#  include <cstddef>
#  include <atomic>

typedef uint32_t Key;

// *************************************************************************************************
//! \brief This class allows to count the number of created
//! instances. It's safer to use this class than using directly a
//! memeber 'static size_t count;' when a failure occured in creator
//! member the number of instances is correct and also for derived
//! classes.
//!
//! Inspired from: http://www.drdobbs.com/cpp/counting-objects-in-c/184403484
// *************************************************************************************************
template<class T> class InstanceCounter
{
public:

  //! \brief Empty constructor. Increase the number of instances.
  InstanceCounter() { ++m_how_many; }
  //! \brief Constructor by copy. Increase the number of instances.
  InstanceCounter(const InstanceCounter&) { ++m_how_many; }
  //! \brief Destructor. Decrease the number of instances.
  ~InstanceCounter() { --m_how_many; }
  //! \brief Static member. Return the number of instances.
  static Key howMany() { return m_how_many.load(); }

private:

  //! \brief Static member saving the number of instances.
  static std::atomic<Key> m_how_many;
};

template<class T>
std::atomic<Key> InstanceCounter<T>::m_how_many(0U);

// *************************************************************************************************
//! \brief
// *************************************************************************************************
template<class T> class UniqueID
{
public:

  //! \brief Empty constructor. Increase the unique identifier.
  UniqueID() { ++m_id_count; }
  //! \brief Constructor by copy. Increase the unique identifier.
  UniqueID(const UniqueID&) { ++m_id_count; }
  //! \brief Destructor.
  ~UniqueID() { }
  //! \brief Static member. Reset unique identifier.
  static void resetID() { m_id_count = -1U; }
  //! \brief Return the unique identifier.
  static Key getID() { return m_id_count.load(); }

private:

  //! \brief Static member saving the number of instances.
  static std::atomic<Key> m_id_count;
};

template<class T>
std::atomic<Key> UniqueID<T>::m_id_count(0U);

#endif /* CLASS_COUNTER_TPP_ */
