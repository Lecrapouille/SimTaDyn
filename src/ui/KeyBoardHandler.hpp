#ifndef KEYBOARD_HANDLER_HPP
#define KEYBOARD_HANDLER_HPP

#include "Gtkmm.tpp"

class KeyBoardHandler
{
  static constexpr int MAX_KEYS = 0x10000;

public:

  KeyBoardHandler(Gtk::ApplicationWindow& widget, uint32_t const refresh_period_ms = 10u)
  {
    std::cout << "KeyBoardHandler" << std::endl;
    widget.add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

    widget.signal_key_press_event().connect_notify
      (sigc::mem_fun(*this, &KeyBoardHandler::onKeyPressed));
    widget.signal_key_release_event().connect_notify
      (sigc::mem_fun(*this, &KeyBoardHandler::onKeyReleased));

    if (refresh_period_ms > 0)
      {
        Glib::signal_timeout().connect
          (sigc::mem_fun(*this, &KeyBoardHandler::onRefreshKeyboard),
           refresh_period_ms);
      }
  }

  inline bool isKeyPressed(int const keyCode) const
  {
    return (keyCode < MAX_KEYS) ? m_keys[keyCode] : false;
  }

  inline int getLastPressedKey() const
  {
    return m_last;
  }

  inline int consumeLastPressedKey()
  {
    int tmp = m_last;
    m_last = -1;
    return tmp;
  }

private:

  //------------------------------------------------------------------
  //! \brief Asynchronous callback when a keyboard key has been
  //! pressed. We memorize the pressed key. A GTK+ timer will trig
  //! onRefreshKeyboard() which will actions associated to all pressed
  //! keys thanks to .
  //------------------------------------------------------------------
  void onKeyPressed(GdkEventKey* evenement)
  {
    std::cout << "keyPressed" << std::endl;
    int key = evenement->keyval;
    if (key < MAX_KEYS)
      {
        m_keys[key] = true;
        m_last = key;
      }
  }

  //------------------------------------------------------------------
  //! \brief Asynchronous callback when a keyboard key has been released.
  //------------------------------------------------------------------
  inline void onKeyReleased(GdkEventKey* evenement)
  {
    int key = evenement->keyval;
    if (key < MAX_KEYS)
      {
        m_keys[key] = false;
      }
  }

  //------------------------------------------------------------------
  //! \brief Time-triggered callback. Manage pressed keys.
  //------------------------------------------------------------------
  virtual bool onRefreshKeyboard() = 0;

private:

  //! \brief Keyboard pressed keys (true) or released (false).
  bool m_keys[MAX_KEYS] = { false };
  int  m_last = -1;
};

#endif // KEYBOARD_HANDLER_HPP
