#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#include "bag.h"


sem_t semaphore;




bag_t * bb_create(int size)
{
    assert(size > 0);

    bag_t *bag=malloc(sizeof(bag_t));
    assert(bag != NULL);

    bag->elem = malloc( size * sizeof(void*));
    assert(bag->elem != NULL);

    bag->size  = size;

    bag->count = 0;

    bag->is_closed = 0;
    sem_init(&bag->vide, 0, 1);
    sem_init(&bag->plein, size, 1);

    return bag;
}

void bb_add(bag_t * bag, void * element)
{
    sem_post(&(bag->vide));
    assert(bag != NULL);                 // sanity check

    while( bag->count >= bag->size ) { } // CAUTION: this synchronization is bogus
    
    assert( bag-> is_closed == 0 );   // adding to a closed bag is an error



    assert( bag->count < bag->size ); // sanity check

    bag->elem[bag->count] = element;
    bag->count += 1;
    sem_post(&(bag->plein));
}

void * bb_take(bag_t *bag)
{
    sem_wait(&(bag->plein));
    assert(bag != NULL); // sanity check

    while( bag->count <= 0 ) { } // CAUTION: this synchronization is bogus
    

    assert( bag->count > 0); // sanity check

    bag->count -= 1;
    void *r = bag->elem[bag->count];

    sem_wait(&(bag->vide));
    usleep(10);// artificial delay to increase the occurence of race conditions
    return r;
}

void bb_close(bag_t *bag, int N)
{
    assert("not implemented" == 0);
}
