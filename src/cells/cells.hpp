#ifndef CELLS_HPP_
#  define CELLS_HPP_

#  include "types.hpp"

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
  string code_forth;

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

// *************************************************************************************************
// Define a Node (aka Vertex)
// *************************************************************************************************
class SimTaDynNode: public SimTaDynCell, private ClassCounter<SimTaDynNode>
{
public:
  //protected:
  /*
   * Position in space
   */
  Position3D position_;
  // MovedEvent --> register

public:

  /*
   * Constructor: give a name (else use a generic one) and a position in space.
   */
  SimTaDynNode(string pname, const Position3D& p, string pcode_forth) : position_(p)
  { name = pname; code_forth = pcode_forth; registerEventFunc(this, &SimTaDynNode::onMoved); }
  SimTaDynNode(string pname, const Position3D& p) : position_(p)
  { name = pname; registerEventFunc(this, &SimTaDynNode::onMoved); }
  SimTaDynNode(const Position3D& p) : position_(p)
  { name = "Node_" + to_string(id_); registerEventFunc(this, &SimTaDynNode::onMoved); }
  ~SimTaDynNode() { }

  //Position3D getPosition() { return position_; }

  //private
  void onMoved(const EventMoved* movement)
  {
    //cout << __PRETTY_FUNCTION__ << " ";
    cout << "    * "; whoAmI();
    cout << "      My neighbor the cell #" << movement->cell_->privateId() << " nammed \"" << movement->cell_->name
         << "\" moved to position " << movement->cell_->position_ << endl;
  }

  /*
   * Change the position and notify it to all neighbours
   */
  void moveToPosition(const Position3D& p)
  {
    //cout << __PRETTY_FUNCTION__ << endl;
    if (position_ != p)
      {
        position_ = p;
        EventMoved m(this);
        notifyNeighbors(&m);
      }
  }

  Position3D& getPosition() // FIXME: const
  {
    return position_;
  }

  /*
   * Debug
   */
  Key whoAmI()
  {
    cout << "I am the SimTaDynNode #" << id_ << " named \"" << name << "\":" << endl;
    return id_;
  }

  /*
   * Static member returning the number of SimTaDynNode instances created
   */
  static size_t howMany() { return ClassCounter<SimTaDynNode>::howMany(); }
};

#define SimTaDynVertex SimTaDynNode

#if 0
// *************************************************************************************************
// Define an Arc (aka Edge)
// ************************************************************************************************
class SimTaDynArc: public SimTaDynCell, private ClassCounter<SimTaDynArc>
{
  // FIXME: risque de bug:
  // SimTaDynCell* N1 = new SimTaDynNode();
  // N2.addExplicitNeighbor(N1) --> ne sera pas connu du graphe
private:
  SimTaDynCel* node_first_;
  SimTaDynCel* node_end_;
  bool oriented_;

public:
  /*
   * Transition cost (ponderation)
   */
  int cost_;

  // Quand on relie a la souris deux noeuds
  // FIXME utiliser des &
  SimTaDynArc(SimTaDynCel* node_first, SimTaDynCel* node_end, string name, bool oriented = false, int cost = 0)
  {
    node_first_ = node_first;
    node_end_ = node_end;
    private_constructor(oriented, name, cost);
  }

  // Pas possible: uniquement depuis la classe Grpahe
  //SimTaDynArc(Key key_first, Key key_end, string pname, bool oriented = false, int cost = 0)
  //{
  //  node_first_ = Graph.node_search_(node_first);
  //  node_end_ = Graph.node_search_(node_end);
  //  private_constructor(oriented, name, cost);
  //}
  // Quand on est en mode edition creer un arc
  SimTaDynArc(Position3D pf, Position3D pe, string pname, bool oriented = false, int cost = 0)
  {
    node_first_ = new SimTaDynNode(pf);
    node_end_ = new SimTaDynNode(pe);
    private_constructor(oriented, name, cost);
  }
  SimTaDynArc(int Cost_ = 0)
  {
    private_constructor(oriented, name, cost);
    Name = "Node_" + to_string(id);
    Cost = Cost_;
  }
  ~SimTaDynNode() { }

  void setOriented(bool new_bool)
  {
    if (oriented_ != new_bool)
      {
        if (new_bool)
          {
            // Ajoute une relation si elle existe
            node_end->addExplicitNeighbor(node_first);
          }
        else
          {
            // Retire une relation
            node_end->removeExplicitNeighbor(node_first);
          }
        oriented_ = new_bool;
      }
  }

  Key WhoAmI()
  {
    cout << "I am the SimTaDynArc #" << id << " Named " << Name << endl;
    return id;
  }

  static size_t howMany() { return ClassCounter<SimTaDynArc>::howMany(); }

private:
  void private_constructor()
  {
    node_first->addExplicitNeighbor(node_end);
    if (oriented_)
      {
        node_end->addExplicitNeighbor(node_first);
      }
  }
};

#define SimTaDynEdge SimTaDynArc

// *************************************************************************************************
// Define an Zone (aka Area)
// ************************************************************************************************
class SimTaDynZone: public SimTaDynCell, private ClassCounter<SimTaDynArc>
{

};

#define SimTaDynZone SimTaDynArea
#endif

// *************************************************************************************************
// Define a SimTaDyn graph
// ************************************************************************************************
// http://mooreccac.com/kcppdoc/Dijkstra.htm
// https://www.cs.cmu.edu/~pavlo/courses/fall2013/static/papers/osdi2012-graphchi.pdf
typedef map<Key, SimTaDynCell*> listNodes;
class SimTaDynGraph: private ClassCounter<SimTaDynGraph>
{
public:
  Position3D bbox_min; // Bounding box min position
  Position3D bbox_max; // Bounding box max position

  string getName() { return name_; }
  void setName(string name) { name_ = name; }
  SimTaDynGraph(string name) { name_ = name; }
  SimTaDynGraph() { name_ = ""; }
  ~SimTaDynGraph() { }

  void addNode(string name, const Position3D& position, string code_forth)
  {
    SimTaDynCell* node = new SimTaDynNode(name, position, code_forth);
    nodes_.insert(make_pair(node->privateId(), node));
  }

  void addNode(string name, const Position3D& position)
  {
    SimTaDynCell* node = new SimTaDynNode(name, position);
    nodes_.insert(make_pair(node->privateId(), node));
  }

  void addNode(const Position3D& position)
  {
    SimTaDynCell* node = new SimTaDynNode(position);
    nodes_.insert(make_pair(node->privateId(), node));
  }

  SimTaDynCell* getNode(const Key& id) // FIXME &
  {
    //std::set<SimTaDynCell*>::iterator it;

    auto it = nodes_.find(id);
    if (it == nodes_.end())
      {
        throw std::out_of_range("getNode(const Key& id): SimTaDynNode " + to_string(id) + " not found in the graph");
      }
    return it->second;
  }

protected:
  /*
   * Give a name to the element which will be displayed in the GUI.
   * Contrary to id_ several cells can have the same name. By default
   * the name is unique.
   */
  string name_;
  listNodes nodes_;
};

#endif /* CELLS_HPP_ */
