#include "ncurses.h"
#include "stdlib.h"
#include "./visualization.h"
#include "./../table/philosophers.h"

void draw_table(int num_of_philosophers, philosopher_t *philosophers) {
    // Args:
    // num_of_philosophers: number of philosophers to draw
    // philosophers: pointer to list of philosophers to draw
    // 
    // draw_table draws the table with philosophers and their states

    // Initialize ncurses if not already initialized in the main program
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Clear the screen before drawing
    clear();

    // Draw table
    int x = 0;
    int y = 0;
    for (int i = 0; i < num_of_philosophers; i++) {
        mvprintw(y, x, "Philosopher %d: ", philosophers[i].id);
        
        switch (philosophers[i].state) {
            case THINKING:
                printw("Thinking");
                break;
            case HUNGRY:
                printw("Hungry");
                break;
            case EATING:
                printw("Eating");
                break;
            default:
                printw("Unknown state");
                break;
        }
        y++;  // Move to the next row for the next philosopher
    }

    // Refresh screen to show the table
    refresh();

    // Wait for user input to exit (optional, can be replaced with another exit condition)
    getch();

    // End ncurses and clean up
    endwin();
}

void *draw_table_wrapper(void *arg) {
    draw_table_args_t *args = (draw_table_args_t *)arg; // Cast void pointer to draw_table_args_t pointer
    draw_table(args->num_of_philosophers, args->philosophers); // Call draw_table with arguments
    free(args); // Free memory allocated for arguments
    return NULL;
}

void spawn_drawing_thread(int num_of_philosophers, philosopher_t *philosophers, pthread_t *thread) {
    // Args:
    // num_of_philosophers: number of philosophers to draw
    // philosophers: pointer to list of philosophers to draw
    // thread: pointer to store the thread
    //
    // Creates thread responsible for visualization of the simulation.

    draw_table_args_t *args = malloc(sizeof(draw_table_args_t));
    
    if (args == NULL) {
        perror("Failed to allocate memory for draw_table_args_t");
        return;
    }
    args->num_of_philosophers = num_of_philosophers;
    args->philosophers = philosophers;

    pthread_create(thread, NULL, draw_table_wrapper, (void *)args);
}

