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

#include <cassert>

//------------------------------------------------------------------
template<class W>
W* findParent(Gtk::Widget* widget)
{
  while (widget != nullptr)
    {
      widget = widget->get_parent();
      W* casted_widget = dynamic_cast<W*>(widget);
      if (nullptr != casted_widget)
        {
          return casted_widget;
        }
    }
  return nullptr;
}

//------------------------------------------------------------------
template<class W>
std::vector<W*> getChildren(Gtk::Widget* widget, bool recursive)
{
  std::vector<W*> result;
  std::vector<Gtk::Widget*> children;

  if (widget == nullptr)
    return result;

  Gtk::Bin* bin = dynamic_cast<Gtk::Bin*>(widget);
  if (bin != nullptr)
    {
      children.push_back(bin->get_child());
    }
  else
    {
      Gtk::Container* container = dynamic_cast<Gtk::Container*>(widget);
      if (container != nullptr)
        {
          children = container->get_children();
        }
    }

  for (auto& child: children)
    {
      W* casted_widget = dynamic_cast<W*>(child);
      if (nullptr != casted_widget)
        {
          result.push_back(casted_widget);
          if (recursive)
            {
              auto res = getChildren<W>(child, recursive);
              result.insert(result.end(), res.begin(), res.end());
            }
        }
    }
  return result;
}

//------------------------------------------------------------------
template<class W>
void splitWidget(W& older, W& newly, Gtk::Orientation const orientation, Pack const side)
{
  Gtk::Widget* w = older.get_parent();
  assert(w != nullptr && "has no parent");

  Gtk::Box* parent = static_cast<Gtk::Box*>(w);
  assert(parent != nullptr && "parent was not a Gtk::Box");

  int height = parent->get_allocated_height();
  int width = parent->get_allocated_width();

  Gtk::Box* b1 = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
  Gtk::Box* b2 = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::ORIENTATION_VERTICAL, 0);
  assert(b1 != nullptr && "Failed creating the right Gtk::Box");
  assert(b2 != nullptr && "Failed creating the left Gtk::Box");

  Gtk::Paned* paned = Gtk::make_managed<Gtk::Paned>(orientation);
  assert(paned != nullptr && "Failed creating the Gtk::Paned");
  paned->pack1(*b1);//, PANED_RESIZE_MODE, PANED_SHRINK_MODE);
  paned->pack2(*b2);//, PANED_RESIZE_MODE, PANED_SHRINK_MODE);

  parent->remove(older);
  parent->show_all();
  if (Pack::First == side)
    {
      b1->add(newly);
      b2->add(older);
    }
  else
    {
      b1->add(older);
      b2->add(newly);
    }

  switch (orientation)
    {
    case Gtk::Orientation::ORIENTATION_HORIZONTAL:
      paned->set_position(width / 2);
      break;
    case Gtk::Orientation::ORIENTATION_VERTICAL:
      paned->set_position(height / 2);
      break;
    }
  parent->add(*paned);
  parent->show_all();
  newly.grab_focus();
}
