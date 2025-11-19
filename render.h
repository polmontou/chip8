#ifndef CHIP8_RENDER_H
#define CHIP8_RENDER_H

#include "chip8.h"
#include <SDL2/SDL.h>

//Pixel éteint
#define OFF_R 245
#define OFF_G 240
#define OFF_B 230
//Pixel allumé
#define ON_R 52
#define ON_G 53
#define ON_B 65
//Screen specs
#define SCALE 15
#define WIDTH 64
#define HEIGHT 32
#define SCREEN_WIDTH (64*SCALE)
#define SCREEN_HEIGHT (32*SCALE)
#define MS_FRAME 17

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} display;

void render_display(Chip8 *chip8, display *display);
display sdl_init();
void sdl_close(display *display);

#endif //CHIP8_RENDER_H