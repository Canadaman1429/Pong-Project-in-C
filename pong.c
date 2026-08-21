/*
    To run, open Linux distro, navigate to correct folder path (cd /mnt/c/Users/InsertYourUsername/InsertFolderName)
    Run commands: 
    gcc (your compiler) pong.c -o pong.exe -lncurses
    ./pong.exe
    Must install ncurses lib in your Linux distro to work (sudo apt update && sudo apt install libncurses5-dev libncursesw5-dev)

    Made by CCC
    Really shitty non trivial project to improve skill
    Use W and S to control paddle
    IDE Used: VSCodium
*/ 

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h> // Get terminal to behave like a gamescreen, must 
#include <unistd.h> // Low level file operations, used for usleep()
#include <math.h> // Needed for absolute values

#define DELAY 30000

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef i8 b8;
typedef i32 b32;

typedef struct {
    f32 x;
    f32 y;
} vec2f;

typedef struct {
    char* name; // String equivalent
    i32 points;
} player;

int main(int argc, char* argv[]) { // Allow process to accept command line arguments from the OS when running

    vec2f screen = {0, 0}; // Reinitialize to actual size later
    
    vec2f ballPos = {0, 0};
    vec2f ballV = {1, 1};

    vec2f paddlePos = {0, 0};
    vec2f paddleV = {0, 0};

    player p1 = {"Player 1", 0};
    player p2 = {"CPU", 0};

    i32 p1y = 5, p2y = 5;
    i32 p1x = 2, p2x = 0; // Reinitialize p2x later when screensize is as well
    
    initscr(); // Global variable stdscr is autocreated with call
    noecho(); // No visible user input on gameboard
    curs_set(false); // No cursor
    keypad(stdscr, true); // Enable keyboard keys
    nodelay(stdscr, true); // No longer pauses for user input when getch() is called

    
    while(true) {
        getmaxyx(stdscr, screen.y, screen.x); // Get dimensions of screen
        p2x = (i32)screen.x - 3; // Reinitialize CPU's x-axis paddle with now given screensize
        clear(); // For frame by frame with the ball 


        // Paddle movement for player
        i32 control = getch();
        if (control == 'w') {
            if (p1y > 0) {
                p1y--; // Keep inside top boundary
            }
        }
        if (control == 's') {
            if (p1y + 5 < screen.y) {
                p1y++; // Keep inside bottom boundary
            }
        }

        // Paddle movement for CPU
        i32 p2_center = p2y + (5 / 2);
        if (ballPos.y > p2_center && p2y + 5 < screen.y) {
            p2y++;
        } 
        else if (ballPos.y < p2_center && p2y > 0) {
            p2y--;
        }
        

        // Ball movement
        ballPos.x += ballV.x;
        ballPos.y += ballV.y / 8;

        if (ballPos.x >= screen.x - 2) {
            ballV.x = -ballV.x;
        }
        if (ballPos.x < 0) {
            ballV.x = fabsf(ballV.x); // abs() but for floats
        }
        if (ballPos.y >= screen.y - 2) {
            ballV.y = -ballV.y;
            
        }
        if (ballPos.y < -1) {
            ballV.y = fabsf(ballV.y);
        }
        

        // Player paddle collision
        if ((i32)ballPos.x == p1x + 1 && (i32)ballPos.y >= p1y && (i32)ballPos.y < p1y + 5) {
            ballV.x = fabsf(ballV.x);
            // Random y-direction upon collision
            i32 random = (rand() % 3);
            if (random == 1) {
                ballV.y = -ballV.y;
            }
        }

        // CPU paddle collision
        if ((i32)ballPos.x == p2x - 2 && (i32)ballPos.y >= p2y && (i32)ballPos.y < p2y + 5) {
            ballV.x = -fabsf(ballV.x);
            // Random y-direction upon collision
            i32 random = (rand() % 3);
            if (random == 1) {
                ballV.y = -ballV.y;
            }
        }

        // Ball past left edge then CPU scores
        if (ballPos.x < 0) {
            p2.points++;
            ballPos.x = screen.x / 2;
            ballPos.y = screen.y / 2;
            ballV.x = 1;
        }

        // Ball past right edge then Player 1 scores
        if (ballPos.x >= screen.x) {
            p1.points++;
            ballPos.x = screen.x / 2;
            ballPos.y = screen.y / 2;
            ballV.x = -1;
        }
        

        // Draw scoreboard
        mvprintw(0, (screen.x / 2) - (screen.x / 5), "%s                        %d  vs.  %d                        %s", p1.name, p1.points, p2.points, p2.name);

        // Draw Player 1 paddle
        for (i32 i = 0; i < 5; i++) {
            mvaddch(p1y + i, p1x, '|');
        }

        // Draw CPU paddle
        for (i32 i = 0; i < 5; i++) {
            mvaddch(p2y + i, p2x, '|');
        }

        mvprintw((i32)ballPos.y, (i32)ballPos.x, "o"); // Ncurses uses (y, x) instead of (x, y)
        
        refresh();

        usleep(DELAY); // Use of delay to prevent getch() from running millions of times in while loop

    } // End while


    endwin(); // End window and restore normal terminal
    return 0;


} // End main