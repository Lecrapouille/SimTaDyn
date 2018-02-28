// -*- c++ -*- Coloration Syntaxique pour Emacs
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

template <class Notifier, class Event>
Observable<Notifier, Event>::~Observable()
{
  LOGI("Observable '%s' destroying", info().c_str());
  for (const auto& it: m_observers)
    {
      LOGI("  ");
      detach(*it);
    }
}

template <class Notifier, class Event>
void Observable<Notifier, Event>::attach(Observer<Notifier, Event>& observer)
{
  LOGI("Observable '%s' attaching the Observer '%s'",
       info().c_str(), observer.info().c_str());

  if (true == observer.attached(*this))
    {
      m_observers.push_back(&observer);
    }
  else
    {
      observer.attachOwner(*this);
    }
}

template <class Notifier, class Event>
void Observable<Notifier, Event>::detach(Observer<Notifier, Event>& observer)
{
  LOGI("Observable '%s' detaching the Observer '%s'",
       info().c_str(), observer.info().c_str());

  if (true == observer.attached(*this))
    {
      observer.detachOwner(*this);
    }
  else
    {
      m_observers.erase(std::remove(std::begin(m_observers),
                                    std::end(m_observers), &observer),
                        std::end(m_observers));
    }
}

template <class Notifier, class Event>
void Observable<Notifier, Event>::notify(Event const& event) const
{
  for (const auto& it: m_observers)
    {
      //assert(nullptr != it);
      it->update(*(static_cast<const Notifier*>(this)), event);
    }
}
