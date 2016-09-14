#ifndef DRAWABLE_H_
#  define DRAWABLE_H_

#  include "simtypes.hpp"
#  include "Renderer.hpp"

class Drawable
{
public:
    virtual ~Drawable() {}

protected:
    friend class RenderTarget;
    virtual void draw(Renderer& target, RenderStates states) const = 0;
};

#endif /* DRAWABLE_H_ */
