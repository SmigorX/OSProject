#include "philosophers.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 

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
        
        chopsticks[i].condition = malloc(sizeof(pthread_cond_t));
        if (chopsticks[i].condition == NULL) {
            perror("Failed to allocate memory for condition variable");
            exit(EXIT_FAILURE);
        }

        pthread_cond_init(chopsticks[i].condition, NULL);
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

void chopstick_request(chopstick_t *chopstick, philosopher_t *philosopher) {
    // Args:
    // chopstick: pointer to chopstick to request
    // philosopher: pointer to philosopher requesting the chopstick
    // 
    // chopstick_request requests a chopstick for a philosopher
   
    pthread_mutex_lock(&chopstick->mutex);

    //If we own the chopstick do nothing
    if (chopstick->owner == philosopher) {
        pthread_mutex_unlock(&chopstick->mutex);
        return;
    }

    //Waiting for chopstick
    while ((chopstick->owner->state == HUNGRY && chopstick->state == CLEAN) ||
        (chopstick->owner->state == EATING)) {
            
        pthread_cond_wait(chopstick->condition, &chopstick->mutex);
    }

    //Steal the chopstick
    chopstick->owner = philosopher;
    chopstick->state = CLEAN;

    pthread_mutex_unlock(&chopstick->mutex);
    
    return;
}

void simulate_philosopher(philosopher_t *philosopher) {
    // Args:
    // philosopher: pointer to philosopher to simulate
    //
    // simulate_philosopher simulates the philosopher's behavior
   
    while (1) {    
       // Thinking 
        philosopher->state = THINKING;
        usleep(rand() % 1000000);

        // Hungry
        philosopher->state = HUNGRY;
        chopstick_request(philosopher->left_chopstick, philosopher);
        chopstick_request(philosopher->right_chopstick, philosopher);
    
        // Simulate eating
        philosopher->state = EATING;
        philosopher->left_chopstick->state = DIRTY;
        philosopher->right_chopstick->state = DIRTY;
        usleep(rand() % 1000000);
    
        // Cleanup
        pthread_cond_signal(philosopher->left_chopstick->condition);
        pthread_cond_signal(philosopher->right_chopstick->condition);
    }    
}

// Wrapper function to make simulate_philosopher work with pthread_create
void *simulate_philosopher_wrapper(void *arg) {
    philosopher_t *philosopher = (philosopher_t *)arg;  // Cast the void pointer back to philosopher_t *
    simulate_philosopher(philosopher);  // Now pass the philosopher to the function
    return NULL;
}

void spawn_philosophers(philosopher_t *philosophers, int num_of_philosophers, pthread_t *threads) {
    // Args:
    // philosophers: pointer to philosopher list
    // num_of_philosophers: number of philosophers to spawn threads for
    //
    // spawn_philosophers creates a thread for each philosopher

    for (int i = 0; i < num_of_philosophers; i++) {
        pthread_create(&threads[i], NULL, simulate_philosopher_wrapper, (void *)&philosophers[i]);
    }
}

