#ifndef SIMTADYNHANDLER_HPP_
#  define SIMTADYNHANDLER_HPP_

#  include <typeinfo>
#  include <map>

using namespace std;

// http://www.gamedev.net/page/resources/_/technical/game-programming/effective-event-handling-in-c-r2459

// **************************************************************
//
// **************************************************************
class Event
{
public:
  const string getName() const { return name_; }
protected:
  virtual ~Event() { name_ = "Event"; };
string name_;
};

// **************************************************************
//
// **************************************************************
class TypeInfo
{
public:
  explicit TypeInfo(const type_info& info) : _typeInfo(info) {};

  bool operator < (const TypeInfo& rhs) const
  {
    return _typeInfo.before(rhs._typeInfo) != 0;
  }

private:
  const type_info& _typeInfo;
};

// **************************************************************
//
// **************************************************************
enum class NotifyAction { Done, Failed, Ignored };

// **************************************************************
//
// **************************************************************
class HandlerFunctionBase
{
public:
  virtual ~HandlerFunctionBase() {};
  void exec(const Event* event) { call(event); }
  //NotifyAction exec(const Event* event) { return call(event); }

private:
  virtual void call(const Event*) = 0;
  //virtual NotifyAction call(const Event*) = 0;
};

// **************************************************************
//
// **************************************************************
template <class T, class EventT>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
  typedef void (T::*MemberFunc)(EventT*);
  MemberFunctionHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

private:
  void call(const Event* event)
  {
    (_instance->*_function)(static_cast<EventT*>(event));
  }

private:
  T* _instance;
  MemberFunc _function;
};

// **************************************************************
//
// **************************************************************
class EventHandler
{
public:
  void handleEvent(const Event* event)
  {
    Handlers::iterator it = _handlers.find(TypeInfo(typeid(*event)));
    if (it != _handlers.end())
      {
        it->second->exec(event);
      }
    else
      {
        std::cerr << "handleEvent(): Event not registered" << std::endl;
        //return NotifyAction::Ignored;
      }
  }

  ~EventHandler()
  {
    Handlers::iterator it = _handlers.begin();
    while (it != _handlers.end())
      {
        delete it->second;
        ++it;
      }
    _handlers.clear();
  }

  template <class T, class EventT>
  void registerEventFunc(T* obj, void (T::*memFn)(EventT*))
  {
    _handlers[TypeInfo(typeid(EventT))] = new MemberFunctionHandler<T, EventT>(obj, memFn);
  }

private:
  typedef std::map<TypeInfo, HandlerFunctionBase*> Handlers;
  Handlers _handlers;
};

#endif /* SIMTADYNHANDLER_HPP_ */
