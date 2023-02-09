#include <SDL2/SDL_ttf.h>

#include "../config.h"

#include "structs.h"

static void display_square(SDL_Renderer *renderer, int pos_x, int pos_y, uint8_t r, uint8_t g, uint8_t b) {
    SDL_Rect rect = {0};

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);

    rect.h = SQUARE_SIZE-BORDER_SIZE;
    rect.w = SQUARE_SIZE-BORDER_SIZE;
    rect.x = (pos_x*SQUARE_SIZE)+BORDER_SIZE;
    rect.y = (pos_y*SQUARE_SIZE)+BORDER_SIZE;

    SDL_RenderFillRect(renderer, &rect);
}

void display_map(SDL_Renderer *renderer, const map_t *map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            switch (map->ptr[y][x]) {
            case BACKGROUND: display_square(renderer, x, y, 0, 0, 0); break;
            case SNAKE: display_square(renderer, x, y, 0, 255, 0); break;
            case APPLE: display_square(renderer, x, y, 0, 0, 255); break;
            case ENNEMY: display_square(renderer, x, y, 255, 0, 0); break;
            default: break;
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void display_finish(SDL_Renderer *renderer, param_t *param) {
    static SDL_Surface     *surf_msg = NULL;
    static TTF_Font        *font = NULL;
    static SDL_Texture     *text = NULL;
    static const SDL_Color color = {10, 255, 50, 255};
    static char            msg[22] = {0};
    SDL_Rect               pos;

    if (msg[0] == '\0')
        snprintf(msg, 22, "%s WON !", param->winner);
    if (font == NULL) {
        TTF_Init();

        font = TTF_OpenFont("./ressources/fonts/comicate.ttf", 80);
        if (font == NULL) {
            printf("Failed Display finish screen 1\n");
            return;
        }
    }
    if (surf_msg == NULL) {
        surf_msg = TTF_RenderText_Solid(font, msg, color); 
        if (surf_msg == NULL) {
            printf("Failed Display finish screen 2\n");
            return;   
        }
    }
    if (text == NULL) {
        text = SDL_CreateTextureFromSurface(renderer, surf_msg);
        if (text == NULL) {
            printf("Failed Display finish screen 3\n");
            return;   
        }
    }

    pos.x = WIN_WIDTH/15;
    pos.y = WIN_HEIGHT/2;
    pos.w = WIN_WIDTH - 2*(WIN_WIDTH/15);
    pos.h = 80;

    SDL_RenderCopy(renderer, text, NULL, &pos);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
}