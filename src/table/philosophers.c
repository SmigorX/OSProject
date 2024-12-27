#include "philosophers.h"
#include <pthread.h>

void init_table(int num_of_philosophers, philosopher_t *philosophers, chopstick_t *chopsticks) {
    // Args: 
    // num_of_philosophers: number of philosophers to create
    // philosophers: pointer to list of philosophers to initialize
    // chopsticks: pointer to list of chopsticks to initialize
    // 
    // init_table initializes philosophers and their chopsticks

    // Initialize chopsticks
    for (int i = 0; i < num_of_philosophers; i++) {
        chopsticks[i].owner = NULL;
        chopsticks[i].state = DIRTY;
        pthread_mutex_init(&chopsticks[i].mutex, NULL);
        pthread_cond_init(&chopsticks[i].condition, NULL);
    }

    // Initialize philosophers
    for (int i = 0; i < num_of_philosophers; i++) {
        philosophers[i].id = i;
        philosophers[i].left_chopstick = &chopsticks[i];
        philosophers[i].right_chopstick = &chopsticks[(i + 1) % num_of_philosophers];
        philosophers[i].state = THINKING;
    }

    //set chopsticks owner to lower id philosopher
    for (int i = 0; i < (num_of_philosophers); i++) {
        chopsticks[i].owner = &philosophers[i];
    }
    //set last chopstick owner to first philosopher to kick-start the process
    chopsticks[num_of_philosophers].owner = &philosophers[0];
}

