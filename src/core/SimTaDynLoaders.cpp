#include "SimTaDynLoaders.hpp"

void LoaderManager::registerAllLoaders()
{
  registerLoader<SimTaDynSheet>(std::make_shared<ShapefileLoader>(), "shp");
  registerLoader<SimTaDynMap>(std::make_shared<SimTaDynFileLoader>(), "dyn");
  //registerLoader<GLTexture2D>(std::make_shared<TextureLoader>(), "bmp:png:jpg:jpeg:tga:dds:psd:hdr");
}
