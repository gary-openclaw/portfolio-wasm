/**
 * rooms/home.c - Home room layout
 * 
 * The starting room. Couch, cats, old laptop on desk.
 * Reveals: personal life
 * 
 * Room is 50x80 tiles (400x640 pixels)
 * Layout: Door on bottom wall, couch against left wall,
 * desk with laptop in work corner (top-right area)
 */

#include "../game.h"

// Helper to place multi-tile furniture
static void place_couch(Room *room, int start_x, int start_y) {
    // Couch is 6 tiles wide, 3 tiles tall
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 6; col++) {
            room->tiles[start_y + row][start_x + col].type = TILE_COUCH;
            room->tiles[start_y + row][start_x + col].variant = row * 6 + col;
        }
    }
}

static void place_desk(Room *room, int start_x, int start_y) {
    // Desk is 4 tiles wide, 2 tiles tall
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 4; col++) {
            room->tiles[start_y + row][start_x + col].type = TILE_DESK;
            room->tiles[start_y + row][start_x + col].variant = row * 4 + col;
        }
    }
}

static void place_laptop(Room *room, int start_x, int start_y) {
    // Laptop is 2 tiles wide, 2 tiles tall
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            room->tiles[start_y + row][start_x + col].type = TILE_LAPTOP;
            room->tiles[start_y + row][start_x + col].variant = row * 2 + col;
        }
    }
}

static void place_door(Room *room, int start_x, int start_y) {
    // Door is 2 tiles wide, 3 tiles tall
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 2; col++) {
            room->tiles[start_y + row][start_x + col].type = TILE_DOOR;
            room->tiles[start_y + row][start_x + col].variant = row * 2 + col;
        }
    }
}

void init_room_home(Room *room) {
    room->name = "Home";
    
    // Fill with floor tiles (subtle checkerboard pattern)
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            room->tiles[y][x].type = TILE_FLOOR;
            room->tiles[y][x].variant = (x + y) % 2;
        }
    }
    
    // === WALLS ===
    // Top wall (2 tiles thick for visibility)
    for (int x = 0; x < GRID_WIDTH; x++) {
        room->tiles[0][x].type = TILE_WALL;
        room->tiles[1][x].type = TILE_WALL;
    }
    // Bottom wall
    for (int x = 0; x < GRID_WIDTH; x++) {
        room->tiles[GRID_HEIGHT - 2][x].type = TILE_WALL;
        room->tiles[GRID_HEIGHT - 1][x].type = TILE_WALL;
    }
    // Left wall
    for (int y = 0; y < GRID_HEIGHT; y++) {
        room->tiles[y][0].type = TILE_WALL;
        room->tiles[y][1].type = TILE_WALL;
    }
    // Right wall
    for (int y = 0; y < GRID_HEIGHT; y++) {
        room->tiles[y][GRID_WIDTH - 2].type = TILE_WALL;
        room->tiles[y][GRID_WIDTH - 1].type = TILE_WALL;
    }
    
    // === DOOR ===
    // Door on bottom wall, centered
    // Bottom wall is at y=78,79, door needs to cut through
    // Place door at y=76 so it overlaps with wall visually
    int door_x = GRID_WIDTH / 2 - 1;  // Center (24)
    place_door(room, door_x, GRID_HEIGHT - 5);  // y=75,76,77
    
    // === COUCH ===
    // Against left wall, vertically centered-ish
    // Couch is 6 wide x 3 tall
    // Place with backrest against wall (rotate concept: backrest on left)
    // Actually for top-down, let's put it facing right (backrest at top)
    // Position: x=4 (away from wall), y=35 (center of room vertically)
    place_couch(room, 4, 35);
    
    // === DESK + LAPTOP ===
    // Work corner: top-right area
    // Desk is 4 wide x 2 tall
    // Position: near top-right, but away from walls
    int desk_x = GRID_WIDTH - 10;  // x=40
    int desk_y = 10;
    place_desk(room, desk_x, desk_y);
    
    // Laptop on desk (centered on desk surface)
    // Desk is at (40,10), laptop is 2x2, center it: x=41, y=10
    place_laptop(room, desk_x + 1, desk_y);
}
