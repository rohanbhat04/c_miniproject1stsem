#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#define HEIGHT 10
#define WIDTH 25
#define COIN '*'
#define PACMAN 'P'
#define GHOST 'G'

struct PacMan {
    int x;
    int y;
};

struct Ghost {
    int x;
    int y;
};

void initialize(char playground[HEIGHT][WIDTH], struct PacMan *pacman, struct Ghost *ghost) {
    // Initialize the playground based on the provided layout
    char playfield[HEIGHT][WIDTH] = {
        "#########################",
        "#                       #",
        "#   ##################  #",
        "#                 ###   #",
        "#   ####   ###    ###   #",
        "#   ####   ###    ###   #",
        "#   ####                #",
        "#     ################  #",
        "#                    * *#",
        "#########################"
    };

    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (playfield[i][j] == ' ') {
                playground[i][j] = COIN;  // Place coins in empty spaces
            } else {
                playground[i][j] = playfield[i][j];  // Copy other characters
            }
        }
    }

    // Initialize Pac-Man's position
    pacman->x = 1;  // Adjust the starting position as needed
    pacman->y = 1;

    // Initialize Ghost's position
    ghost->x = 8;  // Adjust the starting position as needed
    ghost->y = 5;

    // Place Pac-Man and Ghost in the playground
    playground[pacman->x][pacman->y] = PACMAN;
    playground[ghost->x][ghost->y] = GHOST;
}

// ... (rest of the code remains the same)

void display(char playground[HEIGHT][WIDTH]) {
    system("cls");
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (playground[i][j] == PACMAN) {
                printf("%c", PACMAN);
            } else if (playground[i][j] == GHOST) {
                printf("%c", GHOST);
            } else {
                printf("%c", playground[i][j]);
            }
        }
        putchar('\n');
    }
}
// ...

void movePacMan(struct PacMan *pacman, int dx, int dy, char playground[HEIGHT][WIDTH]) {
    int new_x = pacman->x + dx;
    int new_y = pacman->y + dy;

    if (new_x >= 0 && new_x < HEIGHT && new_y >= 0 && new_y < WIDTH &&
        playground[new_x][new_y] != '#') {
        // Clear the current position
        playground[pacman->x][pacman->y] = ' ';

        // Update Pac-Man's position
        pacman->x = new_x;
        pacman->y = new_y;

        // Place Pac-Man in the new position
        playground[pacman->x][pacman->y] = PACMAN;
    }
}

void moveGhost(struct Ghost *ghost, struct PacMan pacman, char playground[HEIGHT][WIDTH]) {
    // Implement ghost movement logic here
    int dx = 0, dy = 0;

    // Calculate the direction towards Pac-Man
    if (ghost->x < pacman.x) {
        dx = 1;
    } else if (ghost->x > pacman.x) {
        dx = -1;
    }

    if (ghost->y < pacman.y) {
        dy = 1;
    } else if (ghost->y > pacman.y) {
        dy = -1;
    }

    int new_x = ghost->x + dx;
    int new_y = ghost->y + dy;

    // Check if the next position is a wall
    if (new_x >= 0 && new_x < HEIGHT && new_y >= 0 && new_y < WIDTH &&
        playground[new_x][new_y] != '#') {
        // Clear the current position
        playground[ghost->x][ghost->y] = ' ';

        // Update Ghost's position
        ghost->x = new_x;
        ghost->y = new_y;

        // Place Ghost in the new position
        playground[ghost->x][ghost->y] = GHOST;
    } else {
        // If the next position is a wall, make a random valid move
        int valid_moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int num_moves = 4;

        while (num_moves > 0) {
            int random_index = rand() % 4;
            int random_dx = valid_moves[random_index][0];
            int random_dy = valid_moves[random_index][1];

            int random_new_x = ghost->x + random_dx;
            int random_new_y = ghost->y + random_dy;

            if (random_new_x >= 0 && random_new_x < HEIGHT && random_new_y >= 0 &&
                random_new_y < WIDTH && playground[random_new_x][random_new_y] != '#') {
                // Clear the current position
                playground[ghost->x][ghost->y] = ' ';

                // Update Ghost's position with the random move
                ghost->x = random_new_x;
                ghost->y = random_new_y;

                // Place Ghost in the new position
                playground[ghost->x][ghost->y] = GHOST;
                break;
            }

            // Mark the chosen move as invalid
            valid_moves[random_index][0] = valid_moves[num_moves - 1][0];
            valid_moves[random_index][1] = valid_moves[num_moves - 1][1];
            num_moves--;
        }
    }
}


int main() {
    char playground[HEIGHT][WIDTH];

    struct PacMan pacman;
    struct Ghost ghost;
    initialize(playground, &pacman, &ghost);

int dx = 0, dy = 0;  // Initialize delta values

while (1) {
    display(playground);

    // Check for user input
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
            case 'w':
                dx = -1;
                dy = 0;
                break;
            case 's':
                dx = 1;
                dy = 0;
                break;
            case 'a':
                dx = 0;
                dy = -1;
                break;
            case 'd':
                dx = 0;
                dy = 1;
                break;
            case 'x':
                exit(0);
                break;
        }
    }

    // Move Pac-Man
    movePacMan(&pacman, dx, dy, playground);

    // Move the Ghost
    moveGhost(&ghost, pacman, playground);

    // Check if Pac-Man has collected a coin
    if (pacman.x >= 0 && pacman.x < HEIGHT && pacman.y >= 0 && pacman.y < WIDTH) 
    if (playground[pacman.x][pacman.y] == COIN) {
        // Clear the coin
        playground[pacman.x][pacman.y] = ' ';
    }

    // Check if Pac-Man has collected all coins
    int coins = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (playground[i][j] == COIN) {
                coins++;
            }
        }
    }

    if (coins == 0) {
        display(playground);
        printf("You win!\n");
        exit(0);
    }

    // Check if ghost caught Pac-Man
    if (pacman.x == ghost.x && pacman.y == ghost.y) {
        display(playground);
        printf("Game over! Ghost caught Pac-Man.\n");
        exit(0);
    }

    Sleep(200);  // Adjust the speed of the game
}
}