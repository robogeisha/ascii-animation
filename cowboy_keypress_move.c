#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>

#define MAX_BULLETS 20
#define BULLET_SPEED 1
#define BULLET_SYMBOL "-"
#define COWBOY_SYMBOL "(⌐■_o)︻╦╤-"
#define SCREEN_HEIGHT 24
#define SCREEN_WIDTH 80

struct bullet {
    int position;
    int row;
    int active;
};

void print_cowboy(int cowboy_row, int cowboy_col, struct bullet bullets[MAX_BULLETS]) {
    printf("\033[2J"); // Clear entire screen
    printf("\033[H"); // Move cursor to top-left corner

    // Print the Cowboy
    for (int i = 0; i < cowboy_row; i++) {
        printf("\n");
    }
    for (int i = 0; i < cowboy_col; i++) {
        printf(" ");
    }
    printf("  __\n");
    for (int i = 0; i < cowboy_col; i++) {
        printf(" ");
    }
    printf("_|__|_\n");
    for (int i = 0; i < cowboy_col; i++) {
        printf(" ");
    }
    printf("%s\n", COWBOY_SYMBOL);

    // Print the bullets at the correct positions
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            printf("\033[%d;%dH%s", bullets[i].row + 3, bullets[i].position, BULLET_SYMBOL);
        }
    }
    fflush(stdout);
}

int kbhit(void) {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

void flush_input_buffer() {
    tcflush(STDIN_FILENO, TCIFLUSH);
}

void handle_input(int *cowboy_row, int *cowboy_col, struct bullet bullets[MAX_BULLETS]) {
    if (kbhit()) {
        int c = getchar();
        if (c == ' ') {
            // Activate a new bullet
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].position = *cowboy_col + strlen(COWBOY_SYMBOL)-8;
                    bullets[i].row = *cowboy_row;
                    bullets[i].active = 1;
                    break;
                }
            }
            flush_input_buffer();
        } else if (c == '\033') {
            // Arrow key was pressed

            getchar(); // Skip the [
            int arrow = getchar();

            // Move the cowboy
            switch (arrow) {
                case 'A': // Up arrow
                    if (*cowboy_row > 0) (*cowboy_row)--;
                    break;
                case 'B': // Down arrow
                    if (*cowboy_row < SCREEN_HEIGHT - 4) (*cowboy_row)++;
                    break;
                case 'C': // Right arrow
                    if (*cowboy_col < SCREEN_WIDTH - strlen(COWBOY_SYMBOL)) (*cowboy_col)++;
                    break;
                case 'D': // Left arrow
                    if (*cowboy_col > 0) (*cowboy_col)--;
                    break;
            }
            flush_input_buffer();
        }
    }
}

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    struct bullet bullets[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].position = -1;
        bullets[i].row = 0;
        bullets[i].active = 0;
    }

    int cowboy_row = 0;
    int cowboy_col = 0;
    print_cowboy(cowboy_row, cowboy_col, bullets);

    while (1) {
        handle_input(&cowboy_row, &cowboy_col, bullets);

        // Move the bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].position += BULLET_SPEED;
                if (bullets[i].position >= SCREEN_WIDTH) {
                    bullets[i].position = -1;
                    bullets[i].active = 0;
                }
            }
        }

        // Print the cowboy and bullets
        print_cowboy(cowboy_row, cowboy_col, bullets);

        usleep(50000); // Sleep for 50ms
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
