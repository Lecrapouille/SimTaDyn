#include "BiDirObserver.hpp"

template <class Event>
BiDirObservable<Event>::~BiDirObservable()
{
  LOGI("BiDirObservable '%s' destroying", info().c_str());
  for (auto& it: m_observers)
    {
      LOGI("  ");
      detachObserver(*it);
    }
}

template <class Event>
void BiDirObservable<Event>::attachObserver(BiDirObserver<Event>& observer)
{
  LOGI("BiDirObservable %s attaching the BiDirObserver %s",
       info().c_str(), observer.info().c_str());

  if (true == observer.attached(*this))
    {
      m_observers.push_back(&observer);
    }
  else
    {
      observer.attachObservable(*this);
    }
}

template <class Event>
void BiDirObservable<Event>::detachObserver(BiDirObserver<Event>& observer)
{
  LOGI("BiDirObservable %s detaching the BiDirObserver %s",
       info().c_str(), observer.info().c_str());

  if (true == observer.attached(*this))
    {
      observer.detachObservable(*this);
    }
  else
    {
      m_observers.erase(std::remove(std::begin(m_observers),
                                    std::end(m_observers), &observer),
                        std::end(m_observers));
    }
}

template <class Event>
void BiDirObservable<Event>::notify(Event &event)
{
  for (auto& it: m_observers)
    {
      it->update(*this, event);
    }
}
