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
    TILE_DOOR,          // Room transition
    TILE_COUCH,         // Couch furniture
    TILE_DESK,          // Desk furniture
    TILE_LAPTOP,        // Laptop on desk
    TILE_BOOKSHELF,     // Bookshelf furniture
    TILE_RUG,           // Rug (non-solid)
    TILE_TV,            // Television
    TILE_COFFEE_TABLE,  // Coffee table
    TILE_COUNTER,       // Kitchen counter
    TILE_FRIDGE,        // Refrigerator
    TILE_CATBED,        // Cat bed (non-solid)
    TILE_PLANT,         // Potted plant
    TILE_BED,           // Bed furniture
    TILE_NIGHTSTAND,    // Nightstand with lamp
    TILE_INTERIOR_WALL, // Interior divider wall
} TileType;

// Variant encodes position within multi-tile object:
// 0=top-left, 1=top-right, 2=bottom-left, 3=bottom-right
// For wider objects: 4=top-mid, 5=bottom-mid, etc.

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
