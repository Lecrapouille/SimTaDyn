#ifndef MAPEDITIONTOOLS_HPP
#  define MAPEDITIONTOOLS_HPP

// ***********************************************************************************************
//! \brief Command Pattern for map edition (insertion, move ...)
//! http://gameprogrammingpatterns.com/command.html
// ***********************************************************************************************
class MapEditionTools
{
public:

  MapEditionTools() {}
  virtual ~MapEditionTools() {}
  virtual void exec1(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void exec2(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void exec3(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void undo() {};
};

class AddCellTool: public MapEditionTools
{
public:

  virtual void exec1(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    std::cout << "Ajoute1 une cellule a la position " << static_cast<int>(x) << " " << static_cast<int>(y) << std::endl;
    MapEditor::instance().map().m_sheet.addNode("");
  }

  virtual void exec2(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    std::cout << "Ajoute2 une cellule a la position " << static_cast<int>(x) << " " << static_cast<int>(y) << std::endl;
  }

  virtual void exec3(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    std::cout << "Ajoute3 une cellule a la position " << static_cast<int>(x) << " " << static_cast<int>(y) << std::endl;
  }
};

class RemoveCellTool: public MapEditionTools
{
public:

  virtual void exec1(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void exec2(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void exec3(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

class SelectCellTool: public MapEditionTools
{
public:

  virtual void exec1(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void exec2(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void exec3(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

class MoveCellTool: public MapEditionTools
{
public:

  virtual void exec1(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void exec2(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void exec3(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

#endif
