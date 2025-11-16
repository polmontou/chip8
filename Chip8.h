typedef struct {
    uint8_t ram[4096];
    uint8_t v[16];
    int16_t i;
    uint16_t pc;
    uint16_t sp;
    uint16_t stack[16];
    uint8_t display[32][64];
} Chip8;

void chip8Init(Chip8 *chip8);
uint16_t chip8Fetch(Chip8 *chip8);
uint8_t chip8Decode(uint16_t instruct);
void jumpToNNN(Chip8 *chip8, uint16_t NNN);
void setVXtoNN(Chip8 *chip8, uint8_t X, uint8_t NN);
void increaseVXofNN(Chip8 *chip8, uint8_t X, uint8_t NN);
void chip8Cycle(Chip8 *chip8);
void return00EE (Chip8 *chip8);
void callNNN(Chip8 *chip8, uint16_t NNN);
void drawSprite(Chip8 *chip8, uint8_t vX, uint8_t vY, uint8_t N);

//
// Created by paulm on 16/11/2025.
//

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#endif //CHIP8_CHIP8_H