// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:     BSL-1.0
//
// NOTE: This file has been adapted for the SimTaDyn project.
// For more informations, see official Poco webpage: https://pocoproject.org/index.html


#include "SharedLibrary.hpp"

SharedLibrary::SharedLibrary()
{
}


SharedLibrary::SharedLibrary(const std::string& path)
{
  loadImpl(path, 0);
}


SharedLibrary::SharedLibrary(const std::string& path, int flags)
{
  loadImpl(path, flags);
}


SharedLibrary::~SharedLibrary()
{
}


void SharedLibrary::load(const std::string& path)
{
  loadImpl(path, 0);
}


void SharedLibrary::load(const std::string& path, int flags)
{
  loadImpl(path, flags);
}


void SharedLibrary::unload()
{
  unloadImpl();
}


bool SharedLibrary::isLoaded() const
{
  return isLoadedImpl();
}


bool SharedLibrary::hasSymbol(const std::string& name)
{
  return findSymbolImpl(name) != 0;
}


void* SharedLibrary::getSymbol(const std::string& name)
{
  void* result = findSymbolImpl(name);
  if (result)
    return result;
  else
    throw NotFoundException(name);
}


const std::string& SharedLibrary::getPath() const
{
  return getPathImpl();
}


std::string SharedLibrary::suffix()
{
  return suffixImpl();
}
