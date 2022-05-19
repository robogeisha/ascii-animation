

#include <stdio.h>
#include <time.h>

void delay(int milliseconds);

int main(){

int rowlimit = 80;                 // shooting range
int shooting_speed = 10;           // shooting speed in Millseconds
char  my_sign='-';                 // my sign (my bullet) is now hyphen
int my_counter=0;

while(1) {
    if (my_counter >= rowlimit) {
    my_counter = 0;
    } else{
        printf("\e[1;1H\e[2J");         // Clear Screen
        printf("    __\n");             // Draw Cowboy
        printf("  _|__|_\n");
        printf("  (⌐■_o)︻╦╤─── %*c\n", my_counter, my_sign); //* is mapped to the counter variable 
        
        delay(shooting_speed);          // LoopTime set
        ++my_counter;
}

}                               // never ending while loop
    
}                               // main ends here

void delay(int milliseconds)  
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}