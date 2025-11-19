#include <SDL2/SDL.h>
#include <stdio.h>
#include "chip8.h"
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

display sdl_init() {
    display display={NULL,NULL};

    if (0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init:%s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    display.window = SDL_CreateWindow("Sinan au pied veineux", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (NULL == display.window) {
        fprintf(stderr, "Erreur SDL_CreateWindow:%s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    display.renderer = SDL_CreateRenderer(display.window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == display.renderer) {
        fprintf(stderr, "Erreur SDL_CreateRenderer:%s", SDL_GetError());
        SDL_DestroyWindow(display.window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return display;
}


void render_display(display *display, Chip8 *chip8) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (chip8->display[y][x] == 1) {
                SDL_SetRenderDrawColor(display->renderer, ON_R, ON_G, ON_B, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(display->renderer, OFF_R, OFF_G, OFF_B, SDL_ALPHA_OPAQUE);
            }

            SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};

            SDL_RenderFillRect(display->renderer, &rect);
        }
    }

}

void sdl_cleanup(display *display) {
    if (display->renderer != NULL) {
        SDL_DestroyRenderer(display->renderer);
    }
    if (display->window != NULL) {
        SDL_DestroyWindow(display->window);
    }
    SDL_Quit();
}