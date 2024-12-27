#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>

typedef struct philosopher_t philosopher_t;

typedef enum {
    CLEAN,
    DIRTY,
} chopstick_state_t;

typedef struct {
    pthread_mutex_t mutex; //Mutex for chopstick
    pthread_cond_t condition; //Condition variable for waiting
    philosopher_t *owner; //Philosopher that owns the chopstick
    chopstick_state_t state;
} chopstick_t;

typedef enum {
    THINKING,
    HUNGRY,
    EATING,
} philosopher_state_t;

typedef struct {
    int id;
    chopstick_t *left_chopstick;
    chopstick_t *right_chopstick;
    philosopher_state_t state;
} philosopher_t;

void init_table(int num_of_philosophers, philosopher_t *philosophers, chopstick_t *chopsticks);

#endif

