#include "BiDirObserver.hpp"
#include <iostream>

class MyEvent: public IEvent
{
public:

  MyEvent(std::string const& message)
    : m_message(message)
  {
  }

  inline const std::string& message() const
  {
    return m_message;
  }

private:

  std::string m_message;
};

class MyObservable: public BiDirObservable<MyEvent>
{
public:

  MyObservable(const char* name)
    : BiDirObservable(name)
  {
  }
};

class Subject
  : public BiDirObserver<MyEvent>,
    public BiDirObservable<MyEvent>
{
public:

  ~Subject() {}

  Subject(const char* name)
    : BiDirObserver(name),
      BiDirObservable(name)
  {
  }

  void update(BiDirObservable<MyEvent>& subject, MyEvent& arg) override
  {
    std::cout << "Je suis '" << BiDirObserver<MyEvent>::info()
              << "' et je m'update car "
              << subject.info()
              << " m'a notifie avec l'event '"
              << arg.message()
              << "'" << std::endl;
  }

private:

  std::string m_name;
};

int main()
{
  Subject n1("N1");
  Subject n2("N2");
  Subject n3("N3");
  MyEvent e1("coucou");

  n1.attachObserver(n3);
  n1.attachObserver(n2);
  n3.attachObserver(n1);

  printf("-------\n");
  n1.detachObserver(n3);
  printf("-------\n");
  n1.notify(e1);
  n3.notify(e1);

  //observable.detach(o1);
  //observable.detach(o2);

  return 0;
}
