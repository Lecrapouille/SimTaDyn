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
//#define RTREE_INTERNAL_NODE (RTREE_LEAF > RTREE_LEAF)

/*
 * Retun true for a leaf, else (for internal node) return false
 */
#define IS_A_RTREE_LEAF(level) (RTREE_LEAF == level)

#  ifndef RTREE_MAX_NODES
#    define RTREE_MAX_NODES  4U
#  endif /* RTREE_MAX_NODES */

/*
 * Balance criterion for node splitting
 */
#  ifndef RTREE_MIN_FILL
#    define RTREE_MIN_FILL (RTREE_MAX_NODES / 3U)
#  endif /* RTREE_MIN_FILL */

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
    os << "RTreeBranch(" << b.box << ", " << *(b.child) << "\n";
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
  RTreeNode* insert(const uint32_t tid, AABB const& bbox, uint32_t level);
  RTreeNode* remove(const uint32_t tid, AABB const& bbox);

  static size_t howMany()
  {
    return ClassCounter<RTreeNode>::howMany();
  }

  inline friend std::ostream& operator<<(std::ostream& os, const RTreeNode& n)
  {
    n.debugIndex(os);
    return os;
  }

protected:

  enum rtree_partion_id
    {
      RTREE_PARTITION_0,
      RTREE_PARTITION_1,
      RTREE_UNDEF_PARTITION,
    };

  class RTreeSpliter
  {
  public:
    RTreeBranch BranchBuf[RTREE_MAX_NODES + 1U];
    AABB CoverSplit;
    float32_t CoverSplitVolume;
    //PartitionVars Partitions;
  };

  class PartitionVars
  {
  public:
    void init()
    {
      count[RTREE_PARTITION_0] = count[RTREE_PARTITION_1] = 0;
      cover[RTREE_PARTITION_0] = cover[RTREE_PARTITION_1] = AABB::DUMMY;
      volume[RTREE_PARTITION_0] = volume[RTREE_PARTITION_1] = 0.0f;
      for (uint32_t i = 0; i < RTREE_MAX_NODES + 1U; i++)
        {
          taken[i] = false;
          partition[i] = RTREE_UNDEF_PARTITION;
        }
    }
    uint32_t partition[RTREE_MAX_NODES + 1U];
    uint32_t taken[RTREE_MAX_NODES + 1U];
    uint32_t count[2];
    AABB cover[2];
    float32_t volume[2];

    void methodZero(RTreeSpliter& s);
    void classify(const uint32_t i, const uint32_t group, RTreeSpliter& s);
    void pickSeeds(RTreeSpliter& s);
  };


  class RTreeNodeList
  {
  public:
    RTreeNodeList(RTreeNode* node, RTreeNodeList* next)
    {
      this->next = next;
      this->node = node;
    }

    RTreeNodeList *next;
    RTreeNode *node;
  };

  void initNode();
  AABB cover() const;
  uint32_t pickBranch(AABB const& bbox) const;
  bool disconnectBranch(const uint32_t b);
  RTreeNode* addBranch(RTreeBranch const& b);
  RTreeNode* insert_aux(const uint32_t tid, AABB const& bbox, uint32_t level);
  bool remove_aux(const uint32_t tid, AABB const& bbox, RTreeNodeList* list);
  RTreeNode* splitNodeQuadratic(RTreeBranch const& b, RTreeSpliter& s);
  bool getBranches(RTreeBranch const& b, RTreeSpliter& s);
  uint32_t search_aux(AABB const& bbox) const;

  // 0 is leaf, others positive
  uint32_t level;
  // Number of branches used
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
