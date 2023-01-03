// File:	umalloc.h

// List all group members' names:
// Sanchay Kanade (sk2656)
// Tanya Sharma (tds104)
// iLab machine tested on: ilab4

//We will follow "per allocation" stratergy for our meta data, that is each memory block we allocate 
//will contain its own meta data.

//Capacity of our array mem is 10MB
#define CAPACITY 10*1024*1024


//#define malloc(x) umalloc(x)
//#define free(x) ufree(x)



#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define malloc(x) umalloc(x,__FILE__,__LINE__)
#define free(x) ufree(x,__FILE__,__LINE__)

//This is our mem array from which we will allocate space
static char mem[CAPACITY];

//This stores the value which will tell us if the memory has been in use before or we are using it for the first time
char init = 'd';

//Our memory allocator
//void* umalloc( size_t);
void* umalloc( size_t, char*, int);

//Fucntion to freee up our memory
//void ufree( void*);
void ufree( void*, char*, int );

void prettyPrint();

unsigned int isAllocated(unsigned char*);

unsigned int metadataSize(unsigned char*);

unsigned int payloadSize(unsigned char*);

void createBlock(unsigned char* , unsigned short, unsigned int);