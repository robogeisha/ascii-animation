# ascii-animation


cowboy_shoot_move.c

This simple terminal-based cowboy shooting game allows the player to control a cowboy character and shoot bullets. The player can move the cowboy up, down, left, and right using the arrow keys and shoot bullets using the spacebar. The game runs in an infinite loop, and the player can keep shooting bullets and moving the cowboy around the screen.

Instructions:

Compile the C code using GCC with the following command: gcc cowboy_shoot_move.c -o cowboy_shoot_move
Run the compiled program with ./cowboy_shoot_move
Use the arrow keys to move the cowboy around the screen.
Press the spacebar to shoot bullets.
Press 'CTRL + C' to exit the game.
Code Explanation:

The code defines constants for the maximum number of bullets, bullet speed, bullet symbol, cowboy symbol, screen height, and screen width.
The bullet struct holds information about a bullet, such as its position, row, and whether it is active.
The print_cowboy function prints the cowboy and bullets on the screen.
The kbhit function checks if a keyboard input is available.
The flush_input_buffer function flushes the input buffer to avoid repeated inputs.
The handle_input function processes user input to move the cowboy and shoot bullets.
The main function initializes the cowboy, bullets, and terminal settings, then runs an infinite loop to handle user input, move bullets, and update the screen.
Dependencies:

This code requires the C standard library.
The code has been tested on Unix-like systems (macOS, Linux) and may not work as expected on other platforms.
Note:

As this is a simple terminal-based game, it may experience occasional screen flickering or visual artifacts.
