#include "DialogException.hpp"
#include "Logger.hpp"

void PopupException::popupException(Exception const& e, std::string const& reason)
{
  std::string msg(reason);
  if (!reason.empty())
    msg += ". Reason: " + e.message();

  LOGC("%s - %s", e.what(), msg.c_str());

  Gtk::MessageDialog dialog(getRootWindow(), e.what(), false, Gtk::MESSAGE_WARNING);
  dialog.set_secondary_text(msg);
  dialog.run();
}

void PopupException::popupException(std::string const& what,
                                    std::string const& message,
                                    std::string const& reason)
{
  std::string msg(message);
  if (!reason.empty())
    msg += ". Reason: " + reason;

  LOGC("%s - %s", what.c_str(), msg.c_str());

  Gtk::MessageDialog dialog(getRootWindow(), what, false, Gtk::MESSAGE_WARNING);
  dialog.set_secondary_text(msg);
  dialog.run();
}
