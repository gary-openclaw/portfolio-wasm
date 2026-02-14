/**
 * game.h - Shared types, constants, and state
 */

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

// Window dimensions (portrait, mobile-friendly)
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 640

// Tile system
#define TILE_SIZE 8
#define GRID_WIDTH (WINDOW_WIDTH / TILE_SIZE)   // 50 tiles
#define GRID_HEIGHT (WINDOW_HEIGHT / TILE_SIZE) // 80 tiles

// ----------------------------------------------------------------------------
// Types
// ----------------------------------------------------------------------------

typedef struct { uint8_t r, g, b; } Color;

typedef enum {
    TILE_FLOOR = 0,     // Walkable
    TILE_WALL,          // Solid, blocks movement
    TILE_OBJECT,        // Interactive object (solid)
    TILE_DOOR,          // Room transition
} TileType;

typedef struct {
    TileType type;
    uint8_t variant;    // Visual variant (for texture)
} Tile;

typedef struct {
    Tile tiles[GRID_HEIGHT][GRID_WIDTH];  // 80 rows × 50 cols
    const char *name;
} Room;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    int frame;
    Room *current_room;
} GameState;

// ----------------------------------------------------------------------------
// Globals (defined in game.c)
// ----------------------------------------------------------------------------

extern GameState g_game;
extern const Color PALETTE[4];

// ----------------------------------------------------------------------------
// API
// ----------------------------------------------------------------------------

void game_init(void);
void game_shutdown(void);
void game_run(void);

#endif // GAME_H
