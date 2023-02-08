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