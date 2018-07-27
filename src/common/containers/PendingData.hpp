#ifndef PENDINGDATA_HPP_
#  define PENDINGDATA_HPP_

#  include "Maths.hpp"

// **************************************************************
//! \brief Define an interface class keeping track of the smallest
//! contiguous area that have been changed and needs to be
//! uploaded. This class cannot be use alone but for inheritance.
// **************************************************************
class PendingData
{
public:

  //! \brief Empty constructor: no pending data.
  PendingData()
  {
    clearPending();
  }

  //! \brief Pure virtual destructor, but with a definition. The class
  //! will be abstract, but any inheriting classes will not by default
  //! be abstract.
  virtual ~PendingData() = 0;

  //! \brief Return a boolean indicating if some elements of the block
  //! has chnaged.
  inline bool hasPendingData() const
  {
    bool res = ((uint32_t) -1 != m_pending_start);
    return res;
  }

  //! \brief Return the smallest contiguous area that needs to be
  //! uploaded. If there is no pending data, pos_start will be set to
  //! -1. You can call hasPendingData() before this method.
  void getPendingData(uint32_t &pos_start, uint32_t &pos_end) const
  {
    if ((uint32_t) -1 != m_pending_start)
    {
      pos_start = m_pending_start;
      pos_end = m_pending_end;
    }
    else
    {
      pos_start = 0;
      pos_end = 0;
    }
  }

  //! \brief Call this function when changed elements have been uploaded.
  void clearPending()
  {
    m_pending_start = (uint32_t) -1;
    m_pending_end = (uint32_t) -1;
  }

  //! \brief Update the range indexes of changed elements with a new range.
  void addPendingData(const uint32_t pos_start, const uint32_t pos_end)
  {
    if ((uint32_t) -1 == m_pending_start)
      {
        m_pending_start = pos_start;
        m_pending_end = pos_end;
      }
    else
      {
        m_pending_start = maths::min(m_pending_start, pos_start);
        m_pending_end = maths::max(m_pending_end, pos_end);
      }
  }

  //! \brief Update the range indexes of changed elements with a new range.
  inline void addPendingData(const uint32_t pos_start)
  {
    addPendingData(pos_start, pos_start);
  }

protected:

  //! Indicate which elements have been changed.
  uint32_t m_pending_start, m_pending_end;
};

#endif
