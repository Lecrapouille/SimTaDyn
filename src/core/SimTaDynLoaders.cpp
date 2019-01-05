#include "SimTaDynLoaders.hpp"

void LoaderManager::registerAllLoaders()
{
  registerLoader<ShapefileLoader, SimTaDynSheet>("shp");
  registerLoader<SimTaDynFileLoader, SimTaDynMap>("dyn");
  //registerLoader<TextureLoader, GLTexture2D>("bmp:png:jpg:jpeg:tga:dds:psd:hdr");
}
