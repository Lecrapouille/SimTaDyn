#ifndef CELLS_HPP_
#  define CELLS_HPP_

#  include "classcounter.hpp"
#  include "handler.hpp"
#  include "position.hpp"

class EventMoved : public Event
{
public:
  EventMoved(SimTaDynNode* cell) { cell_ = cell; name_ = "EventMoved"; }
  //  SimTaDynCell* getCell() const { return cell_; }
  //protected:
  SimTaDynNode* cell_;
};

class EventDeleted : public Event {};

// *************************************************************************************************
// This class is the basic ancestor class for SimTaDyn cells: Vertices (aka Nodes), Edges (aka Arcs)
// and Zones (aka Areas).
// *************************************************************************************************
class SimTaDynCell: private ClassCounter<SimTaDynCell>, public EventHandler
{
private:
  /*
   * Why not to avoid to explicitly create a new one every time we need
   * to iterate on a loop;
   */
  set<SimTaDynCell*>::iterator neighbor;

  //EventHandler events_handler_;

protected:
  /*
   * Make this instance unique with this identifier.
   * Used it for comparing element in a container.
   */
  Key id_;

  /*
   * A list of cells in direct relation (explicitly set in the graph editor.
   * For example: a node is connected to other nodes by a graph. Here we are
   * generic to use polymorphism.
   */
  set<SimTaDynCell*> explicit_neighbors;

  /*
   * A list of cells indirectly linked to this cell. For example a Forth
   * could be CELL_01 = @CELL_02 + 10. When CELL_02 changes CELL_01 should
   * also.
   */
  //set<SimTaDynCell*> implicit_neighbors;

public:
  /*
   * A string for storing Forth code
   */
  string code_forth; // FIXME string& code_forth; ?

  /*
   * Mark for already visited element when walking in the grpah
   */
  bool visited;

  /*
   * Mark when the user select a set of element to apply operation on them
   */
  bool selected;

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
  void *extra_data;

  Position3D position_;

  /*
   * Static member returning the number of SimTaDynCell instances created
   */
  using ClassCounter<SimTaDynCell>::howMany;
  static size_t howMany() { return ClassCounter<SimTaDynCell>::howMany(); }

  /*
   * Creator
   */
  SimTaDynCell() { id_ = howMany(); name = "Cell_" + to_string(id_); }
  SimTaDynCell(string new_name, string new_code_forth = "1 1 + .")
  {
    id_ = howMany();
    name = new_name;
    code_forth = new_code_forth;
  }

  /*
   * Destructor: for each explicit/implicit relations linked with,
   * prevent it to remove from its list.
   */
  virtual ~SimTaDynCell()
  {
    whoAmI();
    cout << "  I'm on destruction. I'll remove myself from my neighbors list: {" << endl;
    for (neighbor  = explicit_neighbors.begin();
         neighbor != explicit_neighbors.end();
         ++neighbor)
      {
        cout << "  Please neighor " << (*neighbor)->name << " remove me !" << endl;
        (*neighbor)->removeExplicitNeighbor(this);
        // (*neighbor)->removeImplicitRelation(this);
      }
    cout << "}" << endl;
  }

  /*
   * Observable design pattern: a cell notify all its neighbor from an event
   */
  void notifyNeighbors(Event const* event)
  {
    //NotifyAction ret;

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

  /*
   * Order position in containers with operator == and <
   */
  inline bool operator==(const SimTaDynCell& a) { return a.id_ == this->id_; }
  inline bool operator<(const SimTaDynCell& a) { return a.id_ < this->id_; }
  inline Key privateId() { return id_; }

  virtual void addExplicitNeighbor(SimTaDynCell *c) { explicit_neighbors.insert(c); }
  virtual void removeExplicitNeighbor(SimTaDynCell *c) { explicit_neighbors.insert(c); }
  //virtual void addImplicitNeighbor(SimTaDynCell *c) { implicit_neighbors.insert(c); }
  virtual void moveToPosition(const Position3D& p) = 0;
  virtual Position3D& getPosition() = 0;
  // FIXME: ajouter position;

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

#endif /* CELLS_HPP_ */
