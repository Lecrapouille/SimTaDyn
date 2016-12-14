#include "Redirection.hpp"

streamgui::streamgui(std::ostream &stream,
                     Glib::RefPtr<Gtk::TextBuffer> destination,
                     const uint32_t buffer_size)
  : m_stream(stream),
    m_destination(destination),
    m_buffer_size(buffer_size)
{
  // Alloc buffer and set its boundaries
  m_buffer = (char*) malloc(sizeof (char) * buffer_size);
  setp(m_buffer, m_buffer + buffer_size);

  // Save the old buffer stream and redirect the old stream
  m_old_buf = stream.rdbuf();
  stream.rdbuf(this);
}

streamgui::~streamgui()
{
  sync();
  free(m_buffer);
  // Reset to standard output again
  m_stream.rdbuf(m_old_buf);
}
