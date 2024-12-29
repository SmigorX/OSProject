#ifndef VISUALIZATION_H 
#define VISUALIZATION_H

#include "./../table/philosophers.h"

typedef struct {
    int num_of_philosophers;
    philosopher_t *philosophers;
} draw_table_args_t;

void spawn_drawing_thread(int num_of_philosophers, philosopher_t *philosophers, pthread_t *thread);

#endif

