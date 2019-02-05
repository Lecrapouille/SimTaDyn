#ifndef DIALOGEXCEPTION_HPP
#define DIALOGEXCEPTION_HPP

#  include "Gtkmm.tpp"
#  include "Exception.hpp"

void PopupException(Gtk::Window& win, Exception const& e, std::string const& reason);
void PopupException(Gtk::Window& win, std::string const& what, std::string const& message, std::string const& reason);

#endif
