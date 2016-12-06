#ifndef SIMTADYNEVENTS_HPP_
#  define SIMTADYNEVENTS_HPP_

#  include "SimTaDynCells.hpp"
#  include "SimTaDynHandlers.hpp"


class SimTaDynEvent: public Event
{
public:
  SimTaDynEvent()
  {
    name_ = "Event";
    cell_ = NULL;
  }
  const std::string& getName() const
  {
    return name_;
  }
  const SimTaDynCell* getCell() const
  {
    return cell_;
  }

protected:
  std::string name_;
  const SimTaDynCell* cell_;
};

// **************************************************************
//
// **************************************************************
class EventSimTaDynCellMoved : public SimTaDynEvent
{
public:
  EventSimTaDynCellMoved(const SimTaDynCell* cell)
  {
    cell_ = cell;
    name_ = "EventSimTaDynMoved";
  }
};

// **************************************************************
//
// **************************************************************
class EventSimTaDynContentChanged : public SimTaDynEvent
{
public:
  EventSimTaDynContentChanged(const SimTaDynCell* cell)
  {
    cell_ = cell;
    name_ = "EventSimTaDynContentChanged";
  }
};

// **************************************************************
//
// **************************************************************
class EventDestroyed : public SimTaDynEvent
{
  EventDestroyed(const SimTaDynCell* cell)
  {
    cell_ = cell;
    name_ = "EventSimTaDynDestroyed";
  }
};

#endif /* SIMTADYNEVENTS_HPP_ */
