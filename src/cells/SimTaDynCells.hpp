#ifndef SIMTADYNCELLS_HPP_
#  define SIMTADYNCELLS_HPP_

#  include "classcounter.hpp"
#  include "handler.hpp"
#  include "BoundingBox.hpp"
#include <forward_list>
#include <algorithm>
#  include "Observer.hpp"

class EventMoved : public Event
{
public:
  EventMoved(SimTaDynCell* cell) { cell_ = cell; name_ = "EventMoved"; }
  SimTaDynCell* cell_;
};

class EventDeleted : public Event {};

// *************************************************************************************************
// This class is the basic ancestor class for SimTaDyn cells: Vertices (aka Nodes), Edges (aka Arcs)
// and Zones (aka Areas).
// *************************************************************************************************
class SimTaDynCell
  : private ClassCounter<SimTaDynCell>,
    public EventHandler,
    public Observer<SimTaDynCell, int>,
    public Observable<SimTaDynCell, int>
{
public:
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
   * Position in the map/view (pixel)
   */
  Position3D position_;

  /*
   * Spatialise
   */
  AABB box_;

  /*
   * Mark for already visited element when walking in the grpah
   */
  bool visited;

  /*
   * A string for storing Forth code
   */
  string code_forth_; // TODO: string& pour se referer au dictionnaire (au lieu de copier)

  int32_t value_;

  /*
   * A list of cells in direct relation (explicitly set in the graph editor.
   * For example: a node is connected to other nodes by a graph. Here we are
   * generic to use polymorphism.
   */
  std::forward_list<SimTaDynCell*> neighbors_;

public:
  /*
   * Creator
   */
  SimTaDynCell()
    : box_(AABB::AABB_ZERO)
  {
    id_ = howMany() - 1U;
    name = "Cell_" + std::to_string(id_);
    code_forth_ = "";
    data = NULL;
    selected = visited = false;
    value_ = 0;
  }

   /*
   * Creator
   */
  SimTaDynCell(string new_name, const string& new_code_forth, void *const new_data)
    : box_(AABB::AABB_ZERO)
  {
    id_ = howMany() - 1U;
    name = new_name;
    code_forth_ = new_code_forth;
    data = new_data;
    selected = visited = false;
    value_ = 0;
  }

  /*
   * Destructor: for each explicit/implicit relation linked with
   * 'this', prevent it to remove 'this' from its list.
   */
  virtual ~SimTaDynCell()
  {
  }
#if 0
  virtual ~SimTaDynCell()
  {
    std::forward_list<SimTaDynCell*>::iterator neighbor;

    whoAmI();
    cout << "  I'm on destruction. I'll remove myself from my neighbors list: {" << endl;
    for (neighbor  = neighbors_.begin(); // non observers
         neighbor != neighbors_.end();
         ++neighbor)
      {
        cout << "  Please neighor " << (*neighbor_)->name << " remove me (" << this << ")" << endl;
        (*neighbor)->removeExplicitNeighbor(this);
      }
    cout << "}" << endl;
  }

  /*
   * Observable design pattern: a cell notify all its neighbor from an event
   */
  virtual void notifyNeighbors(Event const* event)
  {
    std::forward_list<SimTaDynCell*>::iterator neighbor;

    try
      {
        whoAmI();
        cout << "  I notify all my neighbors from the event " << event->getName() << " : {" << endl;
        for (neighbor  = neighbors_.begin();
             neighbor != neighbors_.end();
             ++neighbor)
          {
            cout << "  Please neighor " << (*neighbor)->name << " do your action:" << endl;
            (*neighbor)->handleEvent(event);
          }
        cout << "}" << endl;
      }
    catch (const std::out_of_range& oor)
      {
        cerr << "Error: no callback have been linked with this event" << endl;
        return ;
      }
  }

  /*
   * Move the cell to a new location and notify all neighbors the new
   * position.
   */
  virtual void moveToPosition(const Position3D& p)
  {
    if (position_ != p)
      {
        position_ = p;
        EventMoved m(this);
        notifyNeighbors(&m);
    }
  }

  /*
   * Callback when a neighbor moved and prevented us.
   */
  virtual void onMoved(const EventMoved* movement)
  {
    cout << "    * "; whoAmI();
    cout << "      My neighbor the cell #" << movement->cell_->privateId() << " nammed \"" << movement->cell_->name
         << "\" moved to position " << movement->cell_->position_ << endl;
  }
#endif

  /*
   * Accessor. Return the unique identifier
   */
  inline Key privateId() const
  {
    return id_;
  }

  /*
   * Accessor. Return the current position
   */
  const Position3D& getPosition() const
  {
    return position_;
  }

  /*
   * Accessor. Return the current boundingbox
   */
  const AABB& getBoundingBox() const
  {
    return box_;
  }

  /*
   * For graph traversal to avoid iterating on the same visited cell
   */
  inline void setVisited(bool value)
  {
    visited = value;
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
  }

  /*
   * Accessor
   */
  int32_t getValue() const
  {
    return value_;
  }

  void setValue(int32_t value)
  {
    value_ = value;
    //setChanged();
    notifyObservers(value);
  }

  void setValue()
  {
    if ((code_forth_ != std::string("")) && (0 != nbNeighbors()))
      {
        setValue(forth_eval());
      }
  }

  /*
   * Observer pattern
   */
  void update(SimTaDynCell& subject, int arg) override
  {
    (void) subject;
    (void) arg;
    setValue();
  }

  virtual void notifyObservers(int arg)
  {
    typename std::vector<Observer<SimTaDynCell, int> *>::const_iterator it;

    std::cerr << "The Cell #" << this->name << " notifyObservers:" << std::endl;;
    for (it = observers_.begin(); it != observers_.end(); it++)
      {
        std::cerr << "  " << (static_cast<SimTaDynCell *>(*it))->name << std::endl;
        (*it)->update(*(static_cast<SimTaDynCell *>(this)), arg);
      }
  }


  /*
   * Static member returning the number of SimTaDynCell instances created
   */
  using ClassCounter<SimTaDynCell>::howMany;
  static Key howMany()
  {
    return ClassCounter<SimTaDynCell>::howMany();
  }

  inline friend bool operator<(const SimTaDynCell& a, const SimTaDynCell& b);
  inline friend bool operator<=(const SimTaDynCell& a, const SimTaDynCell& b);
  inline friend bool operator>(const SimTaDynCell& a, const SimTaDynCell& b);
  inline friend bool operator>=(const SimTaDynCell& a, const SimTaDynCell& b);
  inline friend bool operator==(const SimTaDynCell& a, const SimTaDynCell& b);
  inline friend bool operator!=(const SimTaDynCell& a, const SimTaDynCell& b);

  inline friend bool operator<(const SimTaDynCell& a, const Key b);
  inline friend bool operator<=(const SimTaDynCell& a, const Key b);
  inline friend bool operator>(const SimTaDynCell& a, const Key b);
  inline friend bool operator>=(const SimTaDynCell& a, const Key b);
  inline friend bool operator==(const SimTaDynCell& a, const Key b);
  inline friend bool operator!=(const SimTaDynCell& a, const Key b);

  virtual inline void addCell(SimTaDynCell* const cell)
  {
    cell->attachObserver(*this);
    //if (false == IsNeighborOf(cell))
      {
        neighbors_.push_front(cell);
      }
  }
  virtual inline void removeExplicitNeighbor(SimTaDynCell* const c)
  {
    cout << "removeExplicitNeighbor " << c << std::endl;
    neighbors_.remove(c);
    c->neighbors_.remove(this);
    cout << "removeED" << std::endl;
  }
  virtual inline uint32_t nbNeighbors()
  {
    uint32_t count = 0U;
    std::forward_list<SimTaDynCell*>::iterator neighbor;
    for (neighbor = neighbors_.begin();
         neighbor != neighbors_.end(); ++neighbor)
      {
        count++;
      }
    return count;
  }
  virtual inline bool IsExplicitNeighborOf(SimTaDynCell* const c) const
  {
    return (std::find(neighbors_.begin(), neighbors_.end(), c) != neighbors_.end());
  }
  virtual inline bool IsExplicitNeighborOf(const Key c)
  {
    std::forward_list<SimTaDynCell*>::iterator neighbor;
    for (neighbor = neighbors_.begin();
         neighbor != neighbors_.end(); ++neighbor)
      {
        if ((*neighbor)->id_ == c)
          return true;
      }
    return false;
  }

  int32_t forth_eval()
  {
    int32_t eval = getValue();
    std::cerr << "eval = " << getValue() << std::endl;
    size_t last = 0;
    size_t next = 0;
    std::string delimiter = " ";

    while ((next = code_forth_.find(delimiter, last)) != string::npos)
      {
        std::cerr << code_forth_.substr(last, next - last) << std::endl;
        last = next + 1;
      }
    std::cerr << code_forth_.substr(last) << std::endl;

    std::forward_list<SimTaDynCell*>::iterator neighbor;
    for (neighbor = neighbors_.begin();
         neighbor != neighbors_.end(); ++neighbor)
      {
        std::cerr << "eval += " << (*neighbor)->getValue() << std::endl;
        eval += (*neighbor)->getValue();
      }
    return eval;
  }

  /*
   * Debug
   */
  virtual void showExplicitNeighbor()
  {
    std::forward_list<SimTaDynCell*>::iterator neighbor;

    whoAmI();
    cout << "  My explicit neighbors are: {" << endl;
    for (neighbor = neighbors_.begin();
         neighbor != neighbors_.end(); ++neighbor)
      {
        cout << "  \"" << (*neighbor)->name << "\"" << endl;
      }
    cout << "}" << endl;
  }

  /*
   * Debug
   */
  virtual Key whoAmI()
  {
    cout << "I am the SimTaDynCell #" << id_ << " nammed \"" << name << "\":" << endl;
    return id_;
  }
};

inline bool operator<(const SimTaDynCell& a, const SimTaDynCell& b)
{
  return a.id_ < b.id_;
}

inline bool operator<=(const SimTaDynCell& a, const SimTaDynCell& b)
{
  return a.id_ <= b.id_;
}

inline bool operator>(const SimTaDynCell& a, const SimTaDynCell& b)
{
  return a.id_ > b.id_;
}

inline bool operator>=(const SimTaDynCell& a, const SimTaDynCell& b)
{
  return a.id_ >= b.id_;
}

inline bool operator==(const SimTaDynCell& a, const SimTaDynCell& b)
{
  return a.id_ == b.id_;
}

inline bool operator!=(const SimTaDynCell& a, const SimTaDynCell& b)
{
  return a.id_ != b.id_;
}

inline bool operator<(const SimTaDynCell& a, const Key b)
{
  return a.id_ < b;
}

inline bool operator<=(const SimTaDynCell& a, const Key b)
{
  return a.id_ <= b;
}

inline bool operator>(const SimTaDynCell& a, const Key b)
{
  return a.id_ > b;
}

inline bool operator>=(const SimTaDynCell& a, const Key b)
{
  return a.id_ >= b;
}

inline bool operator==(const SimTaDynCell& a, const Key b)
{
  return a.id_ == b;
}

inline bool operator!=(const SimTaDynCell& a, const Key b)
{
  return a.id_ != b;
}

#endif /* CELLS_HPP_ */
