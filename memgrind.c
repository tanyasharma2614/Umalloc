/* List all group members' names:
    Sanchay Kanade (sk2656)
    Tanya Sharma (tds104)
    iLab machine tested on: ilab4 
*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "umalloc.c"

void consistancy(){
    printf("Allocating memory\n");
    char* ptr1 = (char*)malloc(10);
    char i = 10;
    printf("Populating memory\n");
    for(i =0 ;i< 10; i++){
        ptr1[i] = i;
    }
    printf("Address is %d\n", ptr1);
    printf("Freeing  up memory\n");

    free(ptr1);

    printf("Re-allocating memory\n");

    char* ptr2 = (char*)malloc(10);

    printf("Re-Populating memory\n");
    for(i =0 ;i< 10; i++){
        ptr2[i] = i;
    }
    printf("Address is %d\n", ptr2);
    if(ptr1==ptr2){
        printf("Consistancy test successful........\n");
    }
    else{
        printf("Consistancy test failed........");
    }
    free(ptr2);
}

void maximization(){
    //allocatiing 1 byte
    printf("..........Starting Maximization..........");
    unsigned int i = 1;
    char* ptr = (char*)malloc(1);
    while(ptr != NULL && i < CAPACITY){
        free(ptr);
        ptr = (char*)malloc(i);
        i = 2*i;
    }
    printf("........Reached Maximization........ %d\n", i/2);
    if(ptr != NULL){
        i = i/4; //since i was doubled at the end of our maximization test;
        free(ptr);
        ptr = (char*)malloc(i);
        if(ptr != NULL){
            printf("Maximization test successful :)\n");

        }
        else{
            printf("Maximization test failed :(\n");
        }
        free(ptr);

    }
    printf(".......Ending Maximization......");
}

void basicCoalescence(){
    printf("..........Starting Basic Coalescence test..........\n");
    printf("Allocating half of maximal memory size\n");
    char* ptr1=(char*)malloc(5*1024*1024);
    printf("Address of first pointer:%d\n",ptr1);
    printf("Allocating a quarterof maximal memory size\n");
    char* ptr2=(char*)malloc(2.5*1024*1024);
    printf("Address of second pointer:%d\n",ptr2);
    printf("Freeing up first pointer\n");
    free(ptr1);
    printf("Freeing up second pointe\n");
    free(ptr2);
    printf("Allocating maximal memory\n");
    char* ptr3=(char*)malloc(10*1024*1024-4);
    printf("Address of third pointer:%d\n",ptr3);
    printf("..........Basic Coalescence succcessful..........");
    //prettyPrint();
    free(ptr3);
}

void saturation(){
    printf("......Starting saturation test.....\n\n");
    struct timeval tv_start, tv_end;
    unsigned int i;
    char * ptr;

    for(i=0;i<9216;i++){
        ptr = (char*)malloc(1024);
    }
    printf("*************9K DONE*************\n\n\n");

    //printf("Time take for 9K allocation test : %d", tv_end.tv_usec - tv_start.tv_usec);
    //sleep(10);
    gettimeofday(&tv_start, NULL);
    while(ptr != NULL){
        ptr = (char*)malloc(1);
    }
    gettimeofday(&tv_end, NULL);
    printf(".........Saturation test complete...........\n %d", ptr);
    printf("Time take for the test : %d", tv_end.tv_sec - tv_start.tv_sec);
    //prettyPrint();
}




/*************************************************
 * Time ovehead function
Includes saturation so no need to call saturation 
before this.
**************************************************/
void TimeOverhead(){
    printf("\n\nStarting time overhead. It will also run saturation first so it will take 20 mins to finish");
    printf("\n..........Starting saturation inside time overhead..........\n");
    unsigned int count = 0;
    struct timeval tv_start, tv_end;
    unsigned int i;
    char* ptr;
    gettimeofday(&tv_start,NULL);
    for(i=0;i<9216;i++){
        ptr=(char*)malloc(1024);
        count ++;
    }
    printf(".....Finished allocating 9k blocks of 1024.....\n");
    //prettyPrint();
    gettimeofday(&tv_end, NULL);
    printf("Time taken for 9k allocations is:%d\n:",tv_end.tv_usec-tv_start.tv_usec);
    sleep(10);
    gettimeofday(&tv_start,NULL);
    char* prev;
    while(ptr!=NULL){
        prev=ptr;
        ptr=(char*)malloc(1); 
        count ++;
        //prev=ptr;
    }
    gettimeofday(&tv_end,NULL);
    printf("Time taken for saturation with 1B blocks is:%d\n",tv_end.tv_usec-tv_start.tv_usec);
    printf("..........Saturation test complete.........\n");
    //prettyPrint();
    //printf("ptr value is = %d\n",ptr);
    //printf("prev value is = %d\n",prev);
    


    printf("..........Starting Time Overhead Test..........\n");
    printf("..........Freeing up the last 1B block of memory..........\n");
    free(prev);
    //prettyPrint();
    gettimeofday(&tv_start,NULL);
    ptr=(char*)malloc(1);
    gettimeofday(&tv_end, NULL);
    printf("\nMax time overhead:%d\n micro seconds", &tv_end.tv_usec-&tv_start.tv_usec);
    printf("pointer count is = %d\n", count);
    printf("/nTime overhead complete");
        //prettyPrint();
}

/*******************************************************************
 * IntermediateColescence function
 * Includes saturation function with timeoverhead no need to call 
 * saturation before them directly call intemediate coalescence.
 * freeing up will give some deallocation errors due to extra space in 
 * array.
********************************************************************/
void IntermediateCoalescence(){
    printf("\n\nStarting IntermediateCoalescence will take 20 min to finish as it will also run saturation\n");
    printf("\n..........Starting saturation inside IntermediateCoalescence..........\n");
    struct timeval tv_start, tv_end;
    unsigned int i=0;
    // an array to store pointers so that we can free them one by one
    //Size of this array was chosen based on the number of blocks being allocated in mem.
    int* arr[524291];
    
    gettimeofday(&tv_start,NULL);
    //Allocating 9k blocks of 1KB
    for(i=0;i<9216;i++){
        arr[i]=(char*)malloc(1024);
    }
    //printf("Value of i is ==%d/n",i);
    i--;
    printf(".....Finished allocating 9k blocks of 1024.....\n");
    //gettimeofday(&tv_end, NULL);
    //printf("Time taken for 9k allocations is: %d micro-secs\n:",tv_end.tv_usec-tv_start.tv_usec);
    
    //sleep(10);
    //gettimeofday(&tv_start,NULL);
    //char* prev;
    printf("Starting rest of the blocks....");
    while((arr[i]=(char*)malloc(1)) != NULL){
        //prev=&ptr;
        //arr[i]=(char*)malloc(1); 
        //printf("Value of pointer %d\n", arr[i]);
        i++;
    }
    gettimeofday(&tv_end,NULL);
    unsigned int t = tv_end.tv_sec-tv_start.tv_sec;
    printf("Total Time taken for saturation blocks is: %d\n secs",tv_end.tv_sec-tv_start.tv_sec);
    printf("..........Saturation test complete.........\n"); 
    unsigned int x = i;
    //prettyPrint();
    printf("........Starting Intermediate Colescence Test ..........\n");
    printf("Now freeing up locations\n\n");
    i=0;
    while(arr[i]!=NULL && i<524291){
        free(arr[i]);
        i++;
    }
    printf("Freeing up done\n\n");
    //prettyPrint();
    
    //Allocating our maximum allocation
    char * ptr_1 = (char*)malloc(8388608);
    printf("Printing after maximum allocation\n");
    if(ptr_1 != NULL){
        printf("Test successfull\n");
        //prettyPrint();
    }
    else{
        printf("test failed");
    }
    //prettyPrint();
    //Freeing up maximum allocaton
    printf("Freeing up maximum allocation\n");
    free(ptr_1);
    //prettyPrint();
    //printf("Pointers allocated = %d\n",x);
    printf("Time taken for saturation is = %d, sec", t);

}



int main (){
    
    consistancy();
    maximization();
    basicCoalescence();
    //These will take 20 mins each to finish as all of them include saturation individually
    //saturation();
    //Both these fucntions will give deallocation errors due to extra pointers in array.
    IntermediateCoalescence();
    TimeOverhead();
    
    
    return 0;
}