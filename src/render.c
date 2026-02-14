/**
 * render.c - Drawing primitives and rendering
 */

#include "render.h"

void render_tile(int tile_x, int tile_y, const Tile *tile) {
    int px = tile_x * TILE_SIZE;
    int py = tile_y * TILE_SIZE;
    SDL_Rect rect = {px, py, TILE_SIZE, TILE_SIZE};
    
    Color c;
    switch (tile->type) {
        case TILE_WALL:
            c = PALETTE[1];
            break;
        case TILE_OBJECT:
            c = PALETTE[2];
            break;
        case TILE_DOOR:
            c = PALETTE[3];
            break;
        case TILE_FLOOR:
        default:
            // Subtle checkerboard
            c = tile->variant ? PALETTE[0] : (Color){0x12, 0x40, 0x12};
            break;
    }
    
    SDL_SetRenderDrawColor(g_game.renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(g_game.renderer, &rect);
}

void render_room(const Room *room) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            render_tile(x, y, &room->tiles[y][x]);
        }
    }
}

void render_frame(void) {
    // Clear
    SDL_SetRenderDrawColor(g_game.renderer, PALETTE[0].r, PALETTE[0].g, PALETTE[0].b, 255);
    SDL_RenderClear(g_game.renderer);
    
    // Render current room
    if (g_game.current_room) {
        render_room(g_game.current_room);
    }
    
    SDL_RenderPresent(g_game.renderer);
}
