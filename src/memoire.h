#ifndef MEMOIRE_H
#define MEMOIRE_H

#include "header_forth.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>

void * xmalloc(size_t size);
void * xcalloc(size_t nmemb, size_t size);
void * xrealloc(void *ptr, size_t size);
t_trash nouveau_trash(void);
void ajout_trash(t_trash * trash, void* ad);
void cleanall_trash(t_trash * trash);
void clean_trash(t_trash * trash, unsigned int j);

#endif /* MEMOIRE_H */
