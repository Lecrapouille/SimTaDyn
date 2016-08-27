#ifndef OBSERVER_HPP_
#  define OBSERVER_HPP_

template <class T, class A>
class Observer
{
public:
  Observer() {}
  virtual ~Observer() {}
  virtual void update(T& subject, A arg) = 0;
};

template <typename T, class A>
class Observable
{
public:
  Observable()
  {
  }

  virtual ~Observable()
  {
  }

  void attachObserver(Observer<T, A> &observer)
  {
    // Ensure no duplication.
    typename std::vector<Observer<T, A> *>::const_iterator it;

    it = std::find(observers_.begin(), observers_.end(), &observer);
    if (it == observers_.end())
      {
        observers_.push_back(&observer);
      }
  }

  void dettachObserver(Observer<T, A> &observer)
  {
    typename std::vector<Observer<T, A> *>::const_iterator it;

    it = std::find(observers_.begin(), observers_.end(), &observer);
    if (it != observers_.end())
      {
        observers_.erase(it);
      }
  }

  void dettachAllObservers()
  {
    observers_.clear();
  }

  virtual void notifyObservers(A arg)
  {
    typename std::vector<Observer<T, A> *>::const_iterator it;

    for (it = observers_.begin(); it != observers_.end(); it++)
      {
        (*it)->update(*(static_cast<T *>(this)), arg);
      }
  }

protected:
  std::vector<Observer<T, A> *> observers_;
};

#endif /* OBSERVER_HPP_ */
