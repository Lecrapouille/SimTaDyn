#include "font.h"

static GLuint		base;

bool LoadTGA(TextureImage *texture, char *filename)
{    
     GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
     GLubyte		TGAcompare[12];	
     GLubyte		header[6];
     GLuint		bytesPerPixel;	
     GLuint		imageSize;
     GLuint		temp;
     GLuint		type=GL_RGBA;
     GLuint             i;

     FILE *file = fopen(filename, "rb");
     
     if( file==NULL ||
	 fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||
	 memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0 ||
	 fread(header,1,sizeof(header),file)!=sizeof(header))
     {
	  if (file == NULL)
	       return FALSE;
	  else
	  {
	       fclose(file);
	       return FALSE;
	  }
     }

     texture->width  = header[1] * 256 + header[0];
     texture->height = header[3] * 256 + header[2];
     
     if(	texture->width	<=0	||
		texture->height	<=0	||
		(header[4]!=24 && header[4]!=32))
     {
	  fclose(file);
	  return FALSE;
     }
     
	texture->bpp	= header[4];
	bytesPerPixel	= texture->bpp/8;
	imageSize		= texture->width*texture->height*bytesPerPixel;

	texture->imageData=(GLubyte *)malloc(imageSize);

	if(	texture->imageData==NULL ||
		fread(texture->imageData, 1, imageSize, file)!=imageSize)
	{
	     if(texture->imageData!=NULL)
		  free(texture->imageData);
	     
	     fclose(file);
	     return FALSE;
	}
	
	for (i=0; i<(unsigned) imageSize; i+=bytesPerPixel)
	{
		temp=texture->imageData[i];
		texture->imageData[i] = texture->imageData[i + 2];
		texture->imageData[i + 2] = temp;
	}

	fclose (file);

	glGenTextures(1, &texture[0].texID);

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (texture[0].bpp==24)
		type=GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;
}

GLvoid BuildFont()
{
     int loop;

     base=glGenLists(95);
     glBindTexture(GL_TEXTURE_2D, textures_font[0].texID);
     for (loop=0; loop<95; loop++)
     {
	  float cx = (float) (loop%16)/16.0f;
	  float cy = (float) (loop/16)/8.0f;
	  glNewList(base+loop,GL_COMPILE);
	  glBegin(GL_QUADS);
	  glTexCoord2f(cx,         1.0f-cy-0.120f); glVertex2i(0,0);
	  glTexCoord2f(cx+0.0625f, 1.0f-cy-0.120f); glVertex2i(16,0);
	  glTexCoord2f(cx+0.0625f, 1.0f-cy);		  glVertex2i(16,16);
	  glTexCoord2f(cx,         1.0f-cy);		  glVertex2i(0,16);
	  glEnd();
	  glTranslated(10,0,0);
	  glEndList();
     }
}

GLvoid glPrint2D(GLint x, GLint y, const char *string, ...)
{
     char		text[256];
     va_list		ap;
     
     if (string == NULL)
	  return;	
     
     va_start(ap, string);
     vsprintf(text, string, ap);
     va_end(ap);
     
     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     glOrtho(0,guts.width,0,guts.height,-1,1);
     glMatrixMode(GL_MODELVIEW);	

     glBindTexture(GL_TEXTURE_2D, textures_font[0].texID);
     glPushMatrix();
     glLoadIdentity();
     glTranslated(x,y,0);
     glListBase(base-32);
     glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
     glPopMatrix();

     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
     glMatrixMode(GL_MODELVIEW);
}

GLvoid glPrint3D(GLfloat x, GLfloat y, GLfloat z, GLfloat f, const char *string, ...)
{
     char		text[256];
     va_list		ap;
     
     if (string == NULL)
	  return;	
     
     va_start(ap, string);
     vsprintf(text, string, ap);
     va_end(ap);
     
     glEnable(GL_TEXTURE_2D);
     glEnable(GL_BLEND);
     glBindTexture(GL_TEXTURE_2D, textures_font[0].texID);
     glPushMatrix();
     glLoadIdentity();
     glTranslatef(x,y,z);
     glScaled(f,f,f);
     glListBase(base-32);
     glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
     glPopMatrix();
     glDisable(GL_BLEND);
     glDisable(GL_TEXTURE_2D);
}
