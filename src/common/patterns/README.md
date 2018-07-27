Files:

* Singleton.[ch]pp: Singleton class where other class can inherit from it.

* ClassCounter.tpp: template class for counting class instances. Class
  inherites from this class.

* Observer.hpp: FIXME: shall be reworked (add comments and add web
  links) ! Observer and Observable patterns with N--M relations
  instead of 1--M links. Usually an observer notifies several
  observables. Here an Observable knows all Observers. Used for cell
  notifications in the spreadsheet (cell content changed) and
  preventing segfault when detaching observers.
