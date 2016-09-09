#include "RTree.hpp"

/*
 * Print out all the nodes in an index. For graphics output, displays
 * lower boxes first so they don't obscure enclosing boxes.  For
 * printed output, prints from root downward.
*/
void RTreeNode::debugIndex(std::ostream& os) const
{
  RTreeNode *node;
  uint32_t i;

  debugNode(os);
  if (!IS_A_RTREE_LEAF(level))
    {
      for (i = 0; i < RTREE_MAX_NODES; i++)
        {
          node = branch[i].child;
          if (NULL != node)
            {
              node->debugIndex(os);
            }
        }
    }
}

/*
 * Print out all the data rectangles in an index.
 */
void RTreeNode::debugData(std::ostream& os) const
{
  RTreeNode *node;
  uint32_t i;

  if (IS_A_RTREE_LEAF(level))
    {
      debugNode(os);
    }
  else
    {
      // Internal node
      for (i = 0; i < RTREE_MAX_NODES; i++)
        {
          node = branch[i].child;
          if (NULL != node)
            {
              node->debugData(os);
            }
        }
    }
}

/*
 * Search in an index tree or subtree for all data retangles that
 * overlap the argument rectangle. Returns the number of qualifying
 * data boudingbox.
 */
uint32_t RTreeNode::search(AABB const& bbox) const
{
  uint32_t hitCount = 0;

  if (IS_A_RTREE_LEAF(level))
    {
      for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
        {
          if ((branch[i].child) && bbox.contains(branch[i].box))
            {
              hitCount++;
              // Colorize in green
              std::cout << "search: found #" << hitCount << ": " << this << ", child " << i << std::endl;
            }
        }
    }
  else // Internal node
    {
      for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
        {
          if ((branch[i].child) && bbox.contains(branch[i].box))
            {
              hitCount += branch[i].child->search(bbox);
            }
        }
    }
  return hitCount;
}

RTreeNode* RTreeNode::insert2(AABB const& bbox, uint32_t level)
{
  // Still above level for insertion, go down tree recursively
  if (this->level > level)
    {
      uint32_t i = pickBranch(bbox);
      RTreeNode* newnode = branch[i].child->insert2(bbox, level);

      if (NULL == newnode)
        {
          // child was not split
          branch[i].box = bbox.merge(branch[i].box);
          return NULL;
        }
      else
        {
          // child was split
          branch[i].box = branch[i].child->cover();
          RTreeBranch b(newnode->cover(), newnode);
          return addBranch(b);
        }
    }

  // Have reached level for insertion. Add rect, split if necessary
  else if (this->level == level)
    {
      RTreeBranch b(bbox, NULL); // FIXME NULL --> copier l'address de la figure geometrique !!!
      return addBranch(b);
    }

  // Not supposed to happen
  else
    {
      std::cerr << "** ERROR: RTreeNode::insert2: mismatch level values" << std::endl;
      return NULL;
    }
}

/*
 * Insert a data rectangle into an index structure. InsertRect
 * provides for splitting the root; returns 1 if root was split, 0 if
 * it was not.  The level argument specifies the number of steps up
 * from the leaf level to insert; e.g. a data rectangle goes in at
 * level = 0.  InsertRect2 does the recursion.
*/
RTreeNode* RTreeNode::insert(AABB const& bbox, uint32_t level)
{
  RTreeNode* newnode = insert2(bbox, level);
  if (NULL != newnode)
    {
      RTreeNode* newroot = new RTreeNode(this->level + 1U);
      RTreeBranch b(cover(), this);
      //NonLeafCount++;
      newroot->addBranch(b);

      b.box = newnode->cover();
      b.child = newnode;
      newnode->addBranch(b);
      return newnode;
    }
  return this;
}
