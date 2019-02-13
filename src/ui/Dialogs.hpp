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

  PopupException() {}
  virtual ~PopupException() {}

  void popupException(Exception const& e, std::string const& reason);
  void popupException(std::string const& what, std::string const& message, std::string const& reason);

private:

  virtual Gtk::Window& getRootWindow() = 0;
};

#endif
