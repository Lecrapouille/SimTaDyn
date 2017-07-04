// -*- c++ -*- Coloration Syntaxique pour Emacs
#ifndef SCENEGRAPH_TPP_
#  define SCENEGRAPH_TPP_

#  include "Mesh.hpp"
#  include <memory>
#  include <vector>

// *************************************************************************************************
//
// *************************************************************************************************
template <typename T, uint32_t n>
class SceneNode: public Movable<T, n>
{
public:

  //! \brief Empty constructor with no 3D object. Can be used for
  //! transforming childrens.
  SceneNode(const char *name)
  //: m_world_transform(matrix::Identity)
    :     m_scalef(1)
  {
    m_mesh = nullptr;
    m_parent = nullptr;
    m_name = name;
  }

  //! \brief Constructor with a 3D object.
  SceneNode(Mesh &mesh, const char *name)
  //: m_world_transform(matrix::Identity)
    :     m_scalef(1)
  {
    m_mesh = &mesh;
    m_parent = nullptr;
    m_name = name;
  }

  virtual ~SceneNode()
  {
    uint32_t i = m_childrens.size();
    while (i--)
      {
        delete m_childrens[i];
      }
  }

  inline void mesh(Mesh &mesh)
  {
    m_mesh = &mesh;
  }

  inline Mesh *mesh() const
  {
    return m_mesh;
  }

  // FIXME node shall no already exist whatever in the tree
  void add(SceneNode &node)
  {
    node.m_parent = this;
    m_childrens.push_back(&node);
  }

  inline Matrix<T, n + 1U, n + 1U> const& worldTransform() const
  {
    return m_world_transform;
  }

  virtual void update(float const dt)
  {
    LOGI("ScenGraph::updating '%s'", m_name.c_str());
    if (nullptr != m_parent)
      {
        // This  node  has a parent
        m_world_transform = //FIXME
          Movable<T,n>::transform() *
          m_parent->m_world_transform;

      }
    else
      {
        // Root node, world transform is local transform
        m_world_transform = Movable<T,n>::transform();
      }

    // Update all children
    for (auto i: m_childrens)
      {
        i->update(dt);
      }
  }

  // FIXME: override
  //! \brief Set scale factors of the object.
  inline void scaleF(Vector<T, n> const &scale)
  {
    m_scalef = scale;
  }

  //! \brief Get scale factors of the object.
  inline Vector<T, n> const &scaleF() const
  {
    return m_scalef;
  }

  virtual void drawnBy()//GLRenderer& renderer)
  {
    if (nullptr != m_mesh)
      m_mesh->draw(GL_TRIANGLES);
  }

  typedef typename std::vector<SceneNode<T,n>*>::const_iterator const_iterator;
  typedef typename std::vector<SceneNode<T,n>*>::iterator iterator;

  const_iterator cbegin() const // FIXME
  {
    return m_childrens.cbegin();
  }

  const_iterator cend() const
  {
    return m_childrens.cend();
  }

  //protected:

  Mesh                      *m_mesh;
  SceneNode                 *m_parent;
  Matrix<T, n + 1U, n + 1U>  m_world_transform;
  std::vector<SceneNode<T,n>*>    m_childrens;
  Vector<T, n> m_scalef;
  std::string m_name;
};

#endif /* SCENEGRAPH_TPP_ */
