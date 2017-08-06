//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
// Copyright 2004 Quentin Quadrat <lecrapouille@gmail.com>,
//                Minh-Long Nguyen <>,
//                Benoit Marcot <>
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

#include "memoire.h"

inline void * xmalloc(size_t size)
{
     void *p;

     p = malloc(size);
     if (p == 0)
     {
          fprintf(stderr, "*** Memoire virtuelle epuisee. ***\n");
          exit(1);
     }
     return p;
}

inline void * xcalloc(size_t nmemb, size_t size)
{
     void *p;

     p = calloc(nmemb, size);
     if (p == 0)
     {
          fprintf(stderr, "*** Memoire virtuelle epuisee. ***\n");
          exit(1);
     }
     return p;
}

inline void * xrealloc(void *ptr, size_t size)
{
     void *p;

     p = realloc(ptr, size);
     if (p == 0)
     {
          fprintf(stderr, "*** Memoire virtuelle epuisee. ***\n");
          exit(1);
     }
     return p;
}

inline t_trash nouveau_trash()
{
     t_trash trash;

     trash.i = 0;
     return trash;
}

inline void ajout_trash(t_trash * trash, void* ad)
{
     // ne teste pas si sort du tableau
     (*trash).tab[(*trash).i] = ad;
     ++(*trash).i;
}

inline void cleanall_trash(t_trash * trash)
{
     unsigned int j;

     for (j = 0; j < (*trash).i; ++j)
          free((*trash).tab[j]);
}

inline void clean_trash(t_trash * trash, unsigned int j)
{
     free((*trash).tab[j]);
}
