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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "ClassicSpreadSheet.hpp"
#include "PathManager.hpp"

int main()
{
  // Call it before Logger constructor
  if (!File::mkdir(config::tmp_path))
    {
      std::cerr << "Failed creating the temporary directory '"
                << config::tmp_path << "'" << std::endl;
    }

  PathManager::instance();

  SimForth& forth = SimForth::instance();
  ClassicSpreadSheet sheet("Sheet1");

  forth.boot();
  if (!sheet.readInput("examples/input2.txt"))
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
