#include "RTree.hpp"

/*
 * Make a new index, empty.  Consists of a single node.
 */
RTreeNode::RTreeNode()
{
  level = 0; // FIXME -1
  initNode();
}

// Alias newIndex
RTreeNode::RTreeNode(const uint32_t level)
{
  this->level = level;
  initNode();
}

void RTreeNode::initNode()
{
  //nodeCount = ClassCounter<RTreeNode>::howMany();
  count = 0;
  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      branch[i].child = nullptr;
      // Implicit bounding box init:
      // box =  RTREE_DUMMY_BBOX;
    }
}

RTreeNode::~RTreeNode()
{
  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      if (nullptr != branch[i].child)
        {
          delete branch[i].child;
        }
    }
}

void RTreeNode::debugNode(std::ostream& os) const
{
  for (uint32_t i = 0; i < level; ++i)
    {
      os << "      ";
    }
  os << "RTreeNode(";
  if (IS_A_RTREE_LEAF(level))
    {
      os << "LEAF";
    }
  else
    {
      os << "NODE";
    }
  os << ", level: " << level
     << ", count: " << count
     << ", address: " << this;

  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      os << "\n";
      for (uint32_t i = 0; i < level; ++i)
      {
         os << "      ";
      }
      os << "    branch " << i;
      os << ": child " << branch[i].child;
      os << ", " << branch[i].box;
    }
  os << std::endl;
}

/*
 * Find the smallest rectangle that includes all rectangles in
 * branches of a node.
 * Return AABB:DUMMY if all child are nullptr
 */
AABB RTreeNode::cover() const
{
  AABB bbox;
  bool flag = true;

  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      if (branch[i].child)
        {
          if (flag)
            {
              bbox = branch[i].box;
              flag = false;
            }
          else
            {
              bbox = bbox.merge(branch[i].box);
            }
        }
    }
  return bbox;
}

/*
 * Pick a branch. Pick the one that will need the smallest increase in
 * area to accomodate the new rectangle. This will result in the least
 * total area for the covering rectangles in the current node.  In
 * case of a tie, pick the one which was smaller before, to get the
 * best resolution when searching.
 */
uint32_t RTreeNode::pickBranch(AABB const& bbox) const
{
  uint32_t best = 0;
  bool flag = true;
  float32_t increase, volume;
  float32_t bestIncr = 0.0f; // init useless because of flag but stop compil warnings
  float32_t bestVolume = 0.0f; // idem

  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      if (branch[i].child)
        {
          volume = branch[i].box.volume();
          increase = (bbox.merge(branch[i].box)).volume() - volume;

          if ((increase < bestIncr) || (flag))
            {
              best = i;
              bestVolume = volume;
              bestIncr = increase;
              flag = false;
            }
          else if ((increase == bestIncr) && (volume < bestVolume))
            {
              best = i;
              bestVolume = volume;
              bestIncr = increase;
            }
        }
    }

  return best;
}

/*
 * Disconnect a dependent node.
 * Return false if not possible (bad param or a leaf)
 */
bool RTreeNode::disconnectBranch(const uint32_t b)
{
  if ((b < RTREE_MAX_NODES) && (nullptr != branch[b].child))
    {
      branch[b].child = nullptr;
      branch[b].box = RTREE_DUMMY_BBOX;
      count--;
      return true;
    }
  return false;
}

/*
 * Add a branch to a node. Split the node if necessary. Returns 0 if
 * node not split. Old node updated.  Returns 1 if node split, sets
 * *new to address of new node.  Old node updated, becomes one of two.
*/
RTreeNode* RTreeNode::addBranch(RTreeBranch const& b)
{
  RTreeNode* newNode;

  if (count < RTREE_MAX_NODES)
    {
      for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
        {
          if (nullptr == branch[i].child)
            {
              branch[i] = b;
              ++count;
              break;
            }
        }
      return nullptr;//this;
    }
  else
    {
      RTreeSpliter s;
      newNode = splitNodeQuadratic(b, s);
      /*if (IS_LEAF(level))
        {
          ++LeafCount;
        }
      else
        {
          ++NonLeafCount;
        }*/
      return newNode;
    }
}
