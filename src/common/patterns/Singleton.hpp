#ifndef SINGLETON_HPP_
#  define SINGLETON_HPP_

template <class T> class Singleton
{
public:

  static T& instance()
  {
    if (nullptr == s_instance)
      {
        s_instance = new T;
      }
    return *s_instance;
  }

  static void destroy()
  {
    if (nullptr != s_instance)
      {
        delete s_instance;
        s_instance = nullptr;
      }
  }

protected:

  Singleton() { }
  ~Singleton() { }

private:

  static T* s_instance;

  Singleton(Singleton&);
  void operator=(Singleton);
};

template <class T> T* Singleton<T>::s_instance = nullptr;

#endif /* SINGLETON_HPP_ */
