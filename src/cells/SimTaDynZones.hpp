#ifndef ZONES_HPP_
#  define ZONES_HPP_

// *************************************************************************************************
// Define an Zone (aka Area)
// ************************************************************************************************
class SimTaDynZone: public SimTaDynCell, private ClassCounter<SimTaDynArc>
{

};

#define SimTaDynZone SimTaDynArea

#endif /* ZONES_HPP_ */
