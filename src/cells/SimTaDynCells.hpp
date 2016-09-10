#ifndef SIMTADYNCELLS_HPP_
#  define SIMTADYNCELLS_HPP_

#  include "ClassCounter.hpp"
#  include "SimTaDynHandlers.hpp"
#  include "BoundingBox.hpp"
#  include "Observer.hpp"

class EventMoved : public Event
{
public:
  EventMoved(SimTaDynCell* cell)
  {
    cell_ = cell;
    name_ = "EventMoved";
  }
  SimTaDynCell* cell_;
};

class EventChanged : public Event
{
public:
  EventChanged(SimTaDynCell* cell)
  {
    cell_ = cell;
    name_ = "EventChanged";
  }
  SimTaDynCell* cell_;
};

class EventDestroyed : public Event
{
public:
  EventDestroyed(SimTaDynCell* cell)
  {
    cell_ = cell;
    name_ = "EventDestroyed";
  }
  SimTaDynCell* cell_;
};

// *************************************************************************************************
// This class is the basic ancestor class for SimTaDyn cells: Vertices (aka Nodes), Edges (aka Arcs)
// and Zones (aka Areas).
//
// Design pattern: SimTaDynCell is both time an observer and
// observable (it notifies when the formula changed) and is prevent
// when the result of another cell is changed. Inspiration from:
// http://www.cs.sjsu.edu/~pearce/modules/cases/ss/ss.htm
// *************************************************************************************************
class SimTaDynCell
  : private ClassCounter<SimTaDynCell>,
    public Observer<SimTaDynCell, const Event*>,
    public Observable<SimTaDynCell, const Event*>,
    public EventHandler
{
public:
  /*
   * Idee: liste chainee de SimTaDynCell ? avec next ?
   */
  //SimTaDynCell* next;

  /*
   * Give a name to the element which will be displayed in the GUI.
   * Contrary to id_ several cells can have the same name. By default
   * the name is unique.
   */
  string name;

  /*
   * The following pointer can point to any extra data needed by the
   * cell. This field is used by Forth to extend the implementation
   */
  void *data;

  /*
   * Mark when the user select a set of element to apply operation on them
   */
  bool selected;

protected:
  /*
   * Make this instance unique with this identifier.
   * Used it for comparing element in a container.
   */
  Key id_;

  /*
   * Position in the map/view (pixel). Set as protected to be sure to
   * use accessors which can prevent other cells that this cell moved.
   */
  Position3D position_;

  /*
   * Bounding box for spatialization. Set as protected to be sure to
   * use accessors which can prevent other cells that this cell changed.
   */
  AABB box_;

  /*
   * Mark for already visited element when walking in the grpah
   */
  bool visited;

  /*
   * A string for storing Forth code
   */
  std::string code_forth_;

  /*
   * Pour le moment resultat d'une operation facon Excel
   */
  int32_t value_;
  // bool used_formula;ou empty_formula // si true: affiche la valeur dans la gui sinon affiche rien

  /*
   * Observables
   */
  //std::vector<SimTaDynCell*> subjects_; // FIXME: trouver meilleur container

public:
  /*
   * Creator
   */
  SimTaDynCell()
    : box_(AABB::DUMMY)
  {
    id_ = howMany() - 1U;
    name = "Cell_" + std::to_string(id_);
    code_forth_ = "";

    data = NULL;
    selected = visited = false;
    value_ = 0;
    registerEventFunc(this, &SimTaDynCell::onMoved);
    registerEventFunc(this, &SimTaDynCell::onFormulaChanged);
    registerEventFunc(this, &SimTaDynCell::onDestruction);
  }

   /*
   * Creator
   */
  SimTaDynCell(string new_name, const string& new_code_forth = "", void *const new_data = NULL)
    : box_(AABB::DUMMY)
  {
    id_ = howMany() - 1U;
    name = new_name;
    code_forth_ = new_code_forth;
    data = new_data;
    selected = visited = false;
    value_ = 0;
    registerEventFunc(this, &SimTaDynCell::onMoved);
    registerEventFunc(this, &SimTaDynCell::onFormulaChanged);
    registerEventFunc(this, &SimTaDynCell::onDestruction);
  }

  /*
   * Destructor: for each explicit/implicit relation linked with
   * 'this', prevent it to remove 'this' from its list.
   */
  virtual ~SimTaDynCell()
  {
    //std::cout << "Destroying\n";
    EventDestroyed m(this);
    notifyObservers(&m);
  }

  virtual void onDestruction(const EventDestroyed* destruction)
  {
    std::cout << "\nonDestruction: " << std::endl;
    whoAmI();
    std::cout << "My neighbor the cell #" << destruction->cell_->privateId() << " nammed \"" << destruction->cell_->name
              << "\" (" << destruction->cell_ << ") is destroying" << std::endl;
    //detachObserver(*(destruction->cell_));
    //removeImplicitRelation(*(destruction->cell_));
    destruction->cell_->removeImplicitRelation(*this);
  }

  /*
   * Observer pattern
   */
  void debugDisplayAllObservers()
  {
    std::cout << "--- debugDisplayAllObservers-------------\n";
    whoAmI();
    typename std::vector<Observer<SimTaDynCell, const Event*> *>::const_iterator it;
    std::cout << "Observers list:" << std::endl;
    for (it = observers_.begin(); it != observers_.end(); it++)
      {
        SimTaDynCell *cell = static_cast<SimTaDynCell *>(*it);
        std::cout << "  " << cell->name << std::endl;

        typename std::vector<Observable<SimTaDynCell, const Event*> *>::const_iterator it3;
        std::cout << "     Observables list:" << std::endl;
        for (it3 = cell->observed_.begin(); it3 != cell->observed_.end(); it3++)
          {
            std::cout << "       " << (static_cast<SimTaDynCell *>(*it3))->name << std::endl;
          }
      }

    typename std::vector<Observable<SimTaDynCell, const Event*> *>::const_iterator it4;
    std::cout << "Observables list:" << std::endl;
    for (it4 = observed_.begin(); it4 != observed_.end(); it4++)
      {
        std::cout << "  " << (static_cast<SimTaDynCell *>(*it4))->name << std::endl;
      }

    /*
    typename std::vector<SimTaDynCell*>::const_iterator it2;
    std::cout << "Adjacence list:" << std::endl;
    for (it2 = subjects_.begin(); it2 != subjects_.end(); it2++)
      {
        std::cout << "  " << (*it2)->name << std::endl;
        }*/
  }

  /*
   * Observer pattern
   */
  void update(SimTaDynCell& neighbor, const Event* event) override
  {
    (void) neighbor;
    handleEvent(event);
  }

  /*
   * Move the cell to a new location and notify all neighbors the new
   * position.
   */
  virtual void moveToPosition(const Position3D& p)
  {
    std::cout << "MoveTo\n";
    if (position_ != p)
      {
        position_ = p;
        EventMoved m(this);
        notifyObservers(&m);
      }
  }

  /*
   * Callback when a neighbor moved and prevented us.
   */
  virtual void onMoved(const EventMoved* movement)
  {
    std::cout << "\nonMoved: " << std::endl;
    whoAmI();
    std::cout << "My neighbor the cell #" << movement->cell_->privateId() << " nammed \"" << movement->cell_->name
              << "\" moved to position " << movement->cell_->position_ << std::endl;
  }

  /*
   * Accessor. Return the current position
   */
  const Position3D& getPosition() const
  {
    return position_;
  }

  /*
   * Accessor: return the result of the code Forth
   */
  int32_t getFormulaValue() const
  {
    return value_;
  }

  /*
   * Accessor: set a value instead of the code forth result.  FIXME:
   * ok pour le moment mais idealement il faudrait code_forth =
   * tostring(value) car si on a un code forth "42" et value_ 3 on a
   * conflit ??
   */
  void setFormulaValue(int32_t value)
  {
    if (value_ != value)
      {
        value_ = value;
        EventChanged m(this);
        notifyObservers(&m);
      }
  }

  /*
   * Accessor
   */
  /*void evalFormula(CellForth& forth)
  {
    if (code_forth_ != std::string(""))
      {
        if (forth.eatString(code_forth_))
          {
            setFormulaValue(forth.result);
          }
      }
      }*/

  /*
   * Callback when a neighbor changed its value
   */
  virtual void onFormulaChanged(const EventChanged* changement)
  {
    std::cout << "\nonFormulaChanged: " << std::endl;
    whoAmI();
    cout << " My neighbor the cell #" << changement->cell_->privateId() << " nammed \"" << changement->cell_->name
         << "\" Changed value " << changement->cell_->getFormulaValue() << endl;
  }

  /*
   * Accessor
   */
  const string& getCodeForth() const
  {
    return code_forth_;
  }

  /*
   * Accessor
   */
  void setCodeForth(const string& code_forth)
  {
    code_forth_ = code_forth;
    //evalFormula();
  }

  virtual inline void addImplicitRelation(SimTaDynCell& cell)
  {
    // Observable add Observer
    this->attachObserver(cell);

    // Observer add Observable
    //cell.subjects_.push_back(this);
  }

  virtual inline void removeImplicitRelation(SimTaDynCell& cell)
  {
    std::cout << "removeImplicitRelation " << this << " --> " << &cell << std::endl;
    this->detachObserver(cell);

    /*typename std::vector<SimTaDynCell*>::const_iterator it;
    it = std::find(cell.subjects_.begin(), cell.subjects_.end(), this);
    if (it != cell.subjects_.end())
      {
        std::cout << "Remove Implicit relation (subject list) " << (*it)->name << std::endl;
        cell.subjects_.erase(it);
        }*/
  }

  virtual inline void addExplicitRelation(SimTaDynCell& cell)
  {
    // Observable add Observer
    cell.attachObserver(*this);

    // Observer add Observable
    //subjects_.push_back(&cell);
  }

  virtual inline void removeExplicitRelation(SimTaDynCell& cell)
  {
    cell.detachObserver(*this);

    /*typename std::vector<SimTaDynCell*>::const_iterator it;
    it = std::find(subjects_.begin(), subjects_.end(), &cell);
    if (it != subjects_.end())
      {
        std::cout << "Remove Explicit relation (subject list) " << (*it)->name << std::endl;
        subjects_.erase(it);
        }*/
  }

  /*
   * Accessor. Return the current boundingbox
   */
  const AABB& getBoundingBox() const
  {
    return box_;
  }

  /*
   * Accessor. Return the unique identifier
   */
  inline Key privateId() const
  {
    return id_;
  }

  /*
   * For graph traversal to avoid iterating on the same visited cell
   */
  inline void setVisited(bool value)
  {
    visited = value;
  }

  inline bool getVisited() const
  {
    return visited;
  }

  /*
   * Instances counter
   */
  static Key howMany()
  {
    return ClassCounter<SimTaDynCell>::howMany();
  }

  /*
   * For debug
   */
  virtual Key whoAmI()
  {
    std::cout << "I am the SimTaDynCell #" << id_ << " nammed \"" << name << "\" (" << this << "):" << std::endl;
    return id_;
  }
};

#endif /* CELLS_HPP_ */
