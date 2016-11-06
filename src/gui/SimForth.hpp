#ifndef SIMFORTH_HPP_
#  define SIMFORTH_HPP_

#  include "ForthInner.hpp"
#  include "SimTaDynWindow.hpp"

// *************************************************************************************************
// Chan
// *************************************************************************************************
class SimForth: public Forth
{
public:
  SimForth();
  void addForthMenu(const Gtk::BuiltinStockID icon,
                    const std::string &word);
  void addForthButon(const Gtk::BuiltinStockID icon,
                     const std::string &word); // Glib::ustring
  //                , const Cell16 Forthtoken);
protected:
  uint32_t i;
};

#endif /* SIMFORTH_HPP_ */
