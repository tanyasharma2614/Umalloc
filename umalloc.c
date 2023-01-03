/* List all group members' names:
    Sanchay Kanade (sk2656)
    Tanya Sharma (tds104)
    iLab machine tested on: ilab4 
*/

#include "umalloc.h"

/***********************************************
 * Implementation of malloc fucntion.
 * This fucntion accepts size in bytes to be allocated 
 * and allocates the requested space in the memory 
 * if the space is enough.
 * 
 * *********************************************/

void* umalloc( size_t bytes, char* file, int line){

    if(bytes == 0){
        //Error detection case when amount requested makes not sense.
        printf("Amount requested makes no sense in line %d,%s\n", line, file);
        return NULL;
    }

    //When we need only 1 byte of meta data
    unsigned int sizeRequested = bytes + 1;
    //When we need 2 bytes of meta data
    if(sizeRequested>=32 && sizeRequested<=8191)
        sizeRequested++;
    //If we need 4 bytes of meta data
    if(sizeRequested>=8192)
        sizeRequested = bytes + 4;
    
    int flag = 0;
    //If memory is not initialized
    if(init == 'd'){
        //initialize the memory
        //printf("coming in if block\n\n");
        unsigned char* currentBlock = (unsigned char*)&mem[0];
        //If we have enough memory to add
        if (sizeRequested <= CAPACITY) { 
                //printf("%d\n", sizeRequested);
                createBlock(currentBlock, 1, sizeRequested); 
                // Setting the block after this memory to 0 so that we dont allocate anything after it.
                createBlock(currentBlock + sizeRequested, 0, 0); 
                init = 'i';
                //printf("Printing init in umalloc %d\n", init);
                //returing allocated block
                return currentBlock + metadataSize(currentBlock); 
                
        } 
        else { 
                //Error detection case when there is free memory  but not enough for the allocation 
                //in the mem array.
                printf("Not enough memory.\n There is no free memory in line %d, %s\n", line, file);
                return NULL;
        }
    }
    // If memory is already initialized
    else{
        //printf("Coming in else block/n");
        unsigned int i = 0;

        while (i < CAPACITY) {
            // Current memory block to be analyzed
            unsigned char* currentBlock = (unsigned char*)&mem[i];
            // if not allocated
            if (isAllocated(currentBlock) == 0) {
                // If we are at a point that is not allocated, we could be in two cases
                //  1. We are at the end, meaning after this point, there is NO allocated memory left
                //  2. We are in between chunks of used memory
                // Therefore, we verify based on the size.
                unsigned int sizeOfCurrentBlock = payloadSize(currentBlock);
                //printf("%d\n", sizeOfCurrentBlock);
                //if end is reached
                if (sizeOfCurrentBlock == 0) { 
                    if (i + sizeRequested <= CAPACITY) { 
                        //if we have enough memory to add
                        createBlock(currentBlock, 1, sizeRequested); 
                        createBlock(currentBlock + sizeRequested, 0, 0); 
                        return currentBlock + metadataSize(currentBlock); 
                    } 
                    else { 
                        //Error detection case when there is free memory  but not enough for the allocation 
                        //in the mem array.
                        printf("Not enough memory.\n There is no free memory %d,%s\n", line, file);
                        return NULL;
                        break;
                    }
                }
                //We have a block free between alocated blocks. So we will comapre  
                //wheather we can fit requested block in this code or not.    
                else { 
                    if (sizeRequested <= sizeOfCurrentBlock && sizeRequested > 0) {
                        flag = 0;
                        createBlock(currentBlock, 1, sizeRequested); 
                    // If free space is left in the memory block, then we set it as not allocated
                    if (sizeOfCurrentBlock - sizeRequested > 0) { 
                        createBlock(currentBlock + sizeRequested, 0, sizeOfCurrentBlock - sizeRequested);
                    }
                        return currentBlock + metadataSize(currentBlock); 
                    }
                    
                }
                if(flag == 1){
                    //Error detection case when 
                        printf("Enough free memory but no block is large enough to fit requested bytes\n %d,%s", line, file);
                }
            }
        
        i += payloadSize(currentBlock);
 
        }
        init = 'i';
    
    }
    //Error detection case when there is no free memory in the mem array.
    printf("There is no free memory. Overflow in line %d,%s\n", line, file);
    return NULL;
}


/**************************************************
 * Implementation of free function.
 * It accpets a pointers as an argument and 
 * frees the requested memory location.
 * It also includes merging logic.
 * 
 * ************************************************/

void ufree( void* ptr, char* file, int line){
    
    //printf("Now Freeing up requested block\n");
    //Using this flag for error detection
    int flag = 0;

    //Pointer to previous node, used to check if the previous node is allocated or not
    char* prevFree = NULL;

    //Iterator for each memory block in mem array
    unsigned int i = 0;

    //Error handling when ptr is not returned by malloc or the pointer is not of a valid address.
    if(ptr < &mem[0] || ptr > &mem[CAPACITY]){
        printf("Freeing address that are not allocated by malloc in line %d, %s", line, file);
        return;
    }

    //if ptr is null
    if(ptr == NULL){
        return; 
    }
    
    while(i<CAPACITY){
        unsigned char* currBlock = (unsigned char*)&mem[i];
        //if the memory block is not allocated
        if(!isAllocated(currBlock)){
            //store the size of the current block in the variable;
            unsigned int sizeOfCurrentBlock = payloadSize(currBlock);
            
            if(sizeOfCurrentBlock == 0){
                if(prevFree != NULL){
                    createBlock(prevFree,0,0);
                }
                break;
            }
            else{
                if(currBlock + metadataSize(currBlock) == ptr && flag ==0){
                    printf("Redundant Freeing Error: \n Cannot deallocate memory that was already deallocated in line %d, %s\n", line, file);
                    //Setting the flag 
                    //printf("Coming in memory NOTallocated if and setting flag\n");
                    flag = 1; 
                    break;
                }
                if(prevFree !=NULL){
                    //merging two chunks together
                    int sizeOfNewBlock = payloadSize(currBlock) + payloadSize(prevFree);
                    createBlock(prevFree,0,sizeOfNewBlock);
                    //Setting the flag so that we dont run into error
                    //flag = 1;
                }

                i+=payloadSize(currBlock);

            }
            if(prevFree == NULL){
                prevFree = currBlock;
            }
        }
        //memory is allocated
        else{
            //If this is the pointer we need to free
            if(currBlock + metadataSize(currBlock)== ptr){
                if((payloadSize(currBlock)-metadataSize(currBlock))!=0){
                    createBlock(currBlock, 0, payloadSize(currBlock));
                }
                //printf("Coming in memory allocated if and setting flag\n");
                flag = 1;
            }
            //If not then set prevFree as NULL
            else{
                prevFree = NULL;
                i += payloadSize(currBlock);
                //flag = 1;
            }
        }
      //Error handling when trying to free invalid pointer
    //printf("Flag is %d", flag);
    if(flag == 0){
            printf("Deallocation Error: \n Attempted to deallocate an invalid pointer in line %d, %s\n\n", line, file);
    }  
    }
 
  
}


/***************************************************
 * Helper Fucntion prettyPrint 
 * This fucntion prints all the blocks and their
 * respective information in our memory.
 *  
 * *************************************************/
void prettyPrint(){

    int i =0 ;
    while(i<CAPACITY){
        unsigned char *currentBlock = (unsigned char*)&mem[i];
        printf("INDEX: %d, ADDRESS: %d\n    is Allocated: %d, METADATA: %d, TOTAL_SIZE: %d\n", i, currentBlock, isAllocated(currentBlock), metadataSize(currentBlock), payloadSize(currentBlock));
        if(payloadSize(currentBlock) > 0 || isAllocated(currentBlock)){

            i = i + payloadSize(currentBlock);
        }
        else{
            i = CAPACITY;
        }
        printf("\n\n");
    }
}

/*****************************************************
 * Helper function isAllocated
 * This fcuntions tells if the current block of 
 * memory is allocated or not by checking the LSB of
 * meta data.
 * ***************************************************/

unsigned int isAllocated(unsigned char* currentBlock){
    
    return *currentBlock & 1;

}

/****************************************************
 * Helper function metadataSize
 * Returns the width of meta to be used for a block.
 * 
 * **************************************************/
unsigned int metadataSize(unsigned char* currentBlock) {
    //printf("Return from bW function %d\n", ((*currentBlock >> 1) & 1));
    if(!((*currentBlock >> 1) & 1) && !((*currentBlock >> 2) & 1)){
        return 1;
    }
    else if(((*currentBlock >> 1) & 1) && !((*currentBlock >> 2) & 1)){
        return 2;
    }
    else{
        //printf("Comming to this else in metadataSize\n\n");
        return 4;
    }
    //return (*currentBlock >> 1) & 1;
}

/****************************************************
 * Helper function payloadSsize
 * This function returns the payload size of the
 * block requested by removing last three LSB's.
 * 
 * **************************************************/

unsigned int payloadSize(unsigned char* currentBlock) {
    unsigned int bytesize = metadataSize(currentBlock);
    if(bytesize == 1){
        return *currentBlock>>3 ;
    }
    else if(bytesize == 2){
        return *((short*)currentBlock)>>3;
    }
    else{
        return (*(unsigned int*)currentBlock) >> 3;
    }
}

/******************************************************
 * Helper function createBlock
 * This fucntion sets thesize of meta data and creates 
 * a memory block for the requested bytes.
 *  * ***************************************************/

void createBlock(unsigned char* currentBlock, unsigned short allocated, unsigned int size) {
    if(size< 32){
        //printf("32");
        // printf("%d\n", (size << 3) + allocated);
        *currentBlock = (size << 3) + allocated;
    }
    else if(size>=32 && size<=8191){
        //printf("8191");
        *((short*)currentBlock ) = (size << 3) + allocated + 2;
    }
    else{
        //printf("more than\n");
        // printf("%d\n",(size << 3) + allocated + 6);
        *((int*)currentBlock) = (size << 3) + allocated + 6;
    }
    
} 