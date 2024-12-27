#include "stdio.h"
#include "stdlib.h"
#include "philosophers.h"

// Using Chandy/Misra algorithm
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments\n");
        return 1;
    }
    
    int num = atoi(argv[1]);
    if (num < 5) {
        printf("Number must be equal or greater than 5\n");
        return 1;
    }
   
    philosopher_t *philosophers = malloc(num_of_philosophers * sizeof(philosopher_t));
    chopstick_t *chopsticks = malloc(num_of_philosophers * sizeof(chopstick_t));


    return 0;
}
