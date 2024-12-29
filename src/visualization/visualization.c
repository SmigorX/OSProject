#include "ncurses.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "./visualization.h"
#include "./../table/philosophers.h"

void draw_table(int num_of_philosophers, philosopher_t *philosophers) {
    // Args:
    // num_of_philosophers: number of philosophers to draw
    // philosophers: pointer to list of philosophers to draw
    // 
    // draw_table draws the table with philosophers and their states

    philosopher_visualization_t *philosophers_visualization = malloc(
        num_of_philosophers * sizeof(philosopher_visualization_t));

    // Initialize ncurses if not already initialized in the main program
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    clear();

    // Initialize color support
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color.\n");
        exit(1);
    }
    start_color();

    // Define color pairs
    init_pair(1, COLOR_BLUE, COLOR_BLACK);   // THINKING: Blue text
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // HUNGRY: Yellow text
    init_pair(3, COLOR_RED, COLOR_BLACK);  // EATING: Green text



    while (1) {
        erase();

        int x = 0;
        int y = 0;
        for (int i = 0; i < num_of_philosophers; i++) {
            mvprintw(y, x, "Philosopher %d: ", philosophers[i].id);

            if (philosophers_visualization[i].state == philosophers[i].state) {
                philosophers_visualization[i].state_counter++;
            } else {
                philosophers_visualization[i].state = philosophers[i].state;
                philosophers_visualization[i].state_counter = 0;
            }

            char state_str[65535] = "";
            
            for (int j = 0; j < philosophers_visualization[i].state_counter / 10; j++) {
                strcat(state_str, "|");
            }

            switch (philosophers[i].state) {
                case THINKING:
                    attron(COLOR_PAIR(0)); // Blue text for THINKING     
                    printw("Thinking");
                    attroff(COLOR_PAIR(0));
                    break;
                case HUNGRY:
                    attron(COLOR_PAIR(1)); // Yellow text for HUNGRY
                    printw("Hungry  ");
                    attroff(COLOR_PAIR(1));
                    break;
                case EATING:
                    attron(COLOR_PAIR(2)); // Green text for EATING
                    printw("Eating  ");
                    attroff(COLOR_PAIR(3));
                    break;
                default:
                    printw("Unknown state");
                    break;
            }

            printw(" %s\n", state_str);
            y++;  // Move to the next row for the next philosopher
        
        }

        // Refresh screen to show the table
        refresh();

        // Sleep for a short time to avoid flickering
        usleep(1000);
    }

    // End ncurses at the end (add cleanup if you want to exit the loop)
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

