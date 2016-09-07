#ifndef OBSERVER_HPP_
#  define OBSERVER_HPP_

template <typename T, class A> class Observable;

// *************************************************************************************************
// Observer
// *************************************************************************************************
template <class T, class A> class Observer
{
public:
  Observer() { observed = NULL; }
  virtual ~Observer() {}
  virtual void update(T& subject, A arg) = 0;

  Observable<T, A>*attached() const
  {
    return observed;
  }

  void attach(Observable<T, A> *new_observed)
  {
    if (new_observed != observed)
      {
        detach();

        if (new_observed)
          {
            std::cout << "Observer::attach\n";
            observed = new_observed;
            observed->attachObserver(*this);
          }
      }
  }

  void detach()
  {
    if (observed)
      {
        std::cout << "Observer::detach\n";
        Observable<T, A> *old_observed = observed;
        observed = NULL;
        old_observed->detachObserver(*this);
      }
  }

protected:
  // http://hillside.net/europlop/HillsideEurope/Papers/EuroPLoP1999/1999_Henney_MutualRegistration.pdf
  Observable<T, A>* observed;
};

// *************************************************************************************************
// Observable
// *************************************************************************************************
template <typename T, class A> class Observable
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
    std::cout << "attachObserver " << &observer << "\n";

    if (observer.attached() == this)
      {
        // Ensure no duplication.
        typename std::vector<Observer<T, A> *>::const_iterator it;
        it = std::find(observers_.begin(), observers_.end(), &observer);
        if (it == observers_.end())
          {
            observers_.push_back(&observer);
          }
      }
    else
      {
        std::cout << "Observable::attach\n";
        observer.attach(this);
      }
  }

  void detachObserver(Observer<T, A> &observer)
  {
    if (observer.attached() == this)
      {
        std::cout << "Observable::detach\n";
        observer.detach();
      }
    else
      {
        typename std::vector<Observer<T, A> *>::const_iterator it;
        it = std::find(observers_.begin(), observers_.end(), &observer);
        if (it != observers_.end())
          {
            observers_.erase(it);
          }
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

    for (it = observers_.begin(); it != observers_.end(); it++)
      {
        (*it)->update(*(static_cast<T *>(this)), arg);
      }
  }

protected:
  std::vector<Observer<T, A> *> observers_; // FIXME containers
};

#endif /* OBSERVER_HPP_ */
