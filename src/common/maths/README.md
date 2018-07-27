Files:

Note: not all of these routines are useful for the SimTaDyn project.

* Vector.tpp: Template class for defining vector of any dimension and
  of any kind of type (integer, float, double ...)

* Matrix.tpp: Template class for defining matrices (not parsed
  matrices) of any dimension and of any kind of type (integer, float,
  double ...).

* Transformation.tpp: Template class for matrix transformation in
  space: rotation, perspective, ... in the same way than GLM library
  (OpenGL Mathematics).

* BoundingBox.tpp: Template class for 2D or 3D boxes, checking
  intersection between them, ... This technic is used in games for
  speed up collisions or mouse picking tests between complex geometry
  objects by gatting a coarse detection.

* Filtering.hpp: different kind of filters (low pass, window averaging).

* Polyfit.hpp: Polynomial fitness: find the polynom passing in a cloud
  of points.

* Maths.[cpp]: Abstract mathematical libraries (like std::),
  conversions or float comparaison.
