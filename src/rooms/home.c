/**
 * rooms/home.c - Home room layout
 *
 * Apartment floor plan with three zones:
 * - Living Room (y=2..39): couch, bed, coffee table, TV, rug, plants, nightstand
 * - Kitchenette (x=2..15, y=41..77): counter, fridge, cat bed
 * - Workspace (x=17..47, y=41..77): bookshelf, desk+laptop, rug, exit door
 *
 * Interior walls divide zones with 3-tile doorway openings.
 */

#include "../game.h"

// --- Placement helpers ---

static void place_couch(Room *room, int sx, int sy) {
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 8; col++) {
            room->tiles[sy + row][sx + col].type = TILE_COUCH;
            room->tiles[sy + row][sx + col].variant = row * 8 + col;
        }
}

static void place_desk(Room *room, int sx, int sy) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 6; col++) {
            room->tiles[sy + row][sx + col].type = TILE_DESK;
            room->tiles[sy + row][sx + col].variant = row * 6 + col;
        }
}

static void place_laptop(Room *room, int sx, int sy) {
    for (int row = 0; row < 2; row++)
        for (int col = 0; col < 2; col++) {
            room->tiles[sy + row][sx + col].type = TILE_LAPTOP;
            room->tiles[sy + row][sx + col].variant = row * 2 + col;
        }
}

static void place_door(Room *room, int sx, int sy) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 2; col++) {
            room->tiles[sy + row][sx + col].type = TILE_DOOR;
            room->tiles[sy + row][sx + col].variant = row * 2 + col;
        }
}

static void place_tv(Room *room, int sx, int sy) {
    for (int row = 0; row < 2; row++)
        for (int col = 0; col < 6; col++) {
            room->tiles[sy + row][sx + col].type = TILE_TV;
            room->tiles[sy + row][sx + col].variant = row * 6 + col;
        }
}

static void place_plant(Room *room, int sx, int sy) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 2; col++) {
            room->tiles[sy + row][sx + col].type = TILE_PLANT;
            room->tiles[sy + row][sx + col].variant = row * 2 + col;
        }
}

// Rug uses edge-flag variant: bit0=top, bit1=bottom, bit2=left, bit3=right
static void place_rug(Room *room, int sx, int sy, int w, int h) {
    for (int row = 0; row < h; row++)
        for (int col = 0; col < w; col++) {
            int flags = 0;
            if (row == 0)     flags |= 1;  // top
            if (row == h - 1) flags |= 2;  // bottom
            if (col == 0)     flags |= 4;  // left
            if (col == w - 1) flags |= 8;  // right
            room->tiles[sy + row][sx + col].type = TILE_RUG;
            room->tiles[sy + row][sx + col].variant = flags;
        }
}

static void place_coffee_table(Room *room, int sx, int sy) {
    for (int row = 0; row < 2; row++)
        for (int col = 0; col < 4; col++) {
            room->tiles[sy + row][sx + col].type = TILE_COFFEE_TABLE;
            room->tiles[sy + row][sx + col].variant = row * 4 + col;
        }
}

static void place_bed(Room *room, int sx, int sy) {
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            room->tiles[sy + row][sx + col].type = TILE_BED;
            room->tiles[sy + row][sx + col].variant = row * 8 + col;
        }
}

static void place_nightstand(Room *room, int sx, int sy) {
    for (int row = 0; row < 2; row++)
        for (int col = 0; col < 2; col++) {
            room->tiles[sy + row][sx + col].type = TILE_NIGHTSTAND;
            room->tiles[sy + row][sx + col].variant = row * 2 + col;
        }
}

static void place_bookshelf(Room *room, int sx, int sy) {
    for (int row = 0; row < 2; row++)
        for (int col = 0; col < 12; col++) {
            room->tiles[sy + row][sx + col].type = TILE_BOOKSHELF;
            room->tiles[sy + row][sx + col].variant = row * 12 + col;
        }
}

static void place_counter(Room *room, int sx, int sy) {
    for (int row = 0; row < 2; row++)
        for (int col = 0; col < 12; col++) {
            room->tiles[sy + row][sx + col].type = TILE_COUNTER;
            room->tiles[sy + row][sx + col].variant = row * 12 + col;
        }
}

static void place_fridge(Room *room, int sx, int sy) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 2; col++) {
            room->tiles[sy + row][sx + col].type = TILE_FRIDGE;
            room->tiles[sy + row][sx + col].variant = row * 2 + col;
        }
}

static void place_catbed(Room *room, int sx, int sy) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++) {
            room->tiles[sy + row][sx + col].type = TILE_CATBED;
            room->tiles[sy + row][sx + col].variant = row * 3 + col;
        }
}

// Horizontal interior wall with a doorway gap
static void place_interior_wall_h(Room *room, int y, int x_start, int x_end,
                                   int door_x_start, int door_x_end) {
    for (int x = x_start; x <= x_end; x++) {
        if (x >= door_x_start && x <= door_x_end) continue;
        room->tiles[y][x].type = TILE_INTERIOR_WALL;
        room->tiles[y][x].variant = 0;
    }
}

// Vertical interior wall with a doorway gap
static void place_interior_wall_v(Room *room, int x, int y_start, int y_end,
                                   int door_y_start, int door_y_end) {
    for (int y = y_start; y <= y_end; y++) {
        if (y >= door_y_start && y <= door_y_end) continue;
        room->tiles[y][x].type = TILE_INTERIOR_WALL;
        room->tiles[y][x].variant = 0;
    }
}

void init_room_home(Room *room) {
    room->name = "Home";

    // Fill with floor tiles (subtle checkerboard)
    for (int y = 0; y < GRID_HEIGHT; y++)
        for (int x = 0; x < GRID_WIDTH; x++) {
            room->tiles[y][x].type = TILE_FLOOR;
            room->tiles[y][x].variant = (x + y) % 2;
        }

    // === OUTER WALLS (2 tiles thick) ===
    for (int x = 0; x < GRID_WIDTH; x++) {
        room->tiles[0][x].type = TILE_WALL;
        room->tiles[1][x].type = TILE_WALL;
        room->tiles[GRID_HEIGHT - 2][x].type = TILE_WALL;
        room->tiles[GRID_HEIGHT - 1][x].type = TILE_WALL;
    }
    for (int y = 0; y < GRID_HEIGHT; y++) {
        room->tiles[y][0].type = TILE_WALL;
        room->tiles[y][1].type = TILE_WALL;
        room->tiles[y][GRID_WIDTH - 2].type = TILE_WALL;
        room->tiles[y][GRID_WIDTH - 1].type = TILE_WALL;
    }

    // === INTERIOR WALLS ===
    // Horizontal wall at y=40, x=2..47, doorway x=20..22
    place_interior_wall_h(room, 40, 2, 47, 20, 22);
    // Vertical wall at x=16, y=41..77, doorway y=73..75
    place_interior_wall_v(room, 16, 41, 77, 73, 75);

    // === LIVING ROOM (y=2..39) ===
    place_tv(room, 20, 3);              // TV: 6×2 at (20,3)
    place_plant(room, 5, 3);            // Plant: 2×3 at (5,3)
    place_plant(room, 44, 3);           // Plant: 2×3 at (44,3)
    place_rug(room, 15, 12, 14, 12);    // Living rug: 14×12 at (15,12)
    place_coffee_table(room, 20, 24);   // Coffee table: 4×2 at (20,24)
    place_couch(room, 5, 26);           // Couch: 8×4 at (5,26)
    place_bed(room, 36, 26);            // Bed: 8×8 at (36,26)
    place_nightstand(room, 34, 28);     // Nightstand: 2×2 at (34,28)

    // === KITCHENETTE (x=2..15, y=41..77) ===
    place_counter(room, 2, 44);         // Counter: 12×2 at (2,44)
    place_fridge(room, 2, 48);          // Fridge: 2×3 at (2,48)
    place_catbed(room, 7, 58);          // Cat bed: 3×3 at (7,58)

    // === WORKSPACE (x=17..47, y=41..77) ===
    place_bookshelf(room, 22, 43);      // Bookshelf: 12×2 at (22,43)
    place_desk(room, 30, 50);           // Desk: 6×3 at (30,50)
    place_laptop(room, 32, 50);         // Laptop: 2×2 at (32,50) overlaps desk
    place_rug(room, 28, 60, 10, 6);     // Workspace rug: 10×6 at (28,60)
    place_door(room, 24, 75);           // Door: 2×3 at (24,75)
}
