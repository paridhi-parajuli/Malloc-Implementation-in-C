#include<stdio.h>
#include "mymalloc.h"
int main()
{
	printf("##########Process Started##########\n");
    initialize();
    char *ptr1,*ptr2,*ptr3,*ptr4,*ptr5;
    ptr1=(char*)mymalloc(100);
    ptr2=(char*)mymalloc(200);
    ptr3=(char*)mymalloc(300);
    ptr4=(char*)mymalloc(400);
    ptr5=(char*)mymalloc(50);
    printf("\nAll chunks allocated");
    printList();  
    printf("\n Freeing 200B");
    myfree(ptr2);
    printList();
    printf("\n Freeing 400B");
    myfree(ptr4);
    printList();
    printf("\n Freeing 300B");
    myfree(ptr3);
    printList();
    printf("\n Freeing 100B");
    myfree(ptr1);
    printList();
    printf("\n Freeing 50B");
    myfree(ptr5);
    printList();
    printf("#####Process Ended#####");
	return(0);
}