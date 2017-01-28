#include "GraphMemory.hpp"
#include "BasicGraph.hpp"
#include "CellForth.hpp"

// **************************************************************
//
// **************************************************************
class SimNode : public BasicNode, public CellForth
{
public:
  SimNode()
    : BasicNode(), CellForth()
  {
    setName();
  }

  SimNode(const Key nodeID)
    : BasicNode(nodeID), CellForth()
  {
    setName();
  }

  const std::string& name() const
  {
    return m_name;
  }

protected:
  inline void setName()
  {
    m_name = "Node" + std::to_string(m_id);
  }

  std::string m_name;
};

// **************************************************************
//
// **************************************************************
class SimArc : public BasicArc, public CellForth
{
public:
  SimArc(const Key fromID, const Key toID)
    : BasicArc(fromID, toID), CellForth()
  {
    setName();
  }

  const std::string& name() const
  {
    return m_name;
  }

protected:
  inline void setName()
  {
    m_name = "Arc" + std::to_string(m_id);
  }

  std::string m_name;
};
