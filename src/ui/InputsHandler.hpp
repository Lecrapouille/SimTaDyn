#ifndef INPUTS_HANDLER_HPP
#define INPUTS_HANDLER_HPP

#include "Gtkmm.tpp"
#include "NonCopyable.hpp"

class MouseHandler: private NonCopyable
{

public:

  MouseHandler() {}
  virtual ~MouseHandler() {}

  void initMouseHandler(Gtk::ApplicationWindow& window)
  {
    window.add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                      Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

    window.signal_button_press_event().connect
      (sigc::mem_fun(*this, &MouseHandler::onMousePressed));
  }


private:

  virtual bool onMousePressed(GdkEventButton* event) = 0;

};

class KeyBoardHandler: private NonCopyable
{
  static constexpr int MAX_KEYS = 0x10000;

public:

  KeyBoardHandler() {}
  virtual ~KeyBoardHandler() {}

  void initKeyBoardHandler(Gtk::ApplicationWindow& window, uint32_t const refresh_period_ms = 10u)
  {
    std::cout << "KeyBoardHandler" << std::endl;
    window.add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

    window.signal_key_press_event().connect_notify
      (sigc::mem_fun(*this, &KeyBoardHandler::onKeyPressed));
    window.signal_key_release_event().connect_notify
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

#endif // INPUTS_HANDLER_HPP
