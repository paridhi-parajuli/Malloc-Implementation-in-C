#include<stddef.h>
// this structure denotes a chunk of memory that has metadata and allocation buffer
struct Chunk {
    size_t size; // gives the size requested by the user
    struct Chunk *next; // points to next chunk of memory
    struct Chunk *prev; // points to previous chunk of memory
    int isFree; // denotes either the chunk is allocated or not; 1 if free else 0

};
void initialize();
void *mymalloc(size_t size);
void myfree(struct Chunk *c);
void printList();	