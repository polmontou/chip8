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
uint16_t chip8_fetch(Chip8 *chip8);
uint8_t chip8_decode(uint16_t instruct);
void jumpTo1NNN(Chip8 *chip8, uint16_t NNN);
void setVXtoNN(Chip8 *chip8, uint8_t X, uint8_t NN);
void increaseVXofNN(Chip8 *chip8, uint8_t X, uint8_t NN);
void chip8_cycle(Chip8 *chip8);


int main(void) {
    Chip8 chip8;
    chip8_init(&chip8);

    chip8.ram[0x200] = 0x61;
    chip8.ram[0x201] = 0x05;
    chip8.ram[0x202] = 0x6D;
    chip8.ram[0x203] = 0xF5;
    while (chip8.ram[chip8.pc] != 0x00) {
        uint16_t currentPc = chip8.pc;
        chip8_cycle(&chip8);
        printf("Register V[%d] = %02x\n", (chip8.ram[currentPc] & 0x0F), chip8.v[chip8.ram[currentPc] & 0x0F]);
        printf("Next instruction : %03x\n", chip8.pc);
    }
    return 0;
}



void chip8_cycle(Chip8 *chip8) {
    uint16_t instruct = chip8_fetch(chip8);
    uint16_t firstNibble = chip8_decode(instruct);
    uint8_t X = 0, NN = 0;
    uint16_t NNN = 0;
    short pcModified = 0;

    switch(firstNibble) {
        case 0x1:
            NNN = (instruct & 0x0FFF);
            jumpTo1NNN(chip8, NNN);
            pcModified = 1;
            break;
        case 0x3:
            X = (instruct & 0x0F00) >> 8;
            NN = (instruct & 0x00FF);

            if (chip8->v[X] == NN) {
                chip8->pc += 4;
                pcModified = 1;
            }
            break;
        case 0x4:
            X = (instruct & 0x0F00) >> 8;
            NN = (instruct & 0x00FF);
            if (chip8->v[X] != NN) {
                chip8->pc += 4;
                pcModified = 1;
            }
            break;
        case 0x6:
            X = (instruct & 0x0F00) >> 8;
            NN = (instruct & 0x00FF);
            setVXtoNN(chip8, X, NN);
            break;
        case 0x7:
            X = (instruct & 0x0F00) >> 8;
            NN = (instruct & 0x00FF);
            increaseVXofNN(chip8, X, NN);
            break;
    }
    if (pcModified == 0) {
        chip8->pc += 2;
    }
}

void jumpTo1NNN(Chip8 *chip8, uint16_t NNN) {
    chip8->pc = NNN;
}

void setVXtoNN(Chip8 *chip8, uint8_t X, uint8_t NN) {
    chip8->v[X] = NN;
}

void increaseVXofNN(Chip8 *chip8, uint8_t X, uint8_t NN) {
    chip8->v[X] += NN;
}
void chip8_init(Chip8 *chip8) {
    memset(chip8->ram, 0, sizeof(chip8->ram));
    memset(chip8->v, 0, sizeof(chip8->v));
    chip8->i = 0;
    chip8->pc = 0x200;
    chip8->sp = 0;
    memset(chip8->stack, 0, sizeof(chip8->stack));
};

int chip8_load_rom(Chip8 *chip8, const char *filename) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        printf("No file loaded, please make you sure you pick an existing one.");
        return -1;
    }
    fread(&chip8->ram[0x200], sizeof(uint8_t),3584, file);
    fclose(file);
    printf("Rom loaded successfully!");
    return 0;
}

uint16_t chip8_fetch(Chip8 *chip8) {
    uint16_t currentInstruct = (chip8->ram[chip8->pc]<<8) | (chip8->ram[chip8->pc+1]);
    return currentInstruct;
}

uint8_t chip8_decode(uint16_t instruct) {
    uint8_t firstNibble = (instruct & 0xF000) >> 12;

    return firstNibble;
}
