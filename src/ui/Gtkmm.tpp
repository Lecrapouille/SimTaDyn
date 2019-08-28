// -*- c++ -*- Coloration Syntaxique pour Emacs
//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef SIMTADYN_GTKMM_TPP
#  define SIMTADYN_GTKMM_TPP

#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-const-variable"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
#  pragma GCC diagnostic ignored "-Wredundant-decls"
#  pragma GCC diagnostic ignored "-Wdeprecated"
#    include <gtkmm.h>
#  pragma GCC diagnostic pop

//------------------------------------------------------------------
//! \file Gtkmm.hpp is a collection of GTK+ helper routines.
//! Some functions was picked from the Tilix project and has been
//! translated from D to c++: https://github.com/gnunn1/tilix
//------------------------------------------------------------------

namespace Gtk
{

#if !GTK_CHECK_VERSION(3,24,0)
//------------------------------------------------------------------
//! \brief Wrapper for Gtk::manage. This function is included
//! GTK+ version 3.24
//------------------------------------------------------------------
template<class W, class... W_Args>
W* make_managed(W_Args&&... args)
{
  return manage(new W(std::forward<W_Args>(args)...));
}
#endif

}

enum class Pack { First, Second };

//------------------------------------------------------------------
//! \brief Traverse widget parents hierarchy until reach the first
//! widget of type T or the root widget.
//! \return the address of the found widget, else return nullptr.
//------------------------------------------------------------------
template<class W>
W* findParent(Gtk::Widget* widget);

//------------------------------------------------------------------
//! \brief Traverse widget children hierarchy and collect widgets
//! of type T.
//! \param recursive if false, halt on the first depth of hierarchy,
//! else traverse everything.
//------------------------------------------------------------------
template<class W>
std::vector<W*> getChildren(Gtk::Widget* widget, bool const recursive);

//------------------------------------------------------------------
//! \brief Split the widget in 2 parts with a Gtk::Paned. Depending
//! on the parameter orientation, the separation can be horizontal
//! or vertical. The widget newly is attached to the remaining part of
//! the Paned. Depending on the parameter child it can be placed on
//! the left or on the right. Note that a Gtk::Box is inserted before
//! widget in the aim to let GTK+ to compute widgets sizes.
//! \warn the parent of older shall be Gtk::Box.
//------------------------------------------------------------------
template<class W>
void splitWidget(W& older, W& newly, Gtk::Orientation const orientation, Pack const side);

#include "Gtkmm.ipp"

#endif
