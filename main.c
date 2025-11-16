#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chip8.h"
typedef struct {
    uint8_t ram[4096];
    uint8_t v[16];
    int16_t i;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[16];
    uint8_t display[32][64];
} Chip8;

// int main(void) {
//     Chip8 chip8;
//     chip8_init(&chip8);
//
//     // Programme principal à 0x200
//     chip8.ram[0x200] = 0x61;  // V1 = 5
//     chip8.ram[0x201] = 0x05;
//     chip8.ram[0x202] = 0x24;  // CALL 0x400
//     chip8.ram[0x203] = 0x00;
//     chip8.ram[0x204] = 0x71;  // V1 += 10 (après le retour)
//     chip8.ram[0x205] = 0x0A;
//
//     // Fonction à 0x400
//     chip8.ram[0x400] = 0x62;  // V2 = 7
//     chip8.ram[0x401] = 0x07;
//     chip8.ram[0x402] = 0x00;  // RETURN
//     chip8.ram[0x403] = 0xEE;
//
//     while (chip8.ram[chip8.pc] != 0x00 || chip8.ram[chip8.pc+1] != 0x00) {
//         uint16_t currentPc = chip8.pc;
//         chip8_cycle(&chip8);
//         //printf("Register V[%d] = %02x\n", (chip8.ram[currentPc] & 0x0F), chip8.v[chip8.ram[currentPc] & 0x0F]);
//         //printf("Next instruction : %03x\n", chip8.pc);
//         printf("Register V[1] = %02x // V[2] = %02x\n", chip8.v[1], chip8.v[2]);
//         printf("Le PC pour la prochaine boucle sera  : %03x\n", chip8.pc);
//     }
//     return 0;
// }
