#ifndef DIALOGEXCEPTION_HPP
#define DIALOGEXCEPTION_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#  include <gtkmm.h>
#pragma GCC diagnostic pop

#  include "Exception.hpp"

void PopupException(Gtk::Window& win, Exception const& e, std::string const& reason);
void PopupException(Gtk::Window& win, std::string const& what, std::string const& message, std::string const& reason);

#endif
