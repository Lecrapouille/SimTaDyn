#ifndef SIMTADYNCELLS_HPP_
#  define SIMTADYNCELLS_HPP_

#  include "classcounter.hpp"
#  include "handler.hpp"
#  include "BoundingBox.hpp"

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
class SimTaDynCell: private ClassCounter<SimTaDynCell>, public EventHandler
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
  string code_forth; // TODO: string& pour se referer au dictionnaire (au lieu de copier)

  /*
   * A list of cells in direct relation (explicitly set in the graph editor.
   * For example: a node is connected to other nodes by a graph. Here we are
   * generic to use polymorphism.
   */
  set<SimTaDynCell*> explicit_neighbors;

public:
  /*
   * Creator
   */
  SimTaDynCell()
    : box_(AABB::AABB_ZERO)
  {
    id_ = howMany() - 1U;
    name = "Cell_" + std::to_string(id_);
    code_forth = "";
    data = NULL;
    selected = visited = false;
  }

   /*
   * Creator
   */
  SimTaDynCell(string new_name, const string& new_code_forth, void *const new_data)
    : box_(AABB::AABB_ZERO)
  {
    id_ = howMany() - 1U;
    name = new_name;
    code_forth = new_code_forth;
    data = new_data;
    selected = visited = false;
  }

  /*
   * Destructor: for each explicit/implicit relations linked with,
   * prevent it to remove from its list.
   */
  virtual ~SimTaDynCell()
  {
    set<SimTaDynCell*>::iterator neighbor;

    whoAmI();
    //cout << "  I'm on destruction. I'll remove myself from my neighbors list: {" << endl;
    for (neighbor  = explicit_neighbors.begin();
         neighbor != explicit_neighbors.end();
         ++neighbor)
      {
        //cout << "  Please neighor " << (*neighbor)->name << " remove me !" << endl;
        (*neighbor)->removeExplicitNeighbor(this);
        // (*neighbor)->removeImplicitRelation(this);
      }
    //cout << "}" << endl;
  }

  /*
   * Observable design pattern: a cell notify all its neighbor from an event
   */
  virtual void notifyNeighbors(Event const* event)
  {
    //NotifyAction ret;
    set<SimTaDynCell*>::iterator neighbor;

    try
      {
        cout << __PRETTY_FUNCTION__ << ":" << endl;
        whoAmI();
        cout << "  I notify all my neighbors from the event " << event->getName() << " : {" << endl;
        for (neighbor  = explicit_neighbors.begin();
             neighbor != explicit_neighbors.end();
             ++neighbor)
          {
            cout << "  Please neighor " << (*neighbor)->name << " do your action:" << endl;
            // ret = (*neighbor)->onNotify(*this, event);
            //ret =
              (*neighbor)->handleEvent(event);
              /*
            if (NotifyAction::Done == ret)
              {
                cout << "    Success" << endl;
              }
            else
              {
                cout << "    Ignored" << endl;
              }*/
          }
        cout << "}" << endl;
      }
    catch (const std::out_of_range& oor)
      {
        cerr << "Error: no callback have been linked with this event" << endl;
        return ;
      }
  }

  inline void setVisited(bool value)
  {
    visited = value;
  }

  inline Key privateId() const
  {
    return id_;
  }

  /*
   * Static member returning the number of SimTaDynCell instances created
   */
  using ClassCounter<SimTaDynCell>::howMany;
  static Key howMany()
  {
    return ClassCounter<SimTaDynCell>::howMany();
  }

#if 0
  /*
   * Order position in containers with operator == and <
   */
  inline bool operator==(const SimTaDynCell& a)
  {
    return a.id_ == this->id_;
  }
  inline bool operator!=(const SimTaDynCell& a)
  {
    return a.id_ != this->id_;
  }
  inline bool operator<(const SimTaDynCell& a)
  {
    return a.id_ < this->id_;
  }
  inline bool operator<=(const SimTaDynCell& a)
  {
    return a.id_ <= this->id_;
  }
  inline bool operator>(const SimTaDynCell& a)
  {
    return a.id_ > this->id_;
  }
  inline bool operator>=(const SimTaDynCell& a)
  {
    return a.id_ >= this->id_;
  }

  inline bool operator==(const Key a)
  {
    return a == this->id_;
  }
  inline bool operator!=(const Key a)
  {
    return a != this->id_;
  }
  inline bool operator<(const Key a)
  {
    return a < this->id_;
  }
  inline bool operator<=(const Key a)
  {
    return a <= this->id_;
  }
  inline bool operator>(const Key a)
  {
    return a > this->id_;
  }
  inline bool operator>=(const Key a)
  {
    return a >= this->id_;
  }
#endif
  inline friend bool operator<(const SimTaDynCell& a, const SimTaDynCell& b);
  inline friend bool operator==(const SimTaDynCell& a, const SimTaDynCell& b);

  //inline friend bool operator<(const SimTaDynCell& a, const Key& b);
  //inline friend bool operator==(const SimTaDynCell& a, const Key& b);

  inline friend bool operator<(const SimTaDynCell* a, const Key b);
  inline friend bool operator==(const SimTaDynCell* a, const Key b);

  inline friend bool operator<(const SimTaDynCell& a, const Key b);
  inline friend bool operator==(const SimTaDynCell& a, const Key b);


  virtual inline void addExplicitNeighbor(SimTaDynCell *c)
  {
    explicit_neighbors.insert(c);
  }
  virtual inline void removeExplicitNeighbor(SimTaDynCell *c)
  {
    explicit_neighbors.erase(c);
  }
  virtual inline uint32_t nbExplicitNeighbors()
  {
    return explicit_neighbors.size();
  }
  virtual inline bool IsExplicitNeighborOf(SimTaDynCell *c) const
  {
    return explicit_neighbors.find(c) != explicit_neighbors.end();
  }
  virtual inline bool IsExplicitNeighborOf(const Key c) const
  {
    return explicit_neighbors.find(c) != explicit_neighbors.end();
  }

  virtual void moveToPosition(const Position3D& p)
  {
    if (position_ != p)
      {
        position_ = p;
        EventMoved m(this);
        notifyNeighbors(&m);
    }
  }

  virtual void onMoved(const EventMoved* movement)
  {
    cout << "    * "; whoAmI();
    cout << "      My neighbor the cell #" << movement->cell_->privateId() << " nammed \"" << movement->cell_->name
         << "\" moved to position " << movement->cell_->position_ << endl;
  }

  const Position3D& getPosition() const
  {
    return position_;
  }

  const AABB& getBoundingBox() const
  {
    return box_;
  }

  /*
   * Debug
   */
  virtual void showExplicitNeighbor()
  {
    whoAmI();
    cout << "  My explicit neighbors are: {" << endl;
    for (set<SimTaDynCell*>::iterator it = explicit_neighbors.begin();
         it != explicit_neighbors.end(); ++it)
      {
        cout << "  \"" << (*it)->name << "\"" << endl;
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

inline bool operator<(const SimTaDynCell& a, const SimTaDynCell& b){ return a.id_ < b.id_; }
inline bool operator==(const SimTaDynCell& a, const SimTaDynCell& b){ return a.id_ == b.id_; }

//inline bool operator<(const SimTaDynCell& a, const Key& b){ return a.id_ < b; }
//inline bool operator==(const SimTaDynCell& a, const Key& b){ return a.id_ == b; }

inline bool operator<(const SimTaDynCell& a, const Key b){ return a.id_ < b; }
inline bool operator==(const SimTaDynCell& a, const Key b){ return a.id_ == b; }

inline bool operator<(const SimTaDynCell* a, const Key b){ return a->id_ < b; }
inline bool operator==(const SimTaDynCell* a, const Key b){ return a->id_ == b; }

#endif /* CELLS_HPP_ */
