#include "ASpreadSheetCell.hpp"

void ASpreadSheetCell::update()
{
  SimForth::instance().interpreteCell(*this);
}

//FIXME
void ASpreadSheetCell::parse()
{
  m_references.clear();
  SimForth::instance().parseCell(*this);
  m_unresolvedRefs = m_references.size();
}

/*
void ASpreadSheetCell::parse(SimForth &forth)
{
  //std::cout << "ASpreadSheetCell::parse()" << std::endl;
  //if (m_evaluated)
  //  return ;

  m_references.clear();
  forth.parseCell(*this);
  m_unresolvedRefs = m_references.size();
}

void ASpreadSheetCell::parse()
{
  parse(SimForth::instance());
  }*/
