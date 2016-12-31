#include "DrawingArea.hpp"

bool GlDrawingArea::keyboard()
{
    Camera2D& camera = Renderer::camera2D();

    if (m_direction[GlDrawingArea::Forward])
    {
      camera.zoomOffset(0.01f);
    }
    if (m_direction[GlDrawingArea::Backward])
    {
      camera.zoomOffset(-0.01f);
    }
    if (m_direction[GlDrawingArea::Up])
    {
      camera.moveOffset(0.0f, -10.0f);
    }
    if (m_direction[GlDrawingArea::Down])
    {
      camera.moveOffset(0.0f, 10.0f);
    }
    if (m_direction[GlDrawingArea::Right])
    {
      camera.moveOffset(10.0f, 0.0f);
    }
    if (m_direction[GlDrawingArea::Left])
    {
      camera.moveOffset(-10.0f, 0.0f);
    }

    // std::cout << camera << std::endl;
    Renderer::applyViewport(camera);
    return true;
}

bool GlDrawingArea::onScrollEvent(GdkEventScroll *event)
{
    Renderer::camera2D().zoomAt(event->x, event->y, event->delta_y);
    Renderer::applyViewport();
    return true;
}
