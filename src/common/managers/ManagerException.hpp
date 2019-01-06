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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef MANAGER_EXCEPTION_HPP_
#  define MANAGER_EXCEPTION_HPP_

#  include "Exception.hpp"

// ***********************************************************************************************
//! \brief This macro will declare a class LoaderException derived from Exception.
// ***********************************************************************************************
DECLARE_EXCEPTION(LoaderException, Exception)

// ***********************************************************************************************
//! \brief This macro will declare a class ResourceManagerException derived from Exception.
// ***********************************************************************************************
DECLARE_EXCEPTION(ResourceManagerException, Exception)

// ***********************************************************************************************
//! \brief This macro will declare a class LoaderManagerException derived from Exception.
// ***********************************************************************************************
DECLARE_EXCEPTION(LoaderManagerException, Exception)

#endif // MANAGER_EXCEPTION_HPP_
