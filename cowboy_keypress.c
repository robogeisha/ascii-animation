#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>

#define MAX_BULLETS 10
#define BULLET_SPEED 2
#define BULLET_SYMBOL "-"
#define COWBOY_SYMBOL "(⌐■_o)︻╦╤-"

struct bullet {
    int position;
    int active;
};

void print_cowboy(int bullet_positions[MAX_BULLETS])
{
    printf("\033[2J"); // Clear entire screen
    printf("\033[H"); // Move cursor to top-left corner

    // Print the Cowboy
    printf("  __\n");
    printf("_|__|_\n");
    printf("%s\n", COWBOY_SYMBOL);

    // Print the bullets at the correct positions
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet_positions[i] >= 0) {
            printf("\033[%d;%dH%s\n", 3, bullet_positions[i] - 5, BULLET_SYMBOL);

        }
    }
}

int kbhit(void)
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int main()
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    struct bullet bullets[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].position = -1;
        bullets[i].active = 0;
    }

    // Print the Cowboy and bullets at the beginning
    int bullet_positions[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet_positions[i] = bullets[i].position;
    }
    print_cowboy(bullet_positions);

    while (1) {
        // Check if the space key is pressed
        if (kbhit() && getchar() == ' ') {
            // Activate a new bullet
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].position = strlen(COWBOY_SYMBOL) - 4;
                    bullets[i].active = 1;
                    break;
                }
            }
        }

        // Animate the bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].position += BULLET_SPEED;
                if (bullets[i].position >= 80) {
                    bullets[i].position = -1;
                    bullets[i].active = 0;
                }
            }
        }

        // Print the cowboy and bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullet_positions[i] = bullets[i].position; 
        }
        print_cowboy(bullet_positions);

        usleep(100000); // Sleep for 100ms
    }
}