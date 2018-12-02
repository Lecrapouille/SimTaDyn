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

#ifndef SIMTADYNLOADERS_HPP_
#  define SIMTADYNLOADERS_HPP_

#  include <cstddef>
#  include "YesEngine/GenHierarchies.h"
using namespace Yes;

// *************************************************************************************************
//! Include here all classes derivating from the Resource class that will be loaded
//! and saved in files.
// *************************************************************************************************

#  include "SimTaDynMap.hpp"
//#  include "GLTextures.hpp"

// *************************************************************************************************
//! Define the list of resource that will be used by the ResourceManager and LoaderManagers.
// *************************************************************************************************

typedef TYPELIST_2(SimTaDynMap,
                   SimTaDynSheet) ResourceList;
//FIXME GLTexture2D) ResourceList;

// *************************************************************************************************
//!
// *************************************************************************************************

#  include "ResourceManager.tpp"
#  include "ShapeFileLoader.hpp"
#  include "SimTaDynFileLoader.hpp"
// #  include "TextureFileLoader.hpp"
#  include "LoaderManager.tpp"

#endif
