//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef OBSERVER_HPP_
#  define OBSERVER_HPP_

#  include "Logger.hpp"
#  include <vector>
#  include <algorithm>
#  include <string>

// *************************************************************************************************
//! \file Inspired by http://hillside.net/europlop/HillsideEurope/Papers/EuroPLoP1999/1999_Henney_MutualRegistration.pdf
//! with difference that Observers has multiple Observables.
// *************************************************************************************************

class IEvent
{
protected:

   virtual ~IEvent() {};
};

template <class Event>
class BiDirObserver;

// *************************************************************************************************
//! \brief Bidirectional Observable is the Subject of the BiDirObserver pattern.
//! The difference with original Oberver pattern is Observers has multiple Observables.
// *************************************************************************************************
template <class Event>
class BiDirObservable
{
public:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  BiDirObservable(const char* info)
    : m_info(info)
  {
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual ~BiDirObservable();

  //------------------------------------------------------------------
  //! \brief Return the information about the BiDirObservable.
  //------------------------------------------------------------------
  const std::string& info() const { return m_info; };

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void attachObserver(BiDirObserver<Event>& observer);

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void detachObserver(BiDirObserver<Event>& observer);

  //------------------------------------------------------------------
  //! \brief Notify all observers
  //------------------------------------------------------------------
  void notify(Event& event);

private:

  std::vector<BiDirObserver<Event>*> m_observers;
  std::string m_info;
};

// *************************************************************************************************
//! \brief Observer class where T is
// *************************************************************************************************
template <class Event>
class BiDirObserver
{
public:

  BiDirObserver(const char* info)
    : m_info(info)
  {
  }

  virtual ~BiDirObserver()
  {
    LOGI("BiDirObserver %s destroying", info().c_str());
    for (auto& it: m_observables)
      {
        detachObservable(*it);
      }
  }

  //------------------------------------------------------------------
  //! \brief Return the information about the BiDirObserver.
  //------------------------------------------------------------------
  const std::string& info() const { return m_info; };

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual void update(BiDirObservable<Event>& subject, Event& event) = 0;

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void attachObservable(BiDirObservable<Event>& observable)
  {
    LOGI("BiDirObserver %s attaching the observable %s",
         info().c_str(), observable.info().c_str());

    auto it = std::find(m_observables.begin(), m_observables.end(), &observable);
    if (it == m_observables.end())
      {
        m_observables.push_back(&observable);
        observable.attachObserver(*this);
      }
    else
      {
        LOGI("BiDirObserver %s has already attached the observable %s",
               info().c_str(), observable.info().c_str());
      }
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  bool attached(BiDirObservable<Event>& observable)
  {
    return m_observables.end() != std::find(m_observables.begin(), m_observables.end(), &observable);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void detachObservable(BiDirObservable<Event>& observable)
  {
    LOGI("BiDirObserver %s detaching the observable %s\n",
           info().c_str(), observable.info().c_str());

    auto it = std::find(m_observables.begin(), m_observables.end(), &observable);
    if (it != m_observables.end())
      {
        m_observables.erase(it);
        observable.detachObserver(*this);
      }
  }

private:

  //! \brief
  std::vector<BiDirObservable<Event>*> m_observables;
  std::string m_info;
};

#endif /* OBSERVER_HPP_ */
