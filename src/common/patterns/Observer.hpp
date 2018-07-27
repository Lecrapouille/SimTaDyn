#ifndef OBSERVER_HPP_
#  define OBSERVER_HPP_

#  include <vector>

template <typename T, class A> class Observable;

// *************************************************************************************************
// Observer
// *************************************************************************************************
template <class T, class A> class Observer
{
public:
  Observer() { }
  virtual ~Observer() {}
  virtual void update(T& subject, A arg) = 0;

  void attach(Observable<T, A> *new_observed)
  {
    typename std::vector<Observable<T, A> *>::const_iterator it;
    it = std::find(observed_.begin(), observed_.end(), new_observed);
    if (it == observed_.end())
      {
        observed_.push_back(new_observed);
        new_observed->attachObserver(*this);
      }
  }

  void detach(Observable<T, A> *old_observed)
  {
    //std::cout << "    Observer " << this << " detaching observable " << static_cast<T *>(old_observed) << std::endl;

    typename std::vector<Observable<T, A> *>::const_iterator it;
    it = std::find(observed_.begin(), observed_.end(), old_observed);
    if (it != observed_.end())
      {
        //std::cout << "    Observer detached observable\n";
        observed_.erase(it);
        old_observed->detachObserver(*this);
      }
  }

  //protected:
  std::vector<Observable<T, A>*> observed_;
};

// *************************************************************************************************
// Observable
// *************************************************************************************************
template <typename T, class A> class Observable
{
public:
  Observable()
  {
    erased = false;
  }

  virtual ~Observable()
  {
  }

  void attachObserver(Observer<T, A> &observer)
  {
    typename std::vector<Observer<T, A> *>::const_iterator it;
    it = std::find(observers_.begin(), observers_.end(), &observer);
    if (it == observers_.end())
      {
        observers_.push_back(&observer);
        observer.attach(this);
      }
  }

  void detachObserver(Observer<T, A> &observer)
  {
    //std::cout << "detachObserver " << this << " --> " << &observer << std::endl;
    typename std::vector<Observer<T, A> *>::const_iterator it;
    it = std::find(observers_.begin(), observers_.end(), &observer);
    if (it != observers_.end())
      {
        //std::cout << "  Observable " << this << " detaching observer " << static_cast<T *>(*it) << std::endl;
        it_erased = observers_.erase(it);
        erased = true;
        observer.detach(this);
      }
    else
      {
        std::cout << "Not found\n";
      }
  }

  void detachAllObservers()
  {
    std::cout << "Detach all\n";
    observers_.clear();//FIXME
  }

  virtual void notifyObservers(A arg)
  {
    typename std::vector<Observer<T, A> *>::const_iterator it;

    for (it = observers_.begin(); it != observers_.end(); )
      {
        (*it)->update(*(static_cast<T *>(this)), arg);

        // FIXME ugly patch to avoid segfault loop iterator + erase
        if (erased)
          {
            erased = false;
            it = it_erased;
          }
        else
          {
            ++it;
          }
      }
  }

protected:
  std::vector<Observer<T, A> *> observers_; // FIXME containers
  bool erased;
  typename std::vector<Observer<T, A> *>::const_iterator it_erased;
};

#endif /* OBSERVER_HPP_ */
