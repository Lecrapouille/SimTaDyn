//=====================================================================
// SimForth: A Forth for SimTaDyn project.
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

#include "Forth.hpp"
#include "Config.hpp"
#include <unistd.h>
#include <memory>

static void usage(const char* fun)
{
  std::cout << "Usage:   " << fun << " [-option] [argument]" << std::endl;
  std::cout << "option:  " << "-h              Show this usage" << std::endl;
  std::cout << "         " << "-u              Show this usage" << std::endl;
  std::cout << "         " << "-l dico         Load a SimForth dictionary file and smash the current dictionary" << std::endl;
  std::cout << "         " << "-a dico         load a SimForth dictionary file and append to the current dictionary" << std::endl;
  std::cout << "         " << "-d dico         Dump the current dictionary into a binary file" << std::endl;
  std::cout << "         " << "-f file         Interprete a SimForth script file (ascii)" << std::endl;
  std::cout << "         " << "-e string       Interprete a SimForth script string (ascii)" << std::endl;
  std::cout << "         " << "-p              Pretty print the dictionary with or without color (depending on option -x)" << std::endl;
  std::cout << "         " << "-i              Interactive mode (not yet implemented)" << std::endl;
  std::cout << "         " << "-x              Do not use color when displaying dictionary" << std::endl;
}

int main(int argc,char *argv[])
{
  // Call it before Logger constructor
  if (!File::mkdir(config::tmp_path))
    {
      std::cerr << "Failed creating the temporary directory '"
                << config::tmp_path << "'" << std::endl;
    }

  // No option
  if (1 == argc)
  {
    usage(argv[0]);
    return 1;
  }

  // Enable/disable colorful text displayed on terminal
  termcolor::enable();
  for (int i = 1; i < argc; ++i)
    {
      if ((argv[i][0] == '-') && (argv[i][1] == 'x'))
        {
          termcolor::disable();;
        }
    }

  ForthDictionary dico;
  Forth forth(dico);
  std::pair<bool, std::string> res;
  bool r;
  int opt;

  // Boot the default core. Even if the user will load
  // a dictionary instead
  forth.boot();

  while ((opt = getopt(argc, argv, "hua:l:d:f:e:pix")) != -1)
  {
    switch (opt)
      {
        // Help infomation
      case 'h':
      case 'u':
        usage(argv[0]);
        return 1;
        break;

        // Load a dictionary
      case 'a':
      case 'l':
        r = forth.dictionary().load(optarg, ('l' == opt));
        if (r)
          {
            std::cout << "Dictionary successfully loaded from file '"
                      << optarg << "'" << std::endl;
          }
        break;

        // Dump a dictionary
      case 'd':
        r = forth.dictionary().dump(optarg);
        if (r)
          {
            std::cout << "Dictionary successfully dumped in file '"
                      << optarg << "'" << std::endl;
          }
        break;

        // Execute a script file
      case 'f':
        res = forth.interpreteFile(optarg);
        forth.ok(res);
        break;

        // Execute a script given as option
      case 'e':
        res = forth.interpreteString(optarg);
        forth.ok(res);
        break;

        // Pretty print the dictionary
      case 'p':
        forth.displayDictionary();
        break;

        // Interactive mode
      case 'i':
        std::cerr << "Not yet implemented" << std::endl;
        return 1;
        break;

      default:
        break;
      }
  }

  return 0;
}
