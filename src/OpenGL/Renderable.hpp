#ifndef RENDERABLE_HPP_
#  define RENDERABLE_HPP_

#include <iostream>
#include "Renderer.hpp"

class IRenderer;

class IRenderable
{
  friend class IRender;

public:

  //! \brief Be drawn by the given renderer.
  virtual void drawnBy(IRenderer const& renderer) const = 0;

  //! \brief Draw itself. Should be called by a renderer.
  //virtual void draw() const = 0;
};


//! \brief Define a pure virtual class only renderable by OpenGL
//class GLRenderable: public IRenderable
//{
//};

#endif /* RENDERABLE_HPP_ */
