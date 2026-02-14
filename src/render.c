/**
 * render.c - Drawing primitives and rendering
 * 
 * Pixel art rendering for Game Boy style 8x8 tiles.
 * Each furniture piece has hand-crafted pixel patterns.
 */

#include "render.h"

// Helper to draw a single pixel
static void draw_pixel(int x, int y, Color c) {
    SDL_SetRenderDrawColor(g_game.renderer, c.r, c.g, c.b, 255);
    SDL_RenderDrawPoint(g_game.renderer, x, y);
}

// Helper to fill a tile with solid color
static void fill_tile(int px, int py, Color c) {
    SDL_Rect rect = {px, py, TILE_SIZE, TILE_SIZE};
    SDL_SetRenderDrawColor(g_game.renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(g_game.renderer, &rect);
}

// Draw floor tile with subtle pattern
static void draw_floor(int px, int py, int variant) {
    Color base = variant ? PALETTE[0] : (Color){0x12, 0x40, 0x12};
    fill_tile(px, py, base);
}

// Draw wall tile - brick pattern
static void draw_wall(int px, int py, int variant) {
    fill_tile(px, py, PALETTE[2]);  // Mid green base
    
    // Brick lines
    Color line = PALETTE[0];  // Dark
    for (int x = 0; x < TILE_SIZE; x++) {
        draw_pixel(px + x, py + 2, line);
        draw_pixel(px + x, py + 5, line);
    }
    // Vertical mortar - offset per row
    draw_pixel(px + 3, py + 0, line);
    draw_pixel(px + 3, py + 1, line);
    draw_pixel(px + 7, py + 3, line);
    draw_pixel(px + 7, py + 4, line);
    draw_pixel(px + 3, py + 6, line);
    draw_pixel(px + 3, py + 7, line);
}

// Draw door - 2 tiles wide, 3 tiles tall
// Variants: 0=top-left, 1=top-right, 2=mid-left, 3=mid-right, 4=bot-left, 5=bot-right
static void draw_door(int px, int py, int variant) {
    Color frame = PALETTE[1];   // Light green (wood frame)
    Color panel = PALETTE[2];   // Mid green (door panel)
    Color dark = PALETTE[0];    // Dark (shadow/detail)
    Color highlight = PALETTE[3]; // Highlight
    
    fill_tile(px, py, panel);
    
    switch (variant) {
        case 0: // Top-left
            // Top frame
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py, frame);
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 1, frame);
            // Left frame
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px, py + y, frame);
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 1, py + y, frame);
            break;
        case 1: // Top-right
            // Top frame
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py, frame);
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 1, frame);
            // Right frame
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 6, py + y, frame);
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 7, py + y, frame);
            break;
        case 2: // Mid-left - door handle on right tile, so just frame here
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px, py + y, frame);
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 1, py + y, frame);
            // Panel detail
            draw_pixel(px + 4, py + 2, dark);
            draw_pixel(px + 5, py + 2, dark);
            draw_pixel(px + 4, py + 5, dark);
            draw_pixel(px + 5, py + 5, dark);
            break;
        case 3: // Mid-right - has door handle
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 6, py + y, frame);
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 7, py + y, frame);
            // Door handle (knob)
            draw_pixel(px + 1, py + 3, highlight);
            draw_pixel(px + 2, py + 3, highlight);
            draw_pixel(px + 1, py + 4, dark);
            draw_pixel(px + 2, py + 4, highlight);
            // Panel detail
            draw_pixel(px + 4, py + 2, dark);
            draw_pixel(px + 5, py + 2, dark);
            draw_pixel(px + 4, py + 5, dark);
            draw_pixel(px + 5, py + 5, dark);
            break;
        case 4: // Bottom-left
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px, py + y, frame);
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 1, py + y, frame);
            // Bottom frame
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 6, frame);
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 7, frame);
            break;
        case 5: // Bottom-right
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 6, py + y, frame);
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 7, py + y, frame);
            // Bottom frame
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 6, frame);
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 7, frame);
            break;
    }
}

// Draw couch - 6 tiles wide (48px), 3 tiles tall (24px)
// Top-down 3/4 view: backrest visible at top, seat cushions, armrests on sides
// Variants: row*6 + col (0-5 = top row, 6-11 = mid row, 12-17 = bottom row)
static void draw_couch(int px, int py, int variant) {
    int col = variant % 6;
    int row = variant / 6;
    
    Color frame = PALETTE[2];    // Mid - couch frame/structure
    Color cushion = PALETTE[1];  // Light - cushion fabric
    Color shadow = PALETTE[0];   // Dark - shadows/depth
    Color highlight = PALETTE[3]; // Highlight accents
    
    fill_tile(px, py, cushion);  // Base cushion color
    
    // Top row - backrest
    if (row == 0) {
        // Back of couch (darker, recessed)
        fill_tile(px, py, frame);
        // Backrest top edge highlight
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py, highlight);
        }
        // Cushion tops visible
        for (int x = 1; x < TILE_SIZE - 1; x++) {
            draw_pixel(px + x, py + 6, cushion);
            draw_pixel(px + x, py + 7, cushion);
        }
        // Armrest ends
        if (col == 0) {  // Left armrest top
            fill_tile(px, py, frame);
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py, highlight);
            draw_pixel(px + 7, py + 4, shadow);
            draw_pixel(px + 7, py + 5, shadow);
        }
        if (col == 5) {  // Right armrest top
            fill_tile(px, py, frame);
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py, highlight);
            draw_pixel(px, py + 4, shadow);
            draw_pixel(px, py + 5, shadow);
        }
    }
    // Middle row - seat cushions
    else if (row == 1) {
        if (col == 0) {  // Left armrest
            fill_tile(px, py, frame);
            draw_pixel(px + 7, py + 0, shadow);
            draw_pixel(px + 7, py + 1, shadow);
            draw_pixel(px + 7, py + 6, shadow);
            draw_pixel(px + 7, py + 7, shadow);
        } else if (col == 5) {  // Right armrest
            fill_tile(px, py, frame);
            draw_pixel(px, py + 0, shadow);
            draw_pixel(px, py + 1, shadow);
            draw_pixel(px, py + 6, shadow);
            draw_pixel(px, py + 7, shadow);
        } else {  // Seat cushions
            fill_tile(px, py, cushion);
            // Cushion divider lines
            if (col == 2 || col == 4) {
                draw_pixel(px + 7, py + 2, shadow);
                draw_pixel(px + 7, py + 3, shadow);
                draw_pixel(px + 7, py + 4, shadow);
                draw_pixel(px + 7, py + 5, shadow);
            }
            // Cushion highlight
            draw_pixel(px + 3, py + 3, highlight);
            draw_pixel(px + 4, py + 3, highlight);
        }
    }
    // Bottom row - front edge
    else if (row == 2) {
        fill_tile(px, py, frame);
        // Front shadow
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py + 6, shadow);
            draw_pixel(px + x, py + 7, shadow);
        }
        // Cushion overhang
        if (col > 0 && col < 5) {
            for (int x = 0; x < TILE_SIZE; x++) {
                draw_pixel(px + x, py, cushion);
                draw_pixel(px + x, py + 1, cushion);
            }
        }
        // Armrest fronts
        if (col == 0 || col == 5) {
            for (int y = 0; y < 6; y++) {
                draw_pixel(px + 3, py + y, highlight);
                draw_pixel(px + 4, py + y, highlight);
            }
        }
    }
}

// Draw desk - 4 tiles wide (32px), 2 tiles tall (16px)
// Top-down view: surface with legs visible at bottom
// Variants: row*4 + col
static void draw_desk(int px, int py, int variant) {
    int col = variant % 4;
    int row = variant / 4;
    
    Color surface = PALETTE[1];  // Light - desk surface
    Color edge = PALETTE[2];     // Mid - edge/structure
    Color shadow = PALETTE[0];   // Dark - shadow/legs
    Color highlight = PALETTE[3];
    
    if (row == 0) {  // Top row - desk surface
        fill_tile(px, py, surface);
        // Top edge
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py, edge);
        }
        // Wood grain texture
        if (col == 1 || col == 2) {
            draw_pixel(px + 2, py + 3, edge);
            draw_pixel(px + 3, py + 4, edge);
            draw_pixel(px + 5, py + 5, edge);
        }
        // Side edges
        if (col == 0) {
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px, py + y, edge);
        }
        if (col == 3) {
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 7, py + y, edge);
        }
    } else {  // Bottom row - front edge and legs
        fill_tile(px, py, surface);
        // Front edge (thicker)
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py + 4, edge);
            draw_pixel(px + x, py + 5, edge);
            draw_pixel(px + x, py + 6, shadow);
            draw_pixel(px + x, py + 7, shadow);
        }
        // Legs at corners
        if (col == 0) {
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px, py + y, edge);
            draw_pixel(px + 1, py + 5, shadow);
            draw_pixel(px + 1, py + 6, shadow);
            draw_pixel(px + 1, py + 7, shadow);
        }
        if (col == 3) {
            for (int y = 0; y < TILE_SIZE; y++) draw_pixel(px + 7, py + y, edge);
            draw_pixel(px + 6, py + 5, shadow);
            draw_pixel(px + 6, py + 6, shadow);
            draw_pixel(px + 6, py + 7, shadow);
        }
    }
}

// Draw laptop - 2 tiles wide, 2 tiles tall
// Top-down: screen visible (tilted back), keyboard in front
// Variants: row*2 + col
static void draw_laptop(int px, int py, int variant) {
    int col = variant % 2;
    int row = variant / 2;
    
    Color body = PALETTE[2];     // Mid - laptop body
    Color screen = PALETTE[0];   // Dark - screen (off/dark)
    Color keys = PALETTE[0];     // Dark - keyboard
    Color highlight = PALETTE[3];
    Color frame = PALETTE[1];
    
    if (row == 0) {  // Screen (tilted back, viewed from above)
        fill_tile(px, py, body);
        // Screen bezel
        for (int x = 1; x < 7; x++) {
            for (int y = 1; y < 7; y++) {
                draw_pixel(px + x, py + y, screen);
            }
        }
        // Screen glare
        draw_pixel(px + 2, py + 2, highlight);
        draw_pixel(px + 3, py + 2, highlight);
        // Screen content hint (code lines)
        draw_pixel(px + 2, py + 4, frame);
        draw_pixel(px + 3, py + 4, frame);
        draw_pixel(px + 4, py + 4, frame);
        draw_pixel(px + 2, py + 5, frame);
        draw_pixel(px + 3, py + 5, frame);
    } else {  // Keyboard/base
        fill_tile(px, py, body);
        // Keyboard area
        for (int x = 1; x < 7; x++) {
            for (int y = 1; y < 5; y++) {
                draw_pixel(px + x, py + y, keys);
            }
        }
        // Key rows
        for (int x = 1; x < 7; x++) {
            draw_pixel(px + x, py + 1, frame);
            draw_pixel(px + x, py + 3, frame);
        }
        // Trackpad
        draw_pixel(px + 3, py + 5, frame);
        draw_pixel(px + 4, py + 5, frame);
        draw_pixel(px + 3, py + 6, frame);
        draw_pixel(px + 4, py + 6, frame);
    }
}

void render_tile(int tile_x, int tile_y, const Tile *tile) {
    int px = tile_x * TILE_SIZE;
    int py = tile_y * TILE_SIZE;
    
    switch (tile->type) {
        case TILE_FLOOR:
            draw_floor(px, py, tile->variant);
            break;
        case TILE_WALL:
            draw_wall(px, py, tile->variant);
            break;
        case TILE_DOOR:
            draw_door(px, py, tile->variant);
            break;
        case TILE_COUCH:
            draw_couch(px, py, tile->variant);
            break;
        case TILE_DESK:
            draw_desk(px, py, tile->variant);
            break;
        case TILE_LAPTOP:
            draw_laptop(px, py, tile->variant);
            break;
        default:
            fill_tile(px, py, PALETTE[2]);
            break;
    }
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
