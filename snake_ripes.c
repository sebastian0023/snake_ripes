#include "ripes_system.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define RED_COLOR 0xff0000
#define GREEN_COLOR 0x00ff00
#define BLACK_COLOR 0x000000
#define BLUE_COLOR 0x0000ff
#define INITIAL_SNAKE_SIZE 3;
#define SNAKE_ALIVE 1
#define SNAKE_DEAD 2
#define SNAKE_WIN 3

volatile unsigned int *led_base = LED_MATRIX_0_BASE;
volatile unsigned int *pad_up = D_PAD_0_UP;
volatile unsigned int *pad_down = D_PAD_0_DOWN;
volatile unsigned int *pad_right = D_PAD_0_RIGHT;
volatile unsigned int *pad_left = D_PAD_0_LEFT;

const int led_width = LED_MATRIX_0_WIDTH;
const int led_height = LED_MATRIX_0_HEIGHT;
const int led_size = LED_MATRIX_0_SIZE; 

const int right_offset = 1;
const int left_offset = -1;
const int up_offset = -led_width;
const int down_offset = led_width;


int rand_time = 50;


int tail_index;


int *snake_parts[LED_MATRIX_0_WIDTH * LED_MATRIX_0_HEIGHT];


int game_state;


typedef enum _my_bool { false, true } boolean;
