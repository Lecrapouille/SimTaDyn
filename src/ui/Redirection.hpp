#ifndef RED_HPP_
#  define RED_HPP_

#  include <cstdint>
#  include <gtkmm/textview.h>

// http://gtk.10911.n7.nabble.com/Redirecting-cout-to-a-TextBuffer-td42437.html
// TODO: set_color() ?
class streamgui : public std::streambuf
{
public:
  explicit streamgui(std::ostream &stream,
                     Glib::RefPtr<Gtk::TextBuffer> destination,
                     const uint32_t buffer_size = 2048U);
  ~streamgui();
  inline void flush()
  {
    sync();
  }

protected:
  inline int sync() override
  {
    m_destination->insert(m_destination->end(), pbase(), pptr());
    setp(m_buffer, m_buffer + m_buffer_size);
    return 0;
  }

  inline int overflow(int ch = traits_type::eof()) override
  {
    const int ret = sync();
    *pptr() = ch;
    pbump(1);
    return ret;
  }

private:
  std::streambuf *m_old_buf;
  std::ostream &m_stream;
  Glib::RefPtr<Gtk::TextBuffer> m_destination;
  char *m_buffer;
  uint32_t m_buffer_size;
};

#endif
