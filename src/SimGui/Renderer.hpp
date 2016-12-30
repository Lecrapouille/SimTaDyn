#ifndef RENDERER_HPP_
#  define RENDERER_HPP_

#  include "Camera2D.hpp"
#  include "Fonts.hpp"
#  include "SimTaDynGraphs.hpp"
#  include "RTree.hpp" // a inclure dans SimTaDynGraphs.hpp

/*
 * OpenGL renderer.
 */
class Renderer
{
public:

  enum RenderStyle { Fast2D, Medium2D, Low2D };

  Renderer()
    : m_default_camera(),
      m_current_camera(),
      m_background_color(Color::Black),
      m_render_style(Renderer::Fast2D),
      m_RotationAngles(N_AXIS, 0.0f)
  {
  }

  ~Renderer()
  {
  }

  /*
   * Same role than a constructor but here we are sure than
   * and OpenGL context is created before.
   */
  void start();
  void end();

  /*
   * When camera states change, apply
   * these changement to the screen.
   */
  void applyViewport(Camera2D& camera) // FIXME: pas de const a cause de getTransform
  {
    (void) camera;
    //std::cerr << "applyViewport A refaire" << std::endl;
    // FIXME
  }

  /*
   * Apply the viewport for the current camera.
   */
  inline void applyViewport() // FIXME const;
  {
    applyViewport(m_current_camera);
  }

  /*
   * When several cameras are available, select
   * the one to be the current one.
   */
  inline void camera2D(const Camera2D& camera)
  {
    m_current_camera = camera;
  }

  /*
   * Accessor. Return the current camera
   */
  inline Camera2D& camera2D()
  {
    return m_current_camera;
  }

  /*
   * Backup settings to the current camera
   */
  inline void restoreCamera2D()
  {
    m_current_camera = m_default_camera;
  }

  /*
   * Default color when clearing pixel buffer
   */
  inline void backgroundColor(const Color& color)
  {
    m_background_color = color;
  }

  inline Color backgroundColor() const
  {
    return m_background_color;
  }

  inline void clearScreen() const
  {
    glClearColor(m_background_color.r, m_background_color.g,
                 m_background_color.b, m_background_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  /*
   * TODO: ajouter une texture comme fond d'ecran
   */

  /*
   * Display the scene in 2d, 3d, slow, fast
   */
  inline void renderStyle(const RenderStyle style)
  {
    m_render_style = style;
  }

  inline RenderStyle renderStyle() const
  {
    return m_render_style;
  }

  /*
   * Draw a graph as friend function
   */
  void render() const;

  /*
   * Number of pixels of the opengl window
   */
  //virtual uint32_t screenWidth() const = 0;
  //virtual uint32_t screenHeight() const = 0;
  // FIXME
  uint32_t screenWidth() { return 0; }
  uint32_t screenHeight() { return 0; }

protected:
  enum { X_AXIS, Y_AXIS, Z_AXIS, N_AXIS };

  Camera2D m_default_camera;
  Camera2D m_current_camera;
  Color m_background_color;
  SimTaDynFont font_list_[1];
  RenderStyle m_render_style;


  GLuint m_Vao {0};
  GLuint m_Buffer {0};
  GLuint m_Program {0};
  GLuint m_Mvp {0};
  std::vector<float> m_RotationAngles;
};

#endif /* RENDERER_HPP_ */
