#include "ncurses.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "./visualization.h"
#include "./../table/philosophers.h"

void draw_table(int num_of_philosophers, philosopher_t *philosophers, chopstick_t *chopsticks) {
    // Args:
    // num_of_philosophers: number of philosophers to draw
    // philosophers: pointer to list of philosophers to draw
    // 
    // draw_table draws the table with philosophers and their states

    // Visualization logic setup
    philosopher_visualization_t *philosophers_visualization = malloc(
        num_of_philosophers * sizeof(philosopher_visualization_t));

    chopstick_visualization_t *chopsticks_visualization = malloc(
        num_of_philosophers * sizeof(chopstick_visualization_t));

    for (int i = 0; i < num_of_philosophers; i++) {
        chopsticks_visualization[i].owner = chopsticks[i].owner;
    }

    // Ncurses boilerplate 
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    clear();

    // Color support
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color.\n");
        exit(1);
    }
    start_color();

    // Philosopher colors
    init_pair(1, COLOR_BLUE, COLOR_BLACK);   // THINKING
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // HUNGRY
    init_pair(3, COLOR_RED, COLOR_BLACK);  // EATING
    
    // Chopstick colors
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);  // OWNED
    init_pair(5, COLOR_CYAN, COLOR_BLACK);  // BORROWED

    while (1) {
        erase();

        int x = 0;
        int y = 0;

        // Chopstick drawing
        for (int i = 0; i < num_of_philosophers; i++) {
            if (chopsticks_visualization[i].owner == chopsticks[i].owner) {
                    attron(COLOR_PAIR(4));
                    mvprintw(y, x, "Owner %d\n", chopsticks[i].owner->id);
                    attroff(COLOR_PAIR(4));
            } else if (chopsticks_visualization[i].owner != chopsticks[i].owner) {
                    attron(COLOR_PAIR(5));
                    mvprintw(y, x, "Borrower %d\n", chopsticks[i].owner->id);
                    attroff(COLOR_PAIR(5));
            } else {
                    mvprintw(y, x, "Unknown owner\n");
            }
        
            y++;
            x = 0;

            // Philosopher drawing
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
            }

            printw(" %s\n", state_str);

            y++; 
        }

        refresh();
        usleep(1000);
    }

    // End ncurses at the end (add cleanup if you want to exit the loop)
    endwin();
}

void *draw_table_wrapper(void *arg) {
    draw_table_args_t *args = (draw_table_args_t *)arg; // Cast void pointer to draw_table_args_t pointer
    draw_table(args->num_of_philosophers, args->philosophers, args->chopsticks); // Call draw_table with arguments
    free(args); // Free memory allocated for arguments
    return NULL;
}

void spawn_drawing_thread(
    int num_of_philosophers, 
    philosopher_t *philosophers, 
    chopstick_t *chopsticks, 
    pthread_t *thread
) {
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
    args->chopsticks = chopsticks;

    pthread_create(thread, NULL, draw_table_wrapper, (void *)args);
}

