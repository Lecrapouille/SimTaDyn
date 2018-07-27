//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================
//
// Additional license:
//
// This code is a C++ translation of the original C code:
// http://web.archive.org/web/20030624200338/http://www.es.ucsc.edu/~tonig/
// http://web.archive.org/web/20030604132727/http://www.es.ucsc.edu/~tonig/rtrees/rtrees.pdf
// Copyright 1994 1995 Antonin Guttman
// License: ??? Public domain ???
//=====================================================================

#ifndef RTREE_HPP_
#  define RTREE_HPP_

#  include "ClassCounter.tpp"
#  include "BoundingBox.tpp"

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
#define RTREE_DUMMY_BBOX    AABB3f::DUMMY
#define RTREE_LEAF          (0U)
//#define RTREE_INTERNAL_NODE (RTREE_LEAF > RTREE_LEAF)

/*
 * Retun true for a leaf, else (for internal node) return false
 */
#define IS_A_RTREE_LEAF(level) (RTREE_LEAF == level)

#  ifndef RTREE_MAX_NODES
#    define RTREE_MAX_NODES  8U
#  endif /* RTREE_MAX_NODES */

/*
 * Balance criterion for node splitting
 */
#  ifndef RTREE_MIN_FILL
#    define RTREE_MIN_FILL (RTREE_MAX_NODES / 4U)
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

  RTreeBranch(AABB3f const& bbox, RTreeNode* const c = NULL)
  {
    box = bbox;
    child = c;
  }

  RTreeBranch(AABB3f const& bbox, const uint32_t id)
  {
    box = bbox;
    tid = id;
  }

  /*inline friend std::ostream& operator<<(std::ostream& os, const RTreeBranch& b)
  {
    os << "RTreeBranch(" << b.box << ", " << *(b.child) << "\n";
    return os;
    }*/

  AABB3f box;
  union
  {
    RTreeNode* child;
    uint32_t tid; // Tuple ID
  };
};

// **************************************************************
//
// **************************************************************
class RTreeNode
  : private InstanceCounter<RTreeNode>
{
public:
  RTreeNode();
  RTreeNode(const uint32_t level);
  ~RTreeNode();

  void debugNode(std::ostream& out) const;
  void debugIndex(std::ostream& out) const;
  void debugData(std::ostream& out) const;

  uint32_t search(AABB3f const& bbox) const;
  RTreeNode* insert(const uint32_t tid, AABB3f const& bbox, uint32_t level);
  RTreeNode* remove(const uint32_t tid, AABB3f const& bbox);

  static size_t howMany()
  {
    return InstanceCounter<RTreeNode>::howMany();
  }

  inline friend std::ostream& operator<<(std::ostream& os, const RTreeNode& n)
  {
    n.debugIndex(os);
    return os;
  }

  /*
   * For drawing the graph scene
   */
  friend class Renderer;

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
    AABB3f CoverSplit;
    float CoverSplitVolume;
    //PartitionVars Partitions;
  };

  class PartitionVars
  {
  public:
    void init()
    {
      count[RTREE_PARTITION_0] = count[RTREE_PARTITION_1] = 0;
      cover[RTREE_PARTITION_0] = cover[RTREE_PARTITION_1] = AABB3f::DUMMY;
      volume[RTREE_PARTITION_0] = volume[RTREE_PARTITION_1] = 0.0f;
      for (uint32_t i = 0; i < RTREE_MAX_NODES + 1U; ++i)
        {
          taken[i] = false;
          partition[i] = RTREE_UNDEF_PARTITION;
        }
    }
    uint32_t partition[RTREE_MAX_NODES + 1U];
    uint32_t taken[RTREE_MAX_NODES + 1U];
    uint32_t count[2];
    AABB3f cover[2];
    float volume[2];

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
  AABB3f cover() const;
  uint32_t pickBranch(AABB3f const& bbox) const;
  bool disconnectBranch(const uint32_t b);
  RTreeNode* addBranch(RTreeBranch const& b);
  RTreeNode* insert_aux(const uint32_t tid, AABB3f const& bbox, uint32_t level);
  bool remove_aux(const uint32_t tid, AABB3f const& bbox, RTreeNodeList* list);
  RTreeNode* splitNodeQuadratic(RTreeBranch const& b, RTreeSpliter& s);
  bool getBranches(RTreeBranch const& b, RTreeSpliter& s);
  uint32_t search_aux(AABB3f const& bbox) const;

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
  RTreeNode* m_root = nullptr;

  RTree()
  {
    leafCount = 0;
    nonLeafCount = 0;

    m_root = new RTreeNode(RTREE_LEAF);
    ++leafCount;
  }

  ~RTree()
  {
    if (nullptr != m_root)
      {
        delete m_root;
      }
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
