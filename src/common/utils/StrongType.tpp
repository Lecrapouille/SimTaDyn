// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef STRONG_TYPE_TPP_
#  define STRONG_TYPE_TPP_

// This is a copy of BOOST_STRONG_TYPEDEF:
// http://www.boost.org/doc/libs/1_46_1/libs/serialization/doc/strong_typedef.html
// http://www.boost.org/doc/libs/1_46_1/boost/strong_typedef.hpp
//
// Note: contrary to strong_typedef.hpp the empty contructor D::D() calls the
// empty constructor of T. For more information see:
// https://svn.boost.org/trac10/ticket/5182

class empty_base {};

template <class T, class U, class B = empty_base>
struct less_than_comparable2 : B
{
     friend bool operator<=(const T& x, const U& y) { return !(x > y); }
     friend bool operator>=(const T& x, const U& y) { return !(x < y); }
     friend bool operator>(const U& x, const T& y)  { return y < x; }
     friend bool operator<(const U& x, const T& y)  { return y > x; }
     friend bool operator<=(const U& x, const T& y) { return !(y < x); }
     friend bool operator>=(const U& x, const T& y) { return !(y > x); }
};

template <class T, class B = empty_base>
struct less_than_comparable1 : B
{
     friend bool operator>(const T& x, const T& y)  { return y < x; }
     friend bool operator<=(const T& x, const T& y) { return !(y < x); }
     friend bool operator>=(const T& x, const T& y) { return !(x < y); }
};

template <class T, class U, class B = empty_base>
struct equality_comparable2 : B
{
     friend bool operator==(const U& y, const T& x) { return x == y; }
     friend bool operator!=(const U& y, const T& x) { return !(x == y); }
     friend bool operator!=(const T& y, const U& x) { return !(y == x); }
};

template <class T, class B = empty_base>
struct equality_comparable1 : B
{
     friend bool operator!=(const T& x, const T& y) { return !(x == y); }
};

template <class T, class B = empty_base>
struct totally_ordered1
  : less_than_comparable1<T, equality_comparable1<T, B>>
{
};

template <class T, class U, class B = empty_base>
struct totally_ordered2
  : less_than_comparable2<T, U, equality_comparable2<T, U, B>>
{
};

#define STRONG_TYPEDEF(T, D)                                        \
  struct D                                                          \
    : totally_ordered1<D, totally_ordered2<D, T>>                   \
  {                                                                 \
    T t;                                                            \
    /*explicit*/ D(const T t_) : t(t_) {};                          \
    D() : t() {};                                                   \
    D(const D & t_) : t(t_.t){}                                     \
    D & operator=(const D & rhs) { t = rhs.t; return *this;}        \
    D & operator=(const T & rhs) { t = rhs; return *this;}          \
    operator const T & () const { return t; }                       \
    operator T & () { return t; }                                   \
    bool operator==(const D & rhs) const { return t == rhs.t; }     \
    bool operator<(const D & rhs) const { return t < rhs.t; }       \
  };

#endif /* STRONG_TYPE_TPP_ */
