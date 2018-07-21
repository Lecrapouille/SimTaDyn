//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

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
