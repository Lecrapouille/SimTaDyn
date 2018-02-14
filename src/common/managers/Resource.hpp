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

#ifndef RESOURCE_HPP_
#  define RESOURCE_HPP_

#  include "Logger.hpp"
#  include "NonCopyable.hpp"
#  include <memory>

class Resource;

// **************************************************************
//! \brief
// **************************************************************
using ResourcePtr = std::shared_ptr<Resource>;

// **************************************************************
//! \brief Resource is a class created by a ILoader and stored in
//! a ResourceManager. Other class can access to a Resource from
//! the ResourceManager and acquire and dispose of resources.
//! When no longer used the resource is destroyed by ResourceDeleter.
// **************************************************************
class Resource: private NonCopyable
{
public:

  template <class R>
  static ResourcePtr create();

private:

  template <class R, class T>
  friend class ResourceManager;
  friend class ResourceDeleter;

  // FIXME
  //void *operator new     (size_t sz) { return ::operator new(sz); }
  //void *operator new[]   (size_t sz)  { return ::operator new[](sz); }
  //void  operator delete  (void* o) { ::operator delete(o); }
  //void  operator delete[](void* o) { ::operator delete[](o); }
};

// **************************************************************
//! http://arb.developpez.com/c++/raii/shared_ptr/
//! Class used for destroying a Resource instance and eventually
//! prevent the ResouceManager. This class is used as Deleter for
//! shared_ptr.
// **************************************************************
class ResourceDeleter
{
public:

  ResourceDeleter()
  {
  }

  //! Note: be careful if you need to prevent the ResourceManager.
  //! Indeed because of mutex a dead-lock can occurs.
  void operator()(Resource *resource)
  {
    CPP_LOG(logger::Info) << "ResourceDeleter delete resource\n";
    delete resource;
  }
};

template <class R>
ResourcePtr Resource::create()
{
  return ResourcePtr(new R, ResourceDeleter());
}

#endif
