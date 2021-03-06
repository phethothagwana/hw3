/* A simple test harness for memory alloction. */

#include "mm_alloc.h"
#include <stdio.h>
#include <assert.h>

int stuff;

int main(int argc, char **argv)
{
    int *data;  
    /* Variation 1 */
    /* Simple test: invoke mm_malloc to allocate 4 byes and use mm_free to free it */
    data = (int*) mm_malloc(4);
   
    printf("_main @ %lx\n", (long unsigned int) main);
    printf("_main stack %lx\n", (long unsigned int) &data);
    printf("static data: %lx\n", (long unsigned int) &stuff);
    printf("Heap: malloc: %lx\n", (long unsigned int) data);

    data[0] = 1;
    mm_free(data);
    printf("Malloc sanity test successful!\n");

    /* Variation 2 */
    /* Allocate zero byte: invoke mm_malloc to allocate 0 byes and use mm_free to free it */
    data = (int*) mm_malloc(0);
    /* data[0] = 0; */
    mm_free(data);
    printf("Zero test successful!\n");

    /* Variation 3 */
    /* Allocate zero byte: invoke mm_malloc to allocate 0 byes and use mm_free to free it */
    data = (int*) mm_malloc(-1);
    /* data[0] = 0; */
    mm_free(data);
    printf("negative test successful!\n");

    /* Variation 4 */
    /* Verify the first fitting algorithm works correctly */
    void* array[10];
    array[0] = (int*)mm_malloc(5);
    array[1] = (int*)mm_malloc(10);
    array[2] = (int*)mm_malloc(4);
    array[3] = (int*)mm_malloc(15);
    array[4] = (int*)mm_malloc(10);
    /* data[0] = 0; */
    mm_free(array[3]);
    array[5] = (int*)mm_malloc(10);
    printf("array[3] %x, array[5] %x\n", array[3], array[5]);
    mm_free(array[4]);
    mm_free(array[0]);
    mm_free(array[1]);
    mm_free(array[2]);
	
    assert(array[3] == array[5]);
    mm_free(array[5]);
    printf("The first fitting algorithm works correctly!\n");
	
    array[1] = (int*)mm_malloc(5);
    fprintf(stderr, "array[0]: %x array[1]: %x\n", array[0], array[1]);
    assert(array[0] == array[1]);
    fprintf(stderr, "OK\n");
    mm_free(array[1]);
    printf("The first fitting algorithm works correctly!\n");

    array[1] = (int*)mm_malloc(20);
    fprintf(stderr, "array[0]: %x array[1]: %x\n", array[0], array[1]);
    assert(array[1] > array[4]);
    fprintf(stderr, "OK\n");
    mm_free(array[1]);
    printf("The first fitting algorithm works correctly!\n");

    array[0] = (int*)mm_malloc(5);
    array[1] = (int*)mm_malloc(10);
    array[2] = (int*)mm_malloc(4);
    array[3] = (int*)mm_malloc(15);
    array[4] = (int*)mm_malloc(10);
    mm_free(array[1]); 
    mm_free(array[2]); 
    array[2] = mm_malloc(14);
    assert(array[1] == array[2]);
    array[2] = mm_malloc(14);
    mm_free(array[0]);
    mm_free(array[3]);
    array[0] = mm_malloc(14);
    assert(array[0] == array[3]);
    mm_free(array[0]);
    mm_free(array[2]);
    printf("The fusion function works correctly!\n");
     
    return 0;
}
