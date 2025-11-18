#include <stdio.h>
#include "Chip8.h"

void printDisplay(Chip8 *chip8);

int main() {
    Chip8 chip8;
    chip8Init(&chip8);

    // Test : Afficher le chiffre "0"
    chip8.v[2] = 1;      // Position X dans V2
    chip8.v[3] = 1;       // Position Y dans V3
    chip8.i = 0x000;      // Le "0" est au début du fontset

    // Simuler l'instruction 0xD235 (draw sprite VX=V2, VY=V3, height=5)
    drawSprite(&chip8, chip8.v[2], chip8.v[3], 5);

    // Afficher le résultat
    printDisplay(&chip8);
    printf("%d", chip8.v[0xF] == 1);

    return 0;
}

void printDisplay(Chip8 *chip8) {
    printf("\n");
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (chip8->display[y][x] == 1) {
                printf("█");  // Pixel allumé
            } else {
                printf(" ");  // Pixel éteint
            }
        }
        printf("\n");
    }
    printf("\n");
}