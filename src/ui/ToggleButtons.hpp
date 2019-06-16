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

#ifndef TOGGLEBUTTONS_HPP
#  define TOGGLEBUTTONS_HPP

#  include "Gtkmm.tpp"

// *************************************************************************************************
//! \brief A set of buttons inserted in a toolbar passed as parameter. Lika Gtk::RadioButton only one
//! button can be selected at once (pressed) other buttons are unpressed.
// *************************************************************************************************
template<class T>
class ToggleButtons
{
private:

  // *************************************************************************************************
  //! \brief Extended Gtk::ToggleToolButton just for storing informations
  // *************************************************************************************************
  class Button: public Gtk::ToggleToolButton
  {
  public:

    //! \brief save the button pressed.
    uint32_t nth_button;
    //! \brief When button is pressed send an event with the
    //! button position.
    void event(uint32_t);
    //! \brief Signal for send the pressed button position.
    sigc::signal<void, T> signal;
  };

public:

  ToggleButtons(Gtk::Toolbar& toolbar)
    : m_toolbar(toolbar)
  {
  }

  //! \brief
  //! \param toolbar the Gtk::Toolbar already created which will
  //! contains buttons.
  ToggleButtons(Gtk::Toolbar& toolbar, const uint32_t max_buttons)
    : m_toolbar(toolbar)
  {
    m_buttons.resize(max_buttons);
  }

  //! \brief add a button
  void append(const T nth_button,
              const Glib::ustring& tooltip,
              const Gtk::BuiltinStockID icon,
              const sigc::slot1<void, T>& toggle_slot)
  {
    Button* button = Gtk::make_managed<Button>();
    button->nth_button = static_cast<int32_t>(nth_button);

    // The first button will have the pressed look
    if (m_toogled_button == button->nth_button)
      {
        button->set_active(true);
      }

    button->set_stock_id(icon);
    button->set_tooltip_text(tooltip);
    button->signal.connect(toggle_slot);

    m_toolbar.append(*button,
                     [this, button]() {
                       ToggleButtons::toggle(*button);
                     });
    m_buttons.push_back(button);
  }

  //! \brief add a separator button
  void append(Gtk::SeparatorToolItem& separator)
  {
    m_toolbar.append(separator);
  }

  //! \brief Return the position of the currently pressed button.
  inline T button() const
  {
    return static_cast<T>(m_toogled_button);
  }

  //! \brief Return the currently pressed button.
  inline Gtk::ToggleToolButton* button(const uint32_t nth_button)
  {
    if (nth_button > m_buttons.size())
      return nullptr;
    return m_buttons[m_toogled_button];
  }

private:

  //! \brief callback when the user pressed on a button.
  //! Unpress the older button and press the new one.
  //! \note: set_active() will preform another toggle()
  //! callback which craps everything. So we made a hack
  //! for breaking circular events.
  void toggle(Button& new_button)
  {
    if (m_toggling == true)
      return ;
    if (new_button.nth_button > m_buttons.size())
      return ;

    m_toggling = true;
    m_buttons[m_toogled_button]->set_active(false);
    m_toogled_button = new_button.nth_button;
    m_buttons[m_toogled_button]->set_active(true);
    m_toggling = false;
    new_button.signal.emit(button());
  }

private:

  //! \brief the currently pressed button
  uint32_t                            m_toogled_button = 0;
  //! \brief Protect against recursive callbacks
  //! when toggling buttons action is in progress.
  bool                                m_toggling = false;
  //! \brief the toolbar which will receive buttons.
  Gtk::Toolbar&                       m_toolbar;
  //! \brief Store buttons. Ok this storage is in double
  //! with the one managed by the toolbar but here there
  //! is no risk of loosing relative position of buttons
  //! in the case where the user has decidde to swap
  //! buttons inside the toolbar.
  std::vector<Button*> m_buttons;
};

#endif // TOGGLEBUTTONS_HPP
