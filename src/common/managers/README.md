Files:

* Resource.hpp: class created by a ILoader and only managed by a ResourceManager.
  A resource can be shared by several classes and is destroyed when nobody is
  using it.

  class (private) for automaticaly self destruct data when nobody uses
  it. A resource is something loadable from a file and storable to a
  file. Examples: textures, sound tracks.

* ResourceManager.tpp: abstract the life management of its resources
  to the user. The user only get and set resources he
  desired. Resources are free automaticly when no longer need.

* ILoader.hpp: Interface for loading and storing resource in files
  (import/export; open/save). Can throw a LoaderException.

* LoaderManager.tpp: manage a set of loaders depending on the file
  extension.
