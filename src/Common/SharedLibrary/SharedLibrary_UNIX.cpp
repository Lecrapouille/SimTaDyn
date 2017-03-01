// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:     BSL-1.0
//
// NOTE: This file has been adapted for the SimTaDyn project.
// For more informations, see official Poco webpage: https://pocoproject.org/index.html

#include "SharedLibrary_UNIX.hpp"
#include <dlfcn.h>

// Note: cygwin is missing RTLD_LOCAL, set it to 0
#if defined(__CYGWIN__) && !defined(RTLD_LOCAL)
#define RTLD_LOCAL 0
#endif

POCO_IMPLEMENT_EXCEPTION(NotFoundException, simtadyn::Exception, "NotFoundException")
POCO_IMPLEMENT_EXCEPTION(LibraryLoadException, simtadyn::Exception, "LibraryLoadException")
POCO_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, simtadyn::Exception, "LibraryAlreadyLoadedException")

//FastMutex SharedLibraryImpl::_mutex;


SharedLibraryImpl::SharedLibraryImpl()
{
  m_handle = 0;
}


SharedLibraryImpl::~SharedLibraryImpl()
{
}


void SharedLibraryImpl::loadImpl(const std::string& path, int flags)
{
  //FastMutex::ScopedLock lock(_mutex);

  if (m_handle)
    throw LibraryAlreadyLoadedException(path);

  int realFlags = RTLD_LAZY;
  if (flags & SHLIB_LOCAL_IMPL)
    realFlags |= RTLD_LOCAL;
  else
    realFlags |= RTLD_GLOBAL;
  m_handle = dlopen(path.c_str(), realFlags);
  if (!m_handle)
    {
      //std::cerr << dlerror() << "\n";
      const char* err = dlerror();
      throw LibraryLoadException(err ? std::string(err) : path);
    }
  m_path = path;
}


void SharedLibraryImpl::unloadImpl()
{
  //FastMutex::ScopedLock lock(_mutex);

  if (m_handle)
    {
      dlclose(m_handle);
      m_handle = 0;
    }
}


bool SharedLibraryImpl::isLoadedImpl() const
{
  return m_handle != 0;
}


void* SharedLibraryImpl::findSymbolImpl(const std::string& name)
{
  // FastMutex::ScopedLock lock(_mutex);

  void* result = 0;
  if (m_handle)
    {
      result = dlsym(m_handle, name.c_str());
    }
  //std::cerr << dlerror() << "\n";
  return result;
}

const std::string& SharedLibraryImpl::getPathImpl() const
{
  return m_path;
}


std::string SharedLibraryImpl::suffixImpl()
{
  /*#if defined(__APPLE__)
  return ".dylib";
#elif defined(hpux) || defined(_hpux)
  return ".sl";
#elif defined(__CYGWIN__)
  return ".dll";
#else
  return ".so";
  #endif*/
  return ".so";
}
