#include "Texture.hpp"

bool Texture::LoadTGA(std::string const& filename)
{
  GLubyte TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  GLubyte TGAcompare[12];
  GLubyte header[6];
  GLuint  bytesPerPixel;
  GLuint  imageSize;
  GLuint  temp;
  GLuint  type;
  GLubyte *imageData;
  GLuint   bpp;
  GLuint   width;
  GLuint   height;

  imageData = NULL;
  FILE *file = fopen(filename.c_str(), "rb");

  if ((NULL == file) ||
      (fread(TGAcompare, 1, sizeof (TGAcompare), file) != sizeof (TGAcompare)) ||
      (0 != memcmp(TGAheader, TGAcompare, sizeof (TGAheader))) ||
      (fread(header, 1, sizeof (header), file) != sizeof (header)))
    {
      if (file != NULL)
        fclose(file);
      return false;
    }

  width  = header[1] * 256 + header[0];
  height = header[3] * 256 + header[2];

  if ((width <= 0) || (height <= 0) || (header[4] != 24 && header[4] != 32))
    {
      fclose(file);
      return false;
    }

  bpp = header[4];
  bytesPerPixel = bpp / 8;
  imageSize = width * height * bytesPerPixel;
  imageData = (GLubyte *) malloc(imageSize);

  if ((NULL == imageData) ||
      (fread(imageData, 1U, imageSize, file) != imageSize))
    {
      if (NULL != imageData)
        {
          free(imageData);
        }

      fclose(file);
      return false;
    }

  for (GLuint i = 0; i < (GLuint) imageSize; i += bytesPerPixel)
    {
      temp = imageData[i];
      imageData[i] = imageData[i + 2];
      imageData[i + 2] = temp;
    }

  fclose(file);

  glGenTextures(1, &id_);
  check_gl_error();
  glBindTexture(GL_TEXTURE_2D, id_);
  check_gl_error();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  check_gl_error();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  check_gl_error();

  if (24 == bpp)
    type = GL_RGB;
  else
    type = GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
  check_gl_error();

  return true;
}
