#include "SafeObserver.tpp"
#include <iostream>

class MyEvent
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

class MyObservable: public Observable<MyObservable, MyEvent>
{
public:

  MyObservable(const char* name)
    : Observable(name)
  {
  }

  ~MyObservable()
  {
    std::cout << "    Je suis '" << info() << " est je suis destruit" << std::endl;
  }

  std::string specialStuff() const
  {
    return "Pouet";
  }
};

class MyObserver : public Observer<MyObservable, MyEvent>
{
public:

  ~MyObserver()
  {
    std::cout << "    Je suis '" << info() << " est je suis destruit" << std::endl;
  }

  MyObserver(const char* name)
    : Observer(name)
  {
  }

  void update(MyObservable const& subject, MyEvent const& arg) const override
  {
    std::cout << "    Je suis '" << info()
              << "' et je m'update car "
              << subject.info()
              << " m'a notifie avec l'event '"
              << arg.message()
              << "' et son truc special '"
              << subject.specialStuff()
              << "'" << std::endl;
  }

private:

  std::string m_name;
};

int main()
{
#if 0
  MyObservable o("MyObservable");
  MyObserver n1("N1");
  MyObserver n2("N2");
  MyObserver n3("N3");
  MyEvent e1("coucou");

  o.attach(n3);
  o.attach(n2);
  o.attach(n1);

  o.detach(n3);
  o.notify(e1);
#endif

  MyObservable o("MyObservable");
  MyObserver n1("N1");
  MyObserver n2("N2");
  MyEvent e1("coucou");

  {
    MyObserver n3("N3");
    o.attach(n3);
    o.attach(n2);
    o.attach(n1);
  }
  //o.detach(n3);
  o.notify(e1);

  return 0;
}
