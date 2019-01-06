//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "ClassicSpreadSheet.hpp"
#include "PathManager.hpp"
#include <cstdlib>

static void usage(const char* prog_name)
{
  std::cout << prog_name << ": <sheet id>" << std::endl;
  std::cout << "  with <sheet id> an integer >= 1 and <= 5" << std::endl;
  exit(1);
}

int main(int argc, char* argv[])
{
  // Incorrect number of param
  if (argc != 2)
    usage(argv[0]);

  // Incorrect sheet indentifier
  std::string arg(argv[1]);
  int test_id = atoi(argv[1]);
  if ((test_id < 1) || (test_id > 5))
    usage(argv[0]);

  // Call it before Logger constructor
  if (!File::mkdir(config::tmp_path))
    {
      std::cerr << "Failed creating the temporary directory '"
                << config::tmp_path << "'" << std::endl;
    }

  PathManager::instance();

  SimForth& forth = SimForth::instance();
  ClassicSpreadSheet sheet("Sheet" + arg);

  forth.boot();
  std::string sheet_name("examples/input" + arg + ".txt");
  std::cout << "Solving the sheet: " << sheet_name << std::endl;
  if (!sheet.readInput(sheet_name))
    {
      std::cerr << "Abort" << std::endl;
    }

  sheet.parse(forth);
  std::pair<bool, std::string> res = sheet.evaluate(forth);
  forth.ok(res);
  if (res.first)
    {
      sheet.displayResult();
    }

  return 0;
}
