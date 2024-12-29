#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>

typedef struct _philosopher_t philosopher_t;
typedef struct _chopstick_t chopstick_t;

typedef enum {
    CLEAN,
    DIRTY,
} chopstick_state_t;

typedef struct _chopstick_t {
    pthread_mutex_t mutex; 
    pthread_cond_t *condition; 
    philosopher_t *owner; 
    chopstick_state_t state;
} chopstick_t;

typedef enum {
    THINKING,
    HUNGRY,
    EATING,
} philosopher_state_t;

typedef struct _philosopher_t {
    int id;
    chopstick_t *left_chopstick;
    chopstick_t *right_chopstick;
    philosopher_state_t state;
} philosopher_t;

void init_table(int num_of_philosophers, philosopher_t *philosophers, chopstick_t *chopsticks);

void spawn_philosophers(philosopher_t *philosopher, int num_of_philosophers, pthread_t *threads);

#endif

