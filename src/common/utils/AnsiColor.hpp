//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef TEXTCOLOR_HPP_
#  define TEXTCOLOR_HPP_

#include <iostream>

class TextColor
{
public:
  virtual TextColor *clone() const = 0;
  virtual ~TextColor() { };
  virtual void normal() = 0;
  virtual void red() = 0;
  virtual void green() = 0;
  virtual void blue() = 0;
  virtual void yellow() = 0;
  virtual void azul() = 0;
  virtual void grey() = 0;
  virtual void white() = 0;

protected:
  virtual void display(std::ostream &os) const = 0;
  friend std::ostream& operator<<(std::ostream& os, const TextColor& c);
};

inline std::ostream& operator<<(std::ostream& os, const TextColor& c)
{
  c.display(os);
  return os;
}


class NoColor: public TextColor
{
public:
  ~NoColor() { }
  inline virtual TextColor *clone() const override
  {
    return new NoColor(*this);
  }
  inline void normal() override { }
  inline void red() override  { }
  inline void green() override { }
  inline void blue() override { }
  inline void yellow() override { }
  inline void azul() override { }
  inline void grey() override { }
  inline void white() override { }

protected:
  inline void display(std::ostream &os) const override
  {
    os << "";
  }
};

class PosixColor: public TextColor
{
public:
  ~PosixColor() { };
  inline virtual TextColor *clone() const override
  {
    return new PosixColor(*this);
  }
  inline void normal() override { m_color = "\033[0;00m"; }
  inline void red() override { m_color = "\033[1;31m"; }
  inline void green() override { m_color = "\033[1;32m"; }
  inline void blue() override { m_color = "\033[1;34m"; }
  inline void yellow() override { m_color = "\033[1;33m"; }
  inline void azul() override { m_color = "\033[0;36m"; }
  inline void grey() override { m_color = "\033[0;37m"; }
  inline void white() override { m_color = "\033[1;37m"; }

protected:
  inline void display(std::ostream &os) const override
  {
    os << m_color;
  }

  char const *m_color;
};

#endif /* TEXTCOLOR_HPP_ */
