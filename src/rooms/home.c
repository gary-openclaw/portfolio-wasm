/**
 * rooms/home.c - Home room layout
 * 
 * The starting room. Couch, cats, old laptop.
 * Reveals: personal life
 */

#include "../game.h"

void init_room_home(Room *room) {
    room->name = "Home";
    
    // Fill with floor tiles (checkerboard pattern)
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            room->tiles[y][x].type = TILE_FLOOR;
            room->tiles[y][x].variant = (x + y) % 2;
        }
    }
    
    // Walls around the perimeter
    for (int x = 0; x < GRID_WIDTH; x++) {
        room->tiles[0][x].type = TILE_WALL;
        room->tiles[GRID_HEIGHT - 1][x].type = TILE_WALL;
    }
    for (int y = 0; y < GRID_HEIGHT; y++) {
        room->tiles[y][0].type = TILE_WALL;
        room->tiles[y][GRID_WIDTH - 1].type = TILE_WALL;
    }
    
    // Door on right wall (middle)
    room->tiles[GRID_HEIGHT / 2][GRID_WIDTH - 1].type = TILE_DOOR;
    room->tiles[GRID_HEIGHT / 2 + 1][GRID_WIDTH - 1].type = TILE_DOOR;
    
    // Couch (4x2 tiles)
    for (int dy = 0; dy < 2; dy++) {
        for (int dx = 0; dx < 4; dx++) {
            room->tiles[20 + dy][10 + dx].type = TILE_OBJECT;
        }
    }
    
    // Laptop desk (2x2)
    room->tiles[30][25].type = TILE_OBJECT;
    room->tiles[30][26].type = TILE_OBJECT;
    room->tiles[31][25].type = TILE_OBJECT;
    room->tiles[31][26].type = TILE_OBJECT;
}
