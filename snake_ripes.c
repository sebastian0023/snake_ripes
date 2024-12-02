#include "ripes_system.h"
#include <stdio.h>
#include <stdlib.h>

#define SW0 (0x01)
#define SW1 (0x02)
#define SW2 (0x04)
#define SW3 (0x08)
#define SW4 (0x10)
#define SW5 (0x20)
#define SW6 (0x40)
#define SW7 (0x80)

#define LED_MATRIX_0_SIZE (0xdac)
#define LED_MATRIX_0_WIDTH (0x23)
#define LED_MATRIX_0_HEIGHT (0x19)

volatile unsigned int *led_base = (volatile unsigned int *)LED_MATRIX_0_BASE;
volatile unsigned int *switch_base = SWITCHES_1_BASE;

volatile unsigned int *d_pad_up = (volatile unsigned int *)D_PAD_0_UP;
volatile unsigned int *d_pad_do = (volatile unsigned int *)D_PAD_0_DOWN;
volatile unsigned int *d_pad_le = (volatile unsigned int *)D_PAD_0_LEFT;
volatile unsigned int *d_pad_ri = (volatile unsigned int *)D_PAD_0_RIGHT;

#define MAX_SNAKE_SIZE (LED_MATRIX_0_WIDTH * LED_MATRIX_0_HEIGHT)

int game = 1;
int head = 1;
int tail = 0;
int randcounter = 50;
int size = 2;
int new_head_index;

int dx = 0;
int dy = 1;

volatile unsigned int snakeLEDs[MAX_SNAKE_SIZE];

void initSnake() {
    int st1 = (LED_MATRIX_0_WIDTH + 2);
    int st2 = (LED_MATRIX_0_WIDTH * 2 + 2);
    
    *(led_base + st1) = 0xFF0000;
    *(led_base + st1 + 1) = 0xFF0000;
    *(led_base + st1 + LED_MATRIX_0_WIDTH) = 0xFF0000;
    *(led_base + st1 + LED_MATRIX_0_WIDTH + 1) = 0xFF0000;

    *(led_base + st2) = 0xFF0000;
    *(led_base + st2 + 1) = 0xFF0000;
    *(led_base + st2 + LED_MATRIX_0_WIDTH) = 0xFF0000;
    *(led_base + st2 + LED_MATRIX_0_WIDTH + 1) = 0xFF0000;

    snakeLEDs[tail] = st1;
    snakeLEDs[head] = st2;
}

int moveSnake() {
    if (dy == 1) {
        new_head_index = snakeLEDs[head] + LED_MATRIX_0_WIDTH * 2;
    } else if (dy == -1) {
        new_head_index = snakeLEDs[head] - LED_MATRIX_0_WIDTH * 2;
    } else if (dx == 1) {
        new_head_index = snakeLEDs[head] + 2;
    } else if (dx == -1) {
        new_head_index = snakeLEDs[head] - 2;
    }

    if (*(led_base + new_head_index) == 0x0000FF || 
        *(led_base + new_head_index) == 0xFF0000) {
        return 0;
    }

    int apple_collision = 0;
    if (*(led_base + new_head_index) == 0x00FF00 ||
        *(led_base + new_head_index + 1) == 0x00FF00 ||
        *(led_base + new_head_index + LED_MATRIX_0_WIDTH) == 0x00FF00 ||
        *(led_base + new_head_index + LED_MATRIX_0_WIDTH + 1) == 0x00FF00) {
        apple_collision = 1;
    }

    head = (head + 1) % MAX_SNAKE_SIZE;
    snakeLEDs[head] = new_head_index;

    *(led_base + new_head_index) = 0xFF0000;
    *(led_base + new_head_index + 1) = 0xFF0000;
    *(led_base + new_head_index + LED_MATRIX_0_WIDTH) = 0xFF0000;
    *(led_base + new_head_index + LED_MATRIX_0_WIDTH + 1) = 0xFF0000;

    if (apple_collision) {
        eatApple();
        generateApple();
    } else {
        *(led_base + snakeLEDs[tail]) = 0x0;
        *(led_base + snakeLEDs[tail] + 1) = 0x0;
        *(led_base + snakeLEDs[tail] + LED_MATRIX_0_WIDTH) = 0x0;
        *(led_base + snakeLEDs[tail] + LED_MATRIX_0_WIDTH + 1) = 0x0;
        
        tail = (tail + 1) % MAX_SNAKE_SIZE;
    }

    return 1;
}

void printlimits(int color) {
    for (int i = 0; i < LED_MATRIX_0_WIDTH + 1; i++) {
        *(led_base + i) = color;
        *(led_base + (LED_MATRIX_0_HEIGHT - 1) * LED_MATRIX_0_WIDTH + i) = color;
    }
    
    for (int i = 0; i < LED_MATRIX_0_HEIGHT; i++) {
        *(led_base + i * LED_MATRIX_0_WIDTH) = color; 
        *(led_base + i * LED_MATRIX_0_WIDTH + LED_MATRIX_0_WIDTH - 1) = color;
    }
}

void cleanBoard() {
    for (int i = 0; i < MAX_SNAKE_SIZE; i++) {
        *(led_base + i) = 0x0;
    }
}

void changeDirection(int dex, int dey) {
    if ((dx == 1 && dex == -1) || (dx == -1 && dex == 1) ||
        (dy == 1 && dey == -1) || (dy == -1 && dey == 1)) {
        return;
    }
    
    dx = dex;
    dy = dey;
}

void eatApple() {
    size++;

    int last_apple_pos = -1;
    
    for (int i = 0; i < LED_MATRIX_0_SIZE; i++) {
        if (*(led_base + i) == 0x00FF00) {
            *(led_base + i) = 0x0;
            last_apple_pos = i;
        }
    }
}

void generateApple() {
    int flag = 1;
    int attempts = 0;
    const int MAX_ATTEMPTS = 100;

    while (flag && attempts < MAX_ATTEMPTS) {
        randcounter += 5;
        srand(randcounter);
        
        int random_x = 2 + rand() % (LED_MATRIX_0_WIDTH - 4);
        int random_y = 2 + rand() % (LED_MATRIX_0_HEIGHT - 4);
        int position = random_y * LED_MATRIX_0_WIDTH + random_x;

        if (*(led_base + position) == 0x0 &&
            *(led_base + position + 1) == 0x0 &&
            *(led_base + position + LED_MATRIX_0_WIDTH) == 0x0 &&
            *(led_base + position + LED_MATRIX_0_WIDTH + 1) == 0x0) {
            
            *(led_base + position) = 0x00FF00;
            *(led_base + position + 1) = 0x00FF00;
            *(led_base + position + LED_MATRIX_0_WIDTH) = 0x00FF00;
            *(led_base + position + LED_MATRIX_0_WIDTH + 1) = 0x00FF00;
            flag = 0;
        }

        attempts++;
    }

    if (attempts >= MAX_ATTEMPTS) {
        game = 0;
    }
}

void reset() {
    cleanBoard();
    initSnake();
    printlimits(0x0000FF);
    generateApple();

    while (game) {
        game = moveSnake();

       
        for (int i = 0; i < 20000; i++) {
           
        }

       
        if (*d_pad_up == 1) changeDirection(0, -1);
        if (*d_pad_do == 1) changeDirection(0, 1);
        if (*d_pad_le == 1) changeDirection(-1, 0);
        if (*d_pad_ri == 1) changeDirection(1, 0);

        if (*switch_base & SW0) {
            for (int j = 0; j < 100000; j++) {}
            if (*switch_base & SW0) {
                reset();
            }
        }
    }

    cleanBoard();
}


void main() {
    reset();
}
