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
