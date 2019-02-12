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

// *************************************************************************************************
//! \brief
// *************************************************************************************************
template <class L>
bool dialogLoad(std::string const& title, std::string& filename)
{
#if 0
  Gtk::FileChooserDialog dialog(title, Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(getRootWindow());
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Open the dialog window and set the SimTaDyn path as current
  // folder instead of using the "smart-current-folder" strategy
  // thiugh by GTK+ developpers.
  dialog.set_current_folder(config::data_path);

  // Fill filters for selecting type of file. Use the loader manager
  // for filling these filters because its knows all loaders which
  // know file extensions they can load.
  for (auto const& it: LoaderManager::instance().loaders<L>())
    {
      auto filter = Gtk::FileFilter::create();
      // loaders() return a map <<file extension>, <loader>>
      std::string extension("*." + it.first);
      filter->add_pattern(extension);
      filter->set_name(it.second->description() + " (" + extension + ')');
      dialog.add_filter(filter);
    }

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  bool res = (Gtk::RESPONSE_OK == dialog.run());
  filename = dialog.get_filename();

  return res;
#endif
  return false;
}

#endif
