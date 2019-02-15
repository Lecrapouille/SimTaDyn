#ifndef DIALOGEXCEPTION_HPP
#define DIALOGEXCEPTION_HPP

#  include "Gtkmm.tpp"
#  include "Exception.hpp"

// *************************************************************************************************
//! \brief
// *************************************************************************************************
class PopupException
{
public:

  PopupException(Gtk::Window& win)
    : m_win(win)
  {}

  virtual ~PopupException() {}

  void popupException(Exception const& e, std::string const& reason);
  void popupException(std::string const& what, std::string const& message, std::string const& reason);

private:

  Gtk::Window& m_win;
};

#endif
