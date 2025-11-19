#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H
#include <stdint.h>

typedef struct {
    uint8_t ram[4096];
    uint8_t v[16];
    int16_t i;
    uint16_t pc;
    uint16_t sp;
    uint16_t stack[16];
    uint8_t display[32][64];
} Chip8;

void chip8_init(Chip8 *chip8);
uint16_t chip8_fetch(Chip8 *chip8);
void jump_to_NNN(Chip8 *chip8, uint16_t NNN);
void set_vX_to_NN(Chip8 *chip8, uint8_t X, uint8_t NN);
void increase_vX_of_NN(Chip8 *chip8, uint8_t X, uint8_t NN);
void chip8_cycle(Chip8 *chip8);
void return_00EE (Chip8 *chip8);
void call_NNN(Chip8 *chip8, uint16_t NNN);
void draw_sprite(Chip8 *chip8, uint8_t vX, uint8_t vY, uint8_t N);

#endif //CHIP8_CHIP8_H