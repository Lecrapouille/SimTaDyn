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
uint32_t RTreeNode::search_aux(AABB const& bbox) const
{
  uint32_t hitCount = 0;

  if (IS_A_RTREE_LEAF(level))
    {
      for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
        {
          if ((branch[i].child) && bbox.collides(branch[i].box))
            {
              hitCount++;
              // Colorize in green
              std::cout << "  found #" << hitCount << ": " << this << ", child " << i << std::endl;
            }
        }
    }
  else // Internal node
    {
      for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
        {
          if ((branch[i].child) && bbox.collides(branch[i].box))
            {
              hitCount += branch[i].child->search_aux(bbox);
            }
        }
    }

  return hitCount;
}

uint32_t RTreeNode::search(AABB const& bbox) const
{
  uint32_t hitCount;

  std::cout << "search " << bbox << std::endl;

  hitCount = search_aux(bbox);
  if (0 == hitCount)
    {
      std::cout << "  not found" << std::endl;
    }
  return hitCount;
}

RTreeNode* RTreeNode::insert_aux(const uint32_t tid, AABB const& bbox, uint32_t level)
{
  // Still above level for insertion, go down tree recursively
  if (this->level > level)
    {
      uint32_t i = pickBranch(bbox);
      RTreeNode* newnode = branch[i].child->insert_aux(tid, bbox, level);

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
      RTreeBranch b(bbox, (RTreeNode*) tid);
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
RTreeNode* RTreeNode::insert(const uint32_t tid, AABB const& bbox, uint32_t level)
{
  RTreeNode* newnode = insert_aux(tid, bbox, level);
  if (NULL != newnode)
    {
      RTreeNode* newroot = new RTreeNode(this->level + 1U);
      RTreeBranch b(cover(), this);
      //NonLeafCount++;
      newroot->addBranch(b);

      b.box = newnode->cover();
      b.child = newnode;
      newroot->addBranch(b);
      return newroot;
    }
  return this;
}

/*
 * Delete a data rectangle from an index structure.
 * Pass in a pointer to a Rect, the tid of the record, ptr to ptr to root node.
 * Returns 1 if record not found, 0 if success.
 * DeleteRect provides for eliminating the root.
 */
RTreeNode* RTreeNode::remove(const uint32_t tid, AABB const& bbox)
{
  RTreeNodeList *list = NULL;
  RTreeNode *t = NULL;

  if (!remove_aux(tid, bbox, list))
    {
      // Reinsert any branches from eliminated nodes
      while (NULL != list)
        {
          t = list->node;
          for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
            {
              if (NULL != t->branch[i].child)
                {
                  t->branch[i].child->insert((uint32_t) t->branch[i].child,
                                             t->branch[i].box,
                                             t->level);
                }
            }
          RTreeNodeList *e = list;
          list = list->next;
          delete e->node;
          delete e;
        }

      // check for redundant root (not leaf, 1 child) and eliminate
      if (count == 1U)
        {
          for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
            {
              if (t == branch[i].child)
                break;
            }
          delete this;
          return t;
        }
      return this;
    }
  else // Not found
    {
      return this;
    }
}

bool RTreeNode::remove_aux(const uint32_t tid, AABB const& bbox, RTreeNodeList* list)
{
  if (IS_A_RTREE_LEAF(level))
    {
      for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
        {
          if ((NULL != branch[i].child) &&
              (((RTreeNode *) tid) == branch[i].child))
            {
              std::cout << "Found branch to remove " << i << std::endl;
              disconnectBranch(i);
              return false;
            }
        }
      return true;
    }
  else
    {
      for (uint32_t i = 0; i < RTREE_MAX_NODES; i++)
        {
          if ((NULL != branch[i].child) && (bbox.collides(branch[i].box)))
            {
              if (!branch[i].child->remove_aux(tid, bbox, list))
                {
                  if (branch[i].child->count >= RTREE_MIN_FILL)
                    {
                      branch[i].box = branch[i].child->cover();
                    }
                  else
                    {
                      // Not enough entries in child, eliminate child node
                      list->next = new RTreeNodeList(branch[i].child, list);
                      disconnectBranch(i);
                    }
                  return false;
                }
            }
        }
      return true;
    }
}
