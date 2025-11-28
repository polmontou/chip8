#include "chip8.h"
#include "render.h"
#include <unistd.h>

#define CYCLES_PER_FRAME 10

int main(int argc, char **argv) {
    //Checks if there's a given file as argument
    if (argc < 2) {
        printf("No ROM file provided\n");
        return 1;
    }
    //Checks if this file exists
    if (access(argv[1], F_OK) != 0) {
        printf("File does not exist\n");
        return 1;
    }


}

// int main(void) {
//     Chip8 chip8;
//     chip8_init(&chip8);
//
//     printf("Chip8 initialized\n");
//
//     display ctx = sdl_init();
//     SDL_Delay(300);
//     for (int i = 0; i < 16; i++) {
//         chip8_init(&chip8);
//
//         chip8.ram[0x200] = 0x61;
//         chip8.ram[0x201] = i;
//         chip8.ram[0x202] = 0x62;
//         chip8.ram[0x203] = 0x08;
//         chip8.ram[0x204] = 0x63;
//         chip8.ram[0x205] = 0x10;
//         chip8.ram[0x206] = 0xF1;
//         chip8.ram[0x207] = 0x29;
//         chip8.ram[0x208] = 0xD3;
//         chip8.ram[0x209] = 0x25;
//
//         while (chip8.ram[chip8.pc] != 0x00 || chip8.ram[chip8.pc+1] != 0x00) {
//             chip8_cycle(&chip8);
//         }
//
//         SDL_SetRenderDrawColor(ctx.renderer, OFF_R, OFF_G, OFF_B, SDL_ALPHA_OPAQUE);
//         SDL_RenderClear(ctx.renderer);
//         render_display(&ctx, &chip8);
//         SDL_RenderPresent(ctx.renderer);
//         SDL_Delay(2000);
//     }
//
//     sdl_cleanup(&ctx);
//
//     return 0;
// }


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