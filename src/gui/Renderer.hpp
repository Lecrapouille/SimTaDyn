#ifndef RENDERER_HPP_
#  define RENDERER_HPP_

#  include "OpenGL.hpp"
#  include "Color.hpp"
#  include "Camera2D.hpp"
#  include "Fonts.hpp"
#  include "SimTaDynGraphs.hpp"
#  include "RTree.hpp" // a inclure dans SimTaDynGraphs.hpp

/*
 * OpenGL legacy renderer.
 * Note: gtkglextmm does not manage OpenGL >= 3.0
 */
class Renderer
{
public:

  enum RenderStyle { Fast2D, Medium2D, Low2D };

  Renderer()
    : default_camera_(),
      current_camera_(),
      background_color_(Color::Black),
      style_(Renderer::Fast2D)
  {
  }

  ~Renderer();

  /*
   * Same role than code in a constructor.
   * Shall be called during the init of
   * OpenGL context.
   */
  void initialize();

  /*
   * When camera states change, apply
   * these changement to the screen.
   */
  void applyViewport(Camera2D& camera);
  //void applyViewport(const Camera2D& camera) const; // FIXME: pas de const a cause de getTransform

  /*
   * Apply the viewport for the current camera.
   */
  void applyViewport();
  //void applyViewport() const;

  /*
   * When several cameras are available, select
   * the one to be the current one.
   */
  void selectCamera2D(const Camera2D& camera);

  /*
   * Accessor. Return the current camera
   */
  Camera2D& getCamera2D();

  /*
   * Backup settings to the current camera
   */
  void restoreCamera2D();

  /*
   * Default color when clearing pixel buffer
   */
  void setBackgroundColor(const Color& color);
  void clearScreen() const;

  /*
   * TODO: ajouter une texture comme fond d'ecran
   */

  /*
   * Display the scen in 2d, 3d, slow, fast
   */
  void setRenderStyle(const RenderStyle style);

  /*
   * Draw a graph as friend function
   */
  void draw(/*const*/ SimTaDynGraph& graph);// const;
  void draw(const RTreeNode* root, const Vector3D& center) const;
  void draw(const AABB& box, const Vector3D& center) const;
  void zoomFitPage(const SimTaDynGraph& graph);

  /*
   * Number of pixels of the opengl window
   */
  virtual uint32_t getScreenWidth() const = 0;
  virtual uint32_t getScreenHeight() const = 0;

protected:

  enum DisplayList { Node2D, Arc2D, Node3D, Arc3D, nolist_ };

  /*
   *
   */
  GLuint create3DModelNode(void) const;

  Camera2D default_camera_;
  Camera2D current_camera_;
  Color background_color_;
  GLuint display_list_[nolist_] = {};
  SimTaDynFont font_list_[1];
  RenderStyle style_;
};

#endif /* RENDERER_HPP_ */
