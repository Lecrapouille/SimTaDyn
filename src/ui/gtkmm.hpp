#ifndef GTKMM_HPP
#define GTKMM_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#  include <gtkmm.h>
#pragma GCC diagnostic pop

namespace Gtk
{

// This will included in 3.24
template<class T, class... T_Args>
T* make_managed(T_Args&&... args)
{
  return manage(new T(std::forward<T_Args>(args)...));
}

}

#endif
