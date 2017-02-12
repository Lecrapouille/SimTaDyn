#include "RTree.hpp"

/*
 * Split a node.
 * Divides the nodes branches and the extra one between two nodes.
 * Old node is one of the new ones, and one really new one is created.
 * Tries more than one method for choosing a partition, uses best
 * result.
 */
RTreeNode *RTreeNode::splitNodeQuadratic(RTreeBranch const& b, RTreeSpliter& s)
{
  PartitionVars part;
  PartitionVars* p;
  RTreeNode *newnode;
  uint32_t level_;

  // Load all the branches into a buffer, initialize old node */
  level_ = level;
  getBranches(b, s); // assert (true == getB) // s is init here.

  // Find partition
  p = &part;
  p->methodZero(s);
  //float32_t volume = p->cover[RTREE_PARTITION_0].volume() + p->cover[RTREE_PARTITION_1].volume();

  // Put branches from buffer into 2 nodes according to chosen partition
  newnode = new RTreeNode(level_);
  level = level_;

  // Load nodes (copy branches from the buffer into two nodes according to the partition)
  for (uint32_t i = 0; i < RTREE_MAX_NODES + 1U; ++i)
    {
      if (RTREE_PARTITION_0 == p->partition[i])
        {
          addBranch(s.BranchBuf[i]);
          // assert(this == res);
        }
      else if (RTREE_PARTITION_1 == p->partition[i])
        {
          newnode->addBranch(s.BranchBuf[i]);
          // assert(newnode == res);
        }
      else
        {
          std::cerr << "assertion false: RTreeNode::splitNodeQuadratic(): undefined partition\n";
        }
    }

  if (count + newnode->count != RTREE_MAX_NODES + 1U)
    {
      std::cerr << "assertion false: RTreeNode::splitNodeQuadratic: too many branches\n";
    }

  return newnode;
}

/*
 * Load branch buffer with branches from full node plus the extra branch.
 */
bool RTreeNode::getBranches(RTreeBranch const& b, RTreeSpliter& s)
{
  // Load the branch buffer
  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      // Node should have every entry full
      if (NULL == branch[i].child)
        {
          std::cerr << "assertion false: RTreeNode::getBranches() branch " << i << ": is NULL\n";
          return false;
        }
      s.BranchBuf[i] = branch[i];
    }
  s.BranchBuf[RTREE_MAX_NODES] = b;

  // Calculate box containing all in the set
  s.CoverSplit = s.BranchBuf[0].box;
  for (uint32_t i = 1; i < RTREE_MAX_NODES + 1U; ++i)
    {
      s.CoverSplit = s.CoverSplit.merge(s.BranchBuf[i].box);
    }
  s.CoverSplitVolume = s.CoverSplit.volume();

  // Init node
  count = 0;
  level = 0;
  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      branch[i].child = NULL;
      branch[i].box = RTREE_DUMMY_BBOX;
    }

  return true;
}

/*
 * Method #0 for choosing a partition:
 * As the seeds for the two groups, pick the two rects that would waste the
 * most area if covered by a single rectangle, i.e. evidently the worst pair
 * to have in the same group.
 * Of the remaining, one at a time is chosen to be put in one of the two groups.
 * The one chosen is the one with the greatest difference in area expansion
 * depending on which group - the rect most strongly attracted to one group
 * and repelled from the other.
 * If one group gets too full (more would force other group to violate min
 * fill requirement) then other group gets the rest.
 * These last are the ones that can go in either group most easily.
 */
void RTreeNode::PartitionVars::methodZero(RTreeSpliter& s)
{
  float32_t growth0, growth1, diff, biggestDiff;
  uint32_t group;
  uint32_t chosen = 0;
  uint32_t betterGroup = 0;

  init();
  pickSeeds(s);

  while ((count[RTREE_PARTITION_0] + count[RTREE_PARTITION_1] < RTREE_MAX_NODES + 1U) &&
         (count[RTREE_PARTITION_0] < RTREE_MAX_NODES + 1U - RTREE_MIN_FILL) &&
         (count[RTREE_PARTITION_1] < RTREE_MAX_NODES + 1U - RTREE_MIN_FILL))
    {
      biggestDiff = -1.0f;
      for (uint32_t i = 0; i < RTREE_MAX_NODES + 1U; ++i)
        {
          if (!taken[i])
            {
              AABB *r = &(s.BranchBuf[i].box);
              growth0 = (r->merge(cover[RTREE_PARTITION_0])).volume() - volume[RTREE_PARTITION_0];
              growth1 = (r->merge(cover[RTREE_PARTITION_1])).volume() - volume[RTREE_PARTITION_1];

              diff = growth1 - growth0;
              if (diff >= 0)
                {
                  group = RTREE_PARTITION_0;
                }
              else
                {
                  group = RTREE_PARTITION_1;
                  diff = -diff;
                }

              if (diff > biggestDiff)
                {
                  biggestDiff = diff;
                  chosen = i;
                  betterGroup = group;
                }
              else if ((fabs(diff - biggestDiff) <= 0.0001f) && // diff == biggestDiff
                       (count[group] < count[betterGroup]))
                {
                  chosen = i;
                  betterGroup = group;
                }
            }
        }
      classify(chosen, betterGroup, s);
    }

  /* if one group too full, put remaining rects in the other */
  if (count[RTREE_PARTITION_0] + count[RTREE_PARTITION_1] < RTREE_MAX_NODES + 1U)
    {
      if (count[RTREE_PARTITION_0] >= RTREE_MAX_NODES + 1U - RTREE_MIN_FILL)
        group = 1;
      else
        group = 0;
      for (uint32_t i = 0; i < RTREE_MAX_NODES + 1U; ++i)
        {
          if (!taken[i])
            {
              classify(i, group, s);
            }
        }
    }
}

/*
 * Put a branch in one of the groups.
 */
void RTreeNode::PartitionVars::classify(const uint32_t i, const uint32_t group, RTreeSpliter& s)
{
  partition[i] = group;
  taken[i] = true;

  if (0 == count[group])
    {
      cover[group] = s.BranchBuf[i].box;
    }
  else
    {
      cover[group] = s.BranchBuf[i].box.merge(cover[group]);
    }
  volume[group] = cover[group].volume();
  count[group]++;
}

/*
 * Pick two rects from set to be the first elements of the two groups.
 * Pick the two that waste the most area if covered by a single
 * rectangle.
 */
void RTreeNode::PartitionVars::pickSeeds(RTreeSpliter& s)
{
  float32_t waste, worst;
  float32_t volume[RTREE_MAX_NODES + 1U];
  uint32_t seed0 = 0;
  uint32_t seed1 = 0;

  for (uint32_t i = 0; i < RTREE_MAX_NODES + 1U; ++i)
    {
      volume[i] = s.BranchBuf[i].box.volume();
    }

  worst = -s.CoverSplitVolume - 1.0f;
  for (uint32_t i = 0; i < RTREE_MAX_NODES; ++i)
    {
      for (uint32_t j = i + 1U; j < RTREE_MAX_NODES + 1U; ++j)
        {
          waste = (s.BranchBuf[i].box.merge(s.BranchBuf[j].box)).volume();
          waste = waste - volume[i] - volume[j];

          if (waste > worst)
            {
              worst = waste;
              seed0 = i;
              seed1 = j;
            }
        }
    }
  classify(seed0, RTREE_PARTITION_0, s);
  classify(seed1, RTREE_PARTITION_1, s);
}
