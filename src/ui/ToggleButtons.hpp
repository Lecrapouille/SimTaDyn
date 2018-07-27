#ifndef TOGGLEBUTTONS_HPP
#define TOGGLEBUTTONS_HPP

// *************************************************************************************************
//! \brief A set of buttons inserted in a toolbar passed as parameter. Lika Gtk::RadioButton only one
//! button can be selected at once (pressed) other buttons are unpressed.
// *************************************************************************************************
class ToggleButtons
{
private:

  // *************************************************************************************************
  //! \brief Extended Gtk::ToggleToolButton just for storing informations
  // *************************************************************************************************
  class MyToggleToolButton: public Gtk::ToggleToolButton
  {
  public:

    //! \brief save the button pressed.
    uint32_t nth_button;
    //! \brief When button is pressed send an event with the
    //! button position.
    void event(uint32_t);
    //! \brief Signal for send the pressed button position.
    sigc::signal<void, uint32_t> signal;
  };

public:

  ToggleButtons(Gtk::Toolbar& toolbar)
    : m_toolbar(toolbar)
  {
  }

  //! \brief
  //! \param toolbar the Gtk::Toolbar already created which will
  //! contains buttons.
  ToggleButtons(Gtk::Toolbar& toolbar, const uint32_t nb_buttons)
    : m_toolbar(toolbar)
  {
    m_buttons.resize(nb_buttons);
  }

  //! \brief add a button
  void append(const uint32_t nth_button,
              const Glib::ustring& tooltip,
              const Gtk::BuiltinStockID icon,
              const sigc::slot1<void, uint32_t>& toggle_slot)
  {
    MyToggleToolButton* button = Gtk::manage(new MyToggleToolButton());
    button->nth_button = nth_button;

    // The first button will have the pressed look
    if (m_toogled_button == nth_button)
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
  inline uint32_t button() const { return m_toogled_button; }

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
  void toggle(MyToggleToolButton& new_button)
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
    new_button.signal.emit(m_toogled_button);
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
  std::vector<MyToggleToolButton*> m_buttons;
};

#endif
