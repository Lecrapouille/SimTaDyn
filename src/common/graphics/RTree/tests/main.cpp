//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "RTree.hpp"

const Vector3D Vector3D::DUMMY(NAN, NAN, NAN);
const Vector3D Vector3D::ZERO(0.0f, 0.0f, 0.0f);
const Vector3D Vector3D::UNIT_X(1.0f, 0.0f, 0.0f);
const Vector3D Vector3D::UNIT_Y(0.0f, 1.0f, 0.0f);
const Vector3D Vector3D::UNIT_Z(0.0f, 0.0f, 1.0f);
const Vector3D Vector3D::NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f);
const Vector3D Vector3D::NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f);
const Vector3D Vector3D::NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f);
const Vector3D Vector3D::UNIT_SCALE(1.0f, 1.0f, 1.0f);
const Vector3D Vector3D::NEGATIVE_UNIT_SCALE(-1.0f, -1.0f, -1.0f);

const AABB AABB::DUMMY(Vector3D::DUMMY, Vector3D::DUMMY);
const AABB AABB::ZERO(Vector3D::ZERO, Vector3D::ZERO);
const AABB AABB::UNIT_SCALE(Vector3D::NEGATIVE_UNIT_SCALE / 2.0f, Vector3D::UNIT_SCALE / 2.0f);
const AABB AABB::INFINITE(std::numeric_limits<float32_t>::lowest(),
                          std::numeric_limits<float32_t>::lowest(),
                          std::numeric_limits<float32_t>::lowest(),
                          std::numeric_limits<float32_t>::max(),
                          std::numeric_limits<float32_t>::max(),
                          std::numeric_limits<float32_t>::max());

int main(void)
{
  uint32_t tid = 1;

  AABB b1 = AABB::UNIT_SCALE;
  AABB b2 = AABB::UNIT_SCALE * 4.0f;
  AABB b3 = AABB(Vector3D(2.0f, 2.0f, 2.0f), Vector3D(3.0f, 3.0f, 3.0f));

  RTreeNode* root = new RTreeNode(RTREE_LEAF);
  root = root->insert(++tid, b1, 0);
  root = root->insert(++tid, b2, 0);
  root = root->insert(++tid, b3, 0);
  root = root->insert(++tid, b3+1.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE * 6.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE / 6.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 6.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 6.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 15.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 10.0f, 0);

  root = root->insert(++tid, AABB::UNIT_SCALE * 6.0f+1.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE / 6.0f+1.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 6.0f+1.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 6.0f+1.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 15.0f+1.0f, 0);
  root = root->insert(++tid, AABB::UNIT_SCALE + 10.0f+1.0f, 0);

  std::cout << *root << std::endl;

  std::cout << root->search(AABB::UNIT_SCALE + 10.0f) << std::endl;

  /*std::cout << "REMOVING\n";
  root = root->remove(tid, b3+1.0f);
  std::cout << *root << std::endl;*/

  #if 0

  //RTreeBranch* branch = new RTreeBranch(AABB::INFINITE);
  //std::cout << *branch << std::endl;

  AABB b1 = AABB::INFINITE;
  AABB b2 = AABB::UNIT_SCALE;

  std::cout << b1.contains(b2) << std::endl;


  RTreeNode* root = new RTreeNode();
  root->branch[3].box = AABB::UNIT_SCALE;
  root->branch[3].child = root;
  root->branch[1].box = AABB::UNIT_SCALE * 4.0f;
  root->branch[1].child = root;
  root->branch[2].box = AABB(Vector3D::NEGATIVE_UNIT_SCALE/2.0f, Vector3D::ZERO);
  root->branch[2].child = root;
  root->branch[0].box = AABB::ZERO;
  root->branch[0].child = root;
  std::cout << *root << std::endl;

  std::cout << root->search(b2) << std::endl;

  /*RTreeNode* newroot;
  RTreeBranch* branch = new RTreeBranch(AABB::INFINITE);
  newroot = root->addBranch(*branch);

  if (NULL == newroot)
    std::cout << "NULL" << std::endl;
  std::cout << *root << std::endl;
  */
  #endif

  return 0;
}
