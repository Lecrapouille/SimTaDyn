// -*- c++ -*- Coloration Syntaxique pour Emacs
#ifndef MOVABLE_TPP_
#  define MOVABLE_TPP_

#  include "Transformation.tpp"

template <typename T, uint32_t n>
class Movable
{
public:
  Movable()
    m_origin(0),
    m_position(0),
    m_rotation(0),
    m_angle(0),
    m_transform(matrix::Identity),
    m_inverse_transform(matrix::Identity)
  {
  }

  inline void position(Vector<T, n> const &position)
  {
    m_position = position;
    m_to_update = true;
  }

  inline Vector<T, n> const &position() const
  {
    return m_position;
  }

  inline void origin(Vector<T, n> const &origin)
  {
    m_origin = origin;
    m_to_update = true;
  }

  inline Vector<T, n> const &origin() const
  {
    return m_origin;
  }

  inline void scale(Vector<T, n> const &scale)
  {
    m_scale = scale;
    m_to_update = true;
  }

  inline Vector<T, n> const &scale() const
  {
    return m_scale;
  }

  inline void rotate(T const angle, Vector<T, 3U> const &v)
  {
    m_angle = angle;
    m_rotation = v;
    m_to_update = true;
  }

  inline void move(Vector<T, n> const &offset)
  {
    m_position += offset;
  }

  inline void scalefactor(Vector<T, n> const &factor)
  {
    m_scale *= factor;
    m_to_update = true;
  }

  Matrix<T, 4U, 4U> const &transform()
  {
    if (m_to_update)
      {
        m_transform = translate(matrix::Identity, m_position - m_origin);
        m_transform = rotate(m_transform, m_rot_angle, m_rot_axis);
        m_transform = scale(m_transform, m_scale);
        m_to_update = false;

        m_inverse_transform = matrix::inverse(m_transform);
      }

    return m_transform;
  }

protected:

  Vector<T, n> m_origin;
  Vector<T, n> m_position;
  Vector<T, n> m_scale;
  Vector<T, n> m_rotation;
  T m_angle;
  Matrix<T, n + 1U, n + 1U> m_transform;
  Matrix<T, n + 1U, n + 1U> m_inverse_transform;
};

#endif
