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

// Constantes de tamaños
#define LED_MATRIX_0_SIZE	(0xdac)
#define LED_MATRIX_0_WIDTH	(0x23)
#define LED_MATRIX_0_HEIGHT	(0x19)

// Apuntadores a diferentes partes importantesm base y dpad
volatile unsigned int * led_base = (volatile unsigned int *)LED_MATRIX_0_BASE;

volatile unsigned int * d_pad_up = (volatile unsigned int *)D_PAD_0_UP;
volatile unsigned int * d_pad_do = (volatile unsigned int *)D_PAD_0_DOWN;
volatile unsigned int * d_pad_le = (volatile unsigned int *)D_PAD_0_LEFT;
volatile unsigned int * d_pad_ri = (volatile unsigned int *)D_PAD_0_RIGHT;

// Tamaño de la matriz
#define MAX_SNAKE_SIZE (LED_MATRIX_0_WIDTH * LED_MATRIX_0_HEIGHT)

// Variables
int game = 1;
int head = 1;
int tail = 0;

int randcounter = 50;

int size = 2;

int new_head_index;

int dx = 0;
int dy = 1;

// Arreglo donde se guardan las head y tails
volatile unsigned int snakeLEDs[MAX_SNAKE_SIZE];

void initSnake() {
    int st1 = (LED_MATRIX_0_WIDTH + 2);
    int st2 = (LED_MATRIX_0_WIDTH * 2 + 2);
    *(led_base + st1) = 0x00FF00; // Primer bloque de 2x4
    *(led_base + st1 + 1) = 0x00FF00;
    *(led_base + st1 + LED_MATRIX_0_WIDTH) = 0x00FF00;
    *(led_base + st1 + LED_MATRIX_0_WIDTH + 1) = 0x00FF00;

    *(led_base + st2) = 0x00FF00; // Segundo bloque de 2x4
    *(led_base + st2 + 1) = 0x00FF00;
    *(led_base + st2 + LED_MATRIX_0_WIDTH) = 0x00FF00;
    *(led_base + st2 + LED_MATRIX_0_WIDTH + 1) = 0x00FF00;

    snakeLEDs[tail] = st1;
    snakeLEDs[head] = st2;
}
