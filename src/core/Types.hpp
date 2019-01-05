#ifndef TYPES_HPP
#define TYPES_HPP

//------------------------------------------------------------------
//! \brief Define a 3D SceneGraph node (matrix 4x4 of float + VAO)
//------------------------------------------------------------------
class SimTaDynMap;
using SceneGraph       = SceneGraph_t<std::string, SimTaDynSheet, float, 3u>;
using SceneNode        = SceneGraph::Node;
using SceneNodePtr     = std::shared_ptr<SceneNode>;
using SimTaDynSheetPtr = std::shared_ptr<SimTaDynSheet>;
using SimTaDynMapPtr   = std::shared_ptr<SimTaDynMap>;

#endif
