#include "Singleton.hpp"

template <class T> T* Singleton<T>::s_instance = nullptr;
