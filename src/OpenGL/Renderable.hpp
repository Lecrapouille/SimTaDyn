#ifndef RENDERABLE_HPP_
#  define RENDERABLE_HPP_

#include <iostream>
#include "Renderer.hpp"

class GLRenderer;

class Renderable
{
  friend class GLRenderer;

public:

  //! \brief Be drawn by the given renderer.
  virtual void drawnBy(GLRenderer& renderer) const = 0;
  virtual ~Renderable() {}
};

#endif /* RENDERABLE_HPP_ */
