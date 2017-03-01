// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:     BSL-1.0
//
// NOTE: This file has been adapted for the SimTaDyn project.
// For more informations, see official Poco webpage: https://pocoproject.org/index.html

#ifndef SHAREDLIBRARY_UNIX_HPP_
#define SHAREDLIBRARY_UNIX_HPP_

#include "Exception.hpp"

POCO_DECLARE_EXCEPTION(NotFoundException, simtadyn::Exception)
POCO_DECLARE_EXCEPTION(LibraryLoadException, simtadyn::Exception)
POCO_DECLARE_EXCEPTION(LibraryAlreadyLoadedException, simtadyn::Exception)

class SharedLibraryImpl
{
 protected:
  enum Flags
  {
    SHLIB_GLOBAL_IMPL = 1,
    SHLIB_LOCAL_IMPL  = 2
  };

  SharedLibraryImpl();
  ~SharedLibraryImpl();
  void loadImpl(const std::string& path, int flags);
  void unloadImpl();
  bool isLoadedImpl() const;
  void* findSymbolImpl(const std::string& name);
  const std::string& getPathImpl() const;
  static std::string suffixImpl();

 private:
  std::string m_path;
  void* m_handle;
  //static FastMutex _mutex;
};

#endif /* SHAREDLIBRARY_UNIX_HPP_ */
