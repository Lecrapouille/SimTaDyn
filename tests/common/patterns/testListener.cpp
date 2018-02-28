#include "Observer.tpp"
#include <iostream>

// *************************************************************************************************
enum class SimTaDynMapEvent : int32_t { LoadSuccess = 0, Changed = 1 };

// *************************************************************************************************
class SimTaDynMap: public Observable<SimTaDynMap, SimTaDynMapEvent>
{
public:

  SimTaDynMap(const char* name)
    : Observable(name)
  {
  }

  ~SimTaDynMap()
  {
    //std::cout << "    Je suis '" << info() << " est je suis destruit" << std::endl;
  }

  std::string drawMap() const
  {
    return "drawMap";
  }
};

// *************************************************************************************************
class ISimTaDynMapListener
{
public:

  virtual std::string onFailure(std::string const& msg) const = 0;
  virtual std::string onChanged(SimTaDynMap const& subject) const = 0;
};

// *************************************************************************************************
class DrawingArea
  : public ISimTaDynMapListener,
    public Observer<SimTaDynMap, SimTaDynMapEvent>
{
public:

  DrawingArea(const char* name)
    : Observer(name)
  {
  }

  ~DrawingArea()
  {
  }

  virtual std::string onFailure(std::string const& msg) const override { return ""; }
  virtual std::string onChanged(SimTaDynMap const& subject) const override { return std::string("DarwingArea on changed do ") + subject.info(); }

  void update(SimTaDynMap const& subject, SimTaDynMapEvent const& event) const override
  {
    std::cout << "Je suis '" << info()
              << " et " << subject.info()
              << " m'a notifie avec l'event '"
              << static_cast<int>(event)
              << "' et son truc special '"
              << subject.drawMap()
              << "' ";

    switch (event)
      {
      case SimTaDynMapEvent::LoadSuccess:
        std::cout << "Nothing to do" << std::endl;
        break;
      case SimTaDynMapEvent::Changed:
        std::cout << onChanged(subject) << std::endl;
        break;
      }
  }

private:

  std::string m_name;
};

// *************************************************************************************************
class MapEditor
  : public ISimTaDynMapListener,
    public Observer<SimTaDynMap, SimTaDynMapEvent>
{
public:

  MapEditor(const char* name)
    : Observer(name)
  {
  }

  ~MapEditor()
  {
    //std::cout << "    Je suis '" << info() << " est je suis destruit" << std::endl;
  }

  virtual std::string onFailure(std::string const& msg) const override{ return std::string("MapEditor on Failure do ") + msg; }
  virtual std::string onChanged(SimTaDynMap const& subject) const override { return ""; }

  void update(SimTaDynMap const& subject, SimTaDynMapEvent const& event) const override
  {
    std::cout << "Je suis '" << info()
              << " et " << subject.info()
              << " m'a notifie avec l'event '"
              << static_cast<int>(event)
              << "' et son truc special '"
              << subject.drawMap()
              << "' ";

    switch (event)
      {
      case SimTaDynMapEvent::LoadSuccess:
        std::cout << onFailure("dssdsd") << std::endl;
        break;
      case SimTaDynMapEvent::Changed:
        std::cout << "Nothing to do" << std::endl;
        break;
      }
  }

private:

  std::string m_name;
};

// *************************************************************************************************
int main()
{
  SimTaDynMap m("Paris");
  MapEditor ed("MapEditor");
  DrawingArea da("DrawingArea");

  m.attach(ed);
  m.attach(da);
  m.notify(SimTaDynMapEvent::Changed);
  m.notify(SimTaDynMapEvent::LoadSuccess);

  return 0;
}
