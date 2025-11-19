#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

const uint8_t CHIP8_FONTSET[80] = {
    // 0
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    // 1
    0x60, 0xE0, 0x60, 0x60, 0xF0,
    // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    // 4
    0x30, 0x50, 0x90, 0xF0, 0x10,
    // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    // 7
    0xF0, 0x10, 0x20, 0x40, 0x80,
    // 8
    0x60, 0x90, 0x60, 0x90, 0x60,
    // 9
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    // A
    0x60, 0x90, 0xF0, 0x90, 0x90,
    // B
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    // C
    0x60, 0x90, 0x80, 0x90, 0x60,
    // D
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    // E
    0xF0, 0x80, 0xE0, 0x80, 0xF0,
    // F
    0xF0, 0x80, 0xE0, 0x80, 0x80
};

void chip8_init(Chip8 *chip8) {
    memset(chip8->ram, 0, sizeof(chip8->ram));
    memset(chip8->v, 0, sizeof(chip8->v));
    chip8->i = 0;
    chip8->pc = 0x200;
    chip8->sp = 0;
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->display, 0, sizeof(chip8->display));
    memcpy(chip8->ram, CHIP8_FONTSET, sizeof(CHIP8_FONTSET));
    srand(time(NULL));
};


void chip8_cycle(Chip8 *chip8) {
    uint16_t instruct = chip8_fetch(chip8);
    uint16_t firstNibble = (instruct & 0xF000) >> 12;
    uint8_t X = 0, Y = 0, NN = 0, N = 0;
    uint16_t NNN = 0;
    short pcModified = 0;

    switch(firstNibble) {
        case 0x0:
            NN = (instruct & 0x00FF);
            switch (NN) {
                case 0xE0:
                    memset(chip8->display, 0, sizeof(chip8->display));
                    break;
                case 0xEE:
                    return_00EE(chip8);
                    pcModified = 1;
                    break;
            }
            break;
        case 0x1:
            NNN = (instruct & 0x0FFF);
            jump_to_NNN(chip8, NNN);
            pcModified = 1;
            break;
        case 0x2:
            NNN = (instruct & 0x0FFF);
            call_NNN(chip8, NNN);
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
        case 0x5:
            X = (instruct & 0x0F00) >> 8;
            Y = (instruct & 0x00F0) >> 4;

            if (chip8->v[X] == chip8->v[Y]) {
                chip8->pc += 4;
                pcModified = 1;
            }
            break;
        case 0x6:
            X = (instruct & 0x0F00) >> 8;
            NN = (instruct & 0x00FF);
            set_vX_to_NN(chip8, X, NN);
            break;
        case 0x7:
            X = (instruct & 0x0F00) >> 8;
            NN = (instruct & 0x00FF);
            increase_vX_of_NN(chip8, X, NN);
            break;
        case 0x8:
            X = (instruct & 0x0F00) >> 8;
            Y = (instruct & 0x00F0) >> 4;
            N = instruct & 0x000F;
            switch (N) {
                case 0x0:
                    chip8->v[X] = chip8->v[Y];
                    break;
                case 0x1:
                    chip8->v[X] = chip8->v[X] | chip8->v[Y];
                    break;
                case 0x2:
                    chip8->v[X] = chip8->v[X] & chip8->v[Y];
                    break;
                case 0x3:
                    chip8->v[X] = chip8->v[X] ^ chip8->v[Y];
                    break;
                case 0x4:
                    uint16_t total = chip8->v[X] + chip8->v[Y];
                    if (total > 255) {
                        chip8->v[0xF] = 1;
                    } else {
                        chip8->v[0xF] = 0;
                    }
                    chip8->v[X] = chip8->v[Y] + chip8->v[X];
                case 0x5:
                    if (chip8->v[X] >= chip8->v[Y]) {
                        chip8->v[0xF] = 1;
                    } else {
                        chip8->v[0xF] = 0;
                    }
                    chip8->v[X] = chip8->v[X] - chip8->v[Y];
                    break;
                case 0x6:
                    chip8->v[0xF] = chip8->v[X] & 0x01;
                    chip8->v[X] = chip8->v[X] >> 1;
                    break;
                case 0x7:
                    if (chip8->v[Y] >= chip8->v[X]) {
                        chip8->v[0xF] = 1;
                    } else {
                        chip8->v[0xF] = 0;
                    }
                    chip8->v[X] = chip8->v[Y] - chip8->v[X];
                    break;
                case 0x8:
                    chip8->v[0xF] = chip8->v[X] >> 7;
                    chip8->v[X] = chip8->v[X] << 1;
                    break;
            }
            break;
        case 0x9:
            X = (instruct & 0x0F00) >> 8;
            Y = (instruct & 0x00F0) >> 4;

            if (chip8->v[X] != chip8->v[Y]) {
                chip8->pc += 4;
                pcModified = 1;
            }
            break;
        case 0xA:
            NNN = (instruct & 0x0FFF);
            chip8->i = NNN;
            break;
        case 0xB:
            NNN = (instruct & 0x0FFF);
            chip8->pc = NNN + chip8->v[0];
            pcModified = 1;
            break;
        case 0xC:
            X = (instruct & 0x0F00) >> 8;
            NN = instruct & 0x00FF;

            chip8->v[X] = rand() % 256 & NN ;
            break;
        case 0xD:
            X = (instruct & 0x0F00) >> 8;
            Y = (instruct & 0x00F0) >> 4;
            N = instruct & 0x000F;

            chip8->v[0xF] = 0;
            draw_sprite(chip8, chip8->v[X], chip8->v[Y], N);
            break;
        case 0xF:
            X = (instruct & 0x0F00) >> 8;
            NN = instruct & 0x00FF;

            switch (NN) {
                case 0x1E:
                    chip8->i += chip8->v[X];
                    break;
                case 0x29:
                    chip8->i = chip8->v[X]*5;
                    break;
                case 0x33:
                    int value = chip8->v[X];
                    chip8->ram[chip8->i] = value / 100;
                    chip8->ram[chip8->i+1] = value / 10 % 10;
                    chip8->ram[chip8->i+2] = value % 10;
                    break;
                case 0x55:
                    for (int i=0; i<=X; i++) {
                        chip8->ram[chip8->i+i] = chip8->v[i];
                    }
                    break;
                case 0x65:
                    for (int i=0; i<=X; i++) {
                        chip8->v[i] = chip8->ram[chip8->i+i];
                    }
                    break;
            }
    }
    if (pcModified == 0) {
        chip8->pc += 2;
    }
}

void draw_sprite(Chip8 *chip8, uint8_t vX, uint8_t vY, uint8_t N) {
    int currentLine, currentPix;

    for (int line = 0; line < N; line++) {
        currentLine = (vY + line) % 32;

        for (int pix = 0; pix < 8; pix++) {
            currentPix = (vX + pix) % 64;
            uint8_t currentMask = 0x80 >> pix;

            if (chip8->display[currentLine][currentPix] == 1) {
                chip8->v[0xF] = 1;
            }
            chip8->display[currentLine][currentPix] ^= (chip8->ram[chip8->i+line] & currentMask) >> (7-pix);
        }
    }
}

void return_00EE (Chip8 *chip8) {
    chip8->sp--;
    chip8->pc = chip8->stack[chip8->sp];
}

void jump_to_NNN(Chip8 *chip8, uint16_t NNN) {
    chip8->pc = NNN;
}
void call_NNN(Chip8 *chip8, uint16_t NNN) {
    chip8->stack[chip8->sp] = chip8->pc+2;
    chip8->sp++;
    chip8->pc = NNN;
}
void set_vX_to_NN(Chip8 *chip8, uint8_t X, uint8_t NN) {
    chip8->v[X] = NN;
}

void increase_vX_of_NN(Chip8 *chip8, uint8_t X, uint8_t NN) {
    chip8->v[X] += NN;
}

int chip8LoadRom(Chip8 *chip8, const char *filename) {
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