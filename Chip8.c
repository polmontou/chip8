#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t ram[4096];
    uint8_t v[16];
    int16_t i;
    uint16_t pc;
    uint16_t sp;
    uint16_t stack[16];
} Chip8;






//
// Created by paul.montoussy@Digital-Grenoble.local on 11/6/25.
//