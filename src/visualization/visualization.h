#ifndef VISUALIZATION_H 
#define VISUALIZATION_H

#include "./../table/philosophers.h"

typedef struct {
    int num_of_philosophers;
    philosopher_t *philosophers;
} draw_table_args_t;

typedef struct philosopher {
    int id;
    int state;             // The state of the philosopher (e.g., THINKING, HUNGRY, EATING)
    int state_counter;       // The timer to track how long they are in the current state
} philosopher_visualization_t;

void spawn_drawing_thread(int num_of_philosophers, philosopher_t *philosophers, pthread_t *thread);

#endif

