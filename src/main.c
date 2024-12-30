#include "stdio.h"
#include "stdlib.h"
#include "./table/philosophers.h"
#include "./visualization/visualization.h"

// Using Chandy/Misra algorithm
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments\n");
        return 1;
    }
    
    int num_of_philosophers = atoi(argv[1]);
    
    if (num_of_philosophers < 5) {
        printf("Number must be equal or greater than 5\n");
        return 1;
    }
   
    philosopher_t *philosophers = malloc(num_of_philosophers * sizeof(philosopher_t));
    chopstick_t *chopsticks = malloc(num_of_philosophers * sizeof(chopstick_t));

    if (philosophers == NULL || chopsticks == NULL) {
        perror("Failed to allocate memory for philosophers or chopsticks");
        return 1;
    }

    pthread_t philosopher_threads[num_of_philosophers];
    pthread_t drawing_thread;

    init_table(num_of_philosophers, philosophers, chopsticks);

    spawn_philosophers(philosophers, num_of_philosophers, philosopher_threads);

    spawn_drawing_thread(num_of_philosophers, philosophers, chopsticks, &drawing_thread); 

    // --Clean up--
    pthread_join(drawing_thread, NULL);

    for (int i = 0; i < num_of_philosophers; i++) {
        pthread_join(philosopher_threads[i], NULL);
    }

    free(philosophers);
    free(chopsticks);

    return 0;
}

