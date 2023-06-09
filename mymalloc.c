
#include <sys/mman.h>
#include <stdio.h>
#include<stddef.h>
#include "mymalloc.h"
static struct Chunk *freelist; // Making this a global variable so that its values can get updated in the following functions.

// Initialization of the heap of 4096B. First its just has one chunk.
void initialize(){
    printf("Initializing a heap.......\n");
    freelist = mmap(NULL , 4096, PROT_READ|PROT_WRITE, // this gives a pointer of type Chunk.
                    MAP_ANON|MAP_PRIVATE, 0, 0); // This is a system call.
    freelist->prev= NULL; 
    freelist->next=NULL;
    freelist->size = 4096 - sizeof(struct Chunk); // actual size from which allocation can be done
    freelist->isFree= 1; // denotes that the chunk is free
}

void printList(){
    printf("List is : \n");
    int i=1; // this variable is to track the chunk number
    struct Chunk *temp=freelist; // freelist always points to the starting of the big chunk
    while (temp!=NULL){ // This condition is for traversing the list
        printf("Chunk %d: %p\n", i,temp);
        printf("\t size : %zu \n", temp->size);
        printf("\t next : %p\n", temp->next);
        printf("\t prev : %p \n", temp->prev);
        printf("\t is free? %d \n", temp->isFree);
        temp=temp->prev;//here, we are traversing in opposite direction using prev pointer
        ++i;
    }
}

void *mymalloc(size_t requested_size){
    struct Chunk *new = freelist; // new chunk to be added
    struct Chunk *temp= freelist;
    struct Chunk *previous = freelist->prev;
    size_t initial_size= freelist->size;
    // Find your first fit here
    while (temp!=NULL){
        if  (temp->size>=(requested_size +sizeof(struct Chunk))){
            new=temp; // new is the pointer to the first fit chunk
            break;
        }
        temp=temp->prev; // here, we are traversing in opposite direction using prev pointer
    }
    //do splitting
    if (new->size >= (requested_size +sizeof(struct Chunk))){ // comparing size to the sum of requested size plus the metadata size
        int j= requested_size/sizeof(struct Chunk) +1; // +1 is for addition of a chunk size to freelist pointer
        freelist= new + j; // shifting the freelist to left to make room for new chunk
        freelist->size = initial_size - requested_size - sizeof(struct Chunk); // recalculating the size of freelist after allocation
        // Linking the chunks
        freelist->prev= new;
        freelist->next = NULL;
        freelist->isFree= 1;
        new->size = requested_size;
        new->next= freelist;
        new->prev= previous;
        new->isFree=0;
        struct Chunk* result = new + 1; // result points to the actual location from where allocation data can be kept. new pointed to the metadata of the chunk
        return result;
    }
    else {
        printf("No fit is found"); // returns null if no fit is found 
        return NULL;
    }
}


void myfree(struct Chunk *to_free){
    struct Chunk *freed_chunk=to_free -1; //getting the actual pointer of the chunk to be freed
    freed_chunk->isFree =1; // setting its isFree flag to 1
    struct Chunk *x,*y;
    //merging free chunks
    //Check if there is a free chunk in the left side of the free chunk
    if ( freed_chunk->next!=NULL &&(freed_chunk->next)->isFree==1){
        y=freed_chunk->next;
        freed_chunk->next=y->next;
        if ((y->next) !=NULL){ // if there are one or more chunks next to the left free chunk
            (y->next)->prev=freed_chunk;}
        else{
            freelist=freed_chunk; // left free chunk is the freelist itself
        }
        freed_chunk->size =y->size +sizeof(y)+freed_chunk->size; // calculating the size of merged chunk
        freed_chunk->isFree=1;
     }
    //Check if there is a free chunk in the right side of the free chunk
    if (freed_chunk->prev!=NULL && (freed_chunk->prev)->isFree==1){
        x=freed_chunk;
        freed_chunk=x->prev;
        freed_chunk->next=x->next;
        freed_chunk->size=x->size + freed_chunk->size + sizeof(x);
        freed_chunk->isFree=1; 
        if (x->next!=NULL){ //if there are one or more chunks next to the free chunk
            (x->next)->prev =freed_chunk;}
        else{
            freelist=freed_chunk; // freed chunk is the freelist litself
        }
    }
}


