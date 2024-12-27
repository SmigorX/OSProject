#ifndef CHOPSTICKS_H
#define CHOPSTICKS_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex; //Mutex for chopstick
    pthread_cond_t condition; //Condition variable for waiting
    philosopher_t *owner; //Philosopher that owns the chopstick
    chopstick_state_t state;
} chopstick_t;

typedef enum {
    CLEAN,
    DIRTY,
} chopstick_state_t;

#endif

