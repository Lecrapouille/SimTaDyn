#include "proto-graph.hpp"
#include <set>

//Node Arc::m_dummy_border = Node(0);
//Arc Node::m_dummy_neighbor = Arc(0, Node(0), Node(0));

// -----------------------------------------------------
template<class T>
static void sort_and_order(std::vector<T> &vec)
{
  std::set<T> s;
  size_t size = vec.size();
  for (size_t i = 0; i < size; ++i)
    s.insert(vec[i]);
  vec.assign(s.begin(), s.end());
}

// -----------------------------------------------------
// TODO: sauvegarder nodes (= mise en cache) pour eviter
// de recalculer la prochaine fois.
template<class D>
void Node<D>::getNeighborNodes(std::vector<Node<D>>& nodes)
{
  nodes.clear();
  for (auto& it_arc: arcs())
    {
      for (auto& it_node: it_arc.borders())
        {
          if (m_id != it_node.id())
            {
              nodes.push_back(it_node);
            }
        }
    }
}

// -----------------------------------------------------
template<class D>
Arc<D>::Arc(ArcId id, Node<D>& from, Node<D>& to, D const& data)
  : Borders<Node<D>>()
{
  m_id = id;
  m_data = data;
  this->addBorder(from);
  this->addBorder(to);
}

// -----------------------------------------------------
template<class D>
Arc<D>::Arc(ArcId id, Node<D>& from, Node<D>& to)
  : Borders<Node<D>>()
{
  m_id = id;
  this->addBorder(from);
  this->addBorder(to);
}

#include <cassert>

int main()
{
  struct Data
  {
    Data()
    {}
    Data(Data const& data)
      : result(data.result)
    {}
    Data(Data&& data)
      : result(data.result)
    {}
    Data(int r)
      : result(r)
    {}
    Data& operator=(Data const& other)
    {
      if (this != &other)
        {
          result = other.result;
        }
      return *this;
    }

    int result = 0;
  };

  using IntNode = Node<Data>;
  using IntArc = Arc<Data>;
  //Graph<IntNode, IntArc, Data> g(false);

  IntNode n0, n1, n2;

  //IntNode::resetID();

  // name
  std::cout << n0.name() << std::endl;
  assert("N0" == n0.name());
  assert("N1" == n1.name());
  assert("N2" == n2.name());

  // id
  assert(0_z == n0.id());
  assert(1_z == n1.id());
  assert(2_z == n2.id());

  // operator==
  assert(n0 == 0_z); // TODO: manque inverse: 0 == n0
  assert(n1 == 1_z);
  assert(n2 == 2_z);

  // operator==
  IntNode n2bis(2_z);
  assert(2_z == n2bis.id());
  assert(n2 == n2bis);
  assert("N2" == n2bis.name());

  // Neighbors
  assert(0_z == n0.arcs().size());
  assert(0_z == n0.neighbors().size());
  assert(0_z == n0.degree());
  assert(0_z == n0.howManyNeighbors());

  // Data
  IntNode n42(42_z, Data(42));
  assert(42 == n42.data().result);
  n42.data(43);
  assert(43 == n42.data().result);

  // Add arcs.
  IntArc a66(66, n0, n1);
  assert(66_z == a66.id());

  n42.addArc(a66);
  assert(1_z == n42.arcs().size());
  assert(1_z == n42.neighbors().size());
  assert(1_z == n42.degree());
  assert(1_z == n42.howManyNeighbors());
  assert(a66 == n42.arc(0_z));
  assert(a66 == n42.neighbor(0_z));

  // Copy constructor
  IntNode nc(n42);
  assert(42 == nc.id());
  assert(43 == nc.data().result);
  assert(1_z == nc.degree());
  assert(a66 == nc.arc(0_z));

  // Copy assign
  IntNode nca = nc;
  assert(42 == nca.id());
  assert(43 == nca.data().result);
  assert(1_z == nca.degree());
  assert(a66 == nca.arc(0_z));

  // Move constructor
  IntNode nm(std::move(nca));
  assert(42 == nm.id());
  assert(43 == nm.data().result);
  assert(1_z == nm.degree());
  assert(a66 == nm.arc(0_z));

  // Move assign
  IntNode nma; nma = std::move(nm);
  assert(42 == nma.id());
  assert(43 == nma.data().result);
  assert(1_z == nma.degree());
  assert(a66 == nma.arc(0_z));

  return 0;

  #if 0

  std::cout << "AA " << g.nodes()[0].name() << std::endl;
  std::cout << "AA " << g.nodes()[1].name() << std::endl;
  std::cout << "AA " << g.nodes()[2].name() << std::endl;

  IntNode& n1 = g.addNode(1, Data(42));
  IntNode& n2 = g.addNode(2, Data(43));
  IntArc& a1 = g.addArc(n1, n2, Data(44));
  IntArc& a2 = g.addArc(1, 3, Data(45));
  IntArc& a3 = g.addArc(4, 1, Data(46));

  std::cout << n1.name() << " has " << n1.howManyNeighbors() << " arcs as neighbors" << std::endl;
  for (auto& it: n1.neighbors())
    {
      std::cout << it.name() << std::endl;
    }

  std::vector<IntNode> vec;
  n1.getNeighborNodes(vec);
  std::cout << n1.name() << " has " << vec.size() << " nodes as neighbors" << std::endl;
  for (auto& it: vec)
    {
      std::cout << it.name() << std::endl;
    }

  std::cout << a2.name() << " has " << a2.howManyBorders() << " nodes as borders" << std::endl;
  for (auto& it: a2.borders())
    {
      std::cout << it.name() << std::endl;
    }

  if (g.removeArc(a2.id()))
    {
      std::cout << n1.name() << " has " << vec.size() << " nodes as neighbors" << std::endl;
      for (auto& it: vec)
        {
          std::cout << it.name() << std::endl;
        }
    }
  else
    {
      std::cout << "ko" << std::endl;
    }
#endif

  return 0;
}
