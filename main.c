#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t ram[4096];
    uint8_t v[16];
    int16_t i;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[16];
} Chip8;

void chip8_init(Chip8 *chip8);


int main(void) {
    Chip8 chip8;
    chip8_init(&chip8);

    printf("Mon processeur Chip8 est émulé ET initialisé!");
    return 0;
}

void chip8_init(Chip8 *chip8) {
    memset(chip8->ram, 0, sizeof(chip8->ram));
    memset(chip8->v, 0, sizeof(chip8->v));
    chip8->i = 0;
    chip8->pc = 0x200;
    chip8->sp = 0;
    memset(chip8->stack, 0, sizeof(chip8->stack));
};