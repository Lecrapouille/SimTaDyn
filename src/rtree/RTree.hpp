#ifndef RTREE_HPP_
#  define RTREE_HPP_

#  include "ClassCounter.hpp"
#  include "BoundingBox.hpp"

#include <iostream>
#include <fstream>

// Inspired from:
//   http://web.archive.org/web/20030624200338/http://www.es.ucsc.edu/~tonig/
// and:
//   http://web.archive.org/web/20030604132727/http://www.es.ucsc.edu/~tonig/rtrees/rtrees.pdf

// **************************************************************
// An R-tree is a tree data structures used for spatial access
// methods. It is a height-balanced tree similar to a B-tree with
// index records in its leaf nodes containing pointers to Nodes
// correspond to disk data objects pages.
//
// An R-tree has a closed similar to a Quadtree with difference
// is not necessarly height-balanced.
// **************************************************************

/*
 * Interface
 */
#define RTREE_DUMMY_BBOX    AABB::DUMMY
#define RTREE_LEAF          (0U)
#define RTREE_INTERNAL_NODE (RTREE_LEAF > RTREE_LEAF)
/*
 * Retun true for a leaf, else (for internal node) return false
 */
#define IS_A_RTREE_LEAF(level) (RTREE_LEAF == level)


#  ifndef RTREE_MAX_NODES
#    define RTREE_MAX_NODES  4U
#  endif /* RTREE_MAX_NODES */

class RTreeNode;

// **************************************************************
//
// **************************************************************
class RTreeBranch
{
public:
  RTreeBranch(RTreeNode* const c = NULL)
  {
    child = c;
  }

  RTreeBranch(AABB const& bbox, RTreeNode* const c = NULL)
  {
    box = bbox;
    child = c;
  }

  /*inline friend std::ostream& operator<<(std::ostream& os, const RTreeBranch& b)
  {
    b.debugIndex(os);
    return os;
    }*/

  AABB box;
  RTreeNode* child;
};

// **************************************************************
//
// **************************************************************
class RTreeNode
  : private ClassCounter<RTreeNode>
{
public:
  RTreeNode();
  RTreeNode(const uint32_t level);
  ~RTreeNode();

  void debugNode(std::ostream& out) const;
  void debugIndex(std::ostream& out) const;
  void debugData(std::ostream& out) const;

  uint32_t search(AABB const& bbox) const;
  RTreeNode* insert(AABB const& bbox, uint32_t level);

  static size_t howMany()
  {
    return ClassCounter<RTreeNode>::howMany();
  }

  inline friend std::ostream& operator<<(std::ostream& os, const RTreeNode& n)
  {
    //n.debugData(os);
    n.debugNode(os);
    return os;
  }

protected:
  void initNode();
  AABB cover() const;
  uint32_t pickBranch(AABB const& bbox) const;
  bool disconnectBranch(const uint32_t b);
  RTreeNode* addBranch(RTreeBranch const& b);
  RTreeNode* splitNode(RTreeBranch const& b);
  RTreeNode* insert2(AABB const& bbox, uint32_t level);

  // 0 is leaf, others positive
  uint32_t level;
  // ???
  uint32_t count;
  RTreeBranch branch[RTREE_MAX_NODES];
};

// **************************************************************
//
// **************************************************************
class RTree
{
public:
  RTreeNode* root;

  RTree()
  {
    leafCount = 0;
    nonLeafCount = 0;

    root = new RTreeNode(RTREE_LEAF);
    leafCount++;
  }



  uint32_t howManyNodes()
  {
    return RTreeNode::howMany();
  }

  uint32_t howManyLeaves()
  {
    return leafCount;
  }

  uint32_t howManyNonLeaves()
  {
    return nonLeafCount;
  }

private:

  uint32_t leafCount;
  uint32_t nonLeafCount;
};

#endif /* RTREE_HPP_ */
