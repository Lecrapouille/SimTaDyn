#include <iostream>
#include "ClassCounter.tpp"
#include <vector>
#include <utility>

enum GraphElementId : uint32_t
  {
      NODE         = 0x10000000,
      ARC          = 0x20000000,
      ZONE         = 0x40000000,
      MASK         = 0xf0000000,
  };

class Node: private UniqueID<Node>
{
public:

  Node()
    : m_id(getID())
  {
    std::cout << "Node() " << id() << std::endl;
  }

  //! \brief Constructor.
  //! \param nodeID the unique indentifier.
  Node(const uint32_t nodeID)
    : m_id((nodeID))// & ~GraphElementId::MASK) | GraphElementId::NODE)
  {
    std::cout << "Node(" << nodeID << ") " << m_id << std::endl;
    m_arcs.reserve(4);
  }

  //! \brief Constructor by copy.
  //! \param node the reference of an existing node.
  Node(Node const& other)
    : UniqueID<Node>(),
      m_id(other.m_id),// | GraphElementId::NODE),
      m_arcs(other.m_arcs)
  {
    std::cout << "Node() by copy " << m_id << std::endl;
    m_arcs.reserve(4);
  }

  //! \brief Constructor by move
  Node(Node&& other)
  {
    std::cout << "Node() by move " << m_id << std::endl;
    m_id = other.m_id;
    m_arcs = other.m_arcs;
  }

  //! \brief Self copy. Used for the constructor by copy.
  Node* clone() const
  {
    std::cout << "clone()" << std::endl;
    return new Node(*this);
  }

  // Copy assignment operator.
  Node& operator=(const Node& other)
  {
    std::cout << "operator=cpy" << std::endl;

    if (this != &other)
      {
        m_id = other.m_id;
        m_arcs = other.m_arcs;
      }
    return *this;
  }

  // Move assignment operator.
  Node& operator=(Node&& other)
  {
    std::cout << "operator=move" << std::endl;
    if (this != &other)
      {
        m_id = other.m_id;
        m_arcs = other.m_arcs;
      }
    return *this;
  }

  //! \brief Virtual destructor.
  virtual ~Node()
  {
    std::cout << "~Node()" << std::endl;
  }

  //! \brief Return the unique identifier.
  inline uint32_t id() const { return m_id; }


  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  uint32_t m_id;
  //! \brief the list of node neighbors refered by their arc starting
  //! from this node. We prefer sacrificed memory than reducing
  //! computations for looking for neighbors.
  std::vector<Node*> m_arcs;
};

int main()
{
  /*uint32_t id1 = (4 | GraphElementId::ARC);
  printf("0x%x\n", id1);

  // Check removing flags ARC
  id1 = (id1 & ~GraphElementId::MASK) | GraphElementId::NODE;
  printf("0x%x\n", id1);

  // Check removing flags ARC and NODE
  id1 = (4 | GraphElementId::ARC);
  uint32_t id2 = (8 | GraphElementId::NODE);
  id1 = (id2 & ~GraphElementId::MASK) | GraphElementId::ZONE;
  printf("0x%x\n", id1);*/


  Node n1;
  Node n2;
  Node n3;
  Node n4;
  Node p;

  n1.m_arcs.push_back(&n2);
  n1.m_arcs.push_back(&n3);
  n1.m_arcs.push_back(&n4);

  std::cout << "ID: " << n1.id() << " arr: " << n1.m_arcs.size() << std::endl;
  std::cout << "ID: " << p.id() << " arr: " << p.m_arcs.size() << std::endl;

  std::swap(n1, p);

  std::cout << "ID: " << n1.id() << " arr: " << n1.m_arcs.size() << std::endl;
  std::cout << "ID: " << p.id() << " arr: " << p.m_arcs.size() << std::endl;

  return 0;
}
