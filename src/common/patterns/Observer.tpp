// -*- c++ -*- Coloration Syntaxique pour Emacs
//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef OBSERVER_HPP_
#  define OBSERVER_HPP_

#  include "Logger.hpp"
#  include <vector>
#  include <algorithm>
#  include <string>

// *************************************************************************************************
//! \file Inspiration coming from:
//! * http://hillside.net/europlop/HillsideEurope/Papers/EuroPLoP1999/1999_Henney_MutualRegistration.pdf
//! with difference that Observers has multiple Observables.
//! * https://www.codeproject.com/Articles/3267/Implementing-a-Subject-Observer-pattern-with-templ
//! with difference that we use additional template class: the arg passed in the notification.
// *************************************************************************************************

template <class Notifier, class Event>
class Observer;

// *************************************************************************************************
//! \brief Observable class is the Subject class of the Observer pattern.
//! Template: Notifier if the class deriving from Observable that will be passed in the notification.
//! Template: Event: a class to pass for informing observers.
// *************************************************************************************************
template <class Notifier, class Event>
class Observable
{
public:

  //------------------------------------------------------------------
  //! \brief Default constructor. Observable has no name.
  //------------------------------------------------------------------
  Observable()
  {
  }

  //------------------------------------------------------------------
  //! \brief Constructor with a name used in logs
  //------------------------------------------------------------------
  Observable(const char* info)
    : m_info(info)
  {
  }

  //------------------------------------------------------------------
  //! \brief Destructor. Detach all Observers.
  //------------------------------------------------------------------
  virtual ~Observable();

  //------------------------------------------------------------------
  //! \brief Return the information about the Observable.
  //------------------------------------------------------------------
  inline const std::string& info() const { return m_info; };

  //------------------------------------------------------------------
  //! \brief Attach an Observer.
  //------------------------------------------------------------------
  void attach(Observer<Notifier, Event>& observer);

  //------------------------------------------------------------------
  //! \brief Detach an Observer.
  //------------------------------------------------------------------
  void detach(Observer<Notifier, Event>& observer);

  //------------------------------------------------------------------
  //! \brief Notify all observers and pass them a class Event.
  //------------------------------------------------------------------
  void notify(Event const& event) const;

private:

  std::vector<Observer<Notifier, Event>*> m_observers;
  std::string m_info;
};

// *************************************************************************************************
//! \brief Observer will be notified by Observable.
//! Observer class knows which Observable is using it. This is useful for automatic detaching from
//! Observables when the observer is destroyed.
// *************************************************************************************************
template <class Notifier, class Event>
class Observer
{
  friend Observable<Notifier, Event>;

public:

  //------------------------------------------------------------------
  //! \brief Default constructor. Observer has no name.
  //------------------------------------------------------------------
  Observer()
  {
  }

  //------------------------------------------------------------------
  //! \brief Default constructor. Observer has a name for logs
  //------------------------------------------------------------------
  Observer(const char* info)
    : m_info(info)
  {
  }

  //------------------------------------------------------------------
  //! \brief Destructor. Observables are detached.
  //------------------------------------------------------------------
  virtual ~Observer()
  {
    LOGI("Observer '%s' destroying", info().c_str());
    for (auto& it: m_owners)
      {
        detachOwner(*it);
      }
  }

  //------------------------------------------------------------------
  //! \brief Return the information about the Observer.
  //------------------------------------------------------------------
  inline const std::string& info() const { return m_info; };

  //------------------------------------------------------------------
  //! \brief Notification.
  //------------------------------------------------------------------
  virtual void update(Notifier const& subject, Event const& event) const = 0;

private:

  //------------------------------------------------------------------
  //! \brief Attach an Observable
  //------------------------------------------------------------------
  void attachOwner(Observable<Notifier, Event>& observable)
  {
    LOGI("   Observer '%s' saving its owner '%s'",
         info().c_str(), observable.info().c_str());

    const auto it = std::find(m_owners.begin(), m_owners.end(), &observable);
    if (it == m_owners.end())
      {
        m_owners.push_back(&observable);
        observable.attach(*this);
      }
    else
      {
        LOGI("    ==> Observer '%s' has already attached its owner '%s'",
               info().c_str(), observable.info().c_str());
      }
  }

  //------------------------------------------------------------------
  //! \brief Check if an Observable is attached
  //------------------------------------------------------------------
  inline bool attached(Observable<Notifier, Event>& observable) const
  {
    return m_owners.end() != std::find(m_owners.begin(), m_owners.end(), &observable);
  }

  //------------------------------------------------------------------
  //! \brief Detach an Observable
  //------------------------------------------------------------------
  void detachOwner(Observable<Notifier, Event>& observable)
  {
    LOGI("    Observer '%s' removing its owner '%s'",
           info().c_str(), observable.info().c_str());

    const auto it = std::find(m_owners.begin(), m_owners.end(), &observable);
    if (it != m_owners.end())
      {
        m_owners.erase(it);
        observable.detach(*this);
      }
  }

private:

  //! \brief
  std::vector<Observable<Notifier, Event>*> m_owners;
  std::string m_info;
};

#  include "Observer.ipp"

#endif /* OBSERVER_HPP_ */
