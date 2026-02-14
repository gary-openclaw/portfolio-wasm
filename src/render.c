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

// Draw couch - 8 tiles wide (64px), 4 tiles tall (32px)
// Top-down 3/4 view: backrest at top, two rows of seat cushions, front edge at bottom
// Variants: row*8 + col (0-7 = row 0, 8-15 = row 1, 16-23 = row 2, 24-31 = row 3)
static void draw_couch(int px, int py, int variant) {
    int col = variant % 8;
    int row = variant / 8;

    Color frame = PALETTE[2];
    Color cushion = PALETTE[1];
    Color shadow = PALETTE[0];
    Color highlight = PALETTE[3];

    if (row == 0) {
        fill_tile(px, py, frame);
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py, highlight);
        }
        if (col == 0) {
            draw_pixel(px + 7, py + 4, shadow);
            draw_pixel(px + 7, py + 5, shadow);
            draw_pixel(px + 7, py + 6, shadow);
        } else if (col == 7) {
            draw_pixel(px, py + 4, shadow);
            draw_pixel(px, py + 5, shadow);
            draw_pixel(px, py + 6, shadow);
        } else {
            for (int x = 0; x < TILE_SIZE; x++) {
                draw_pixel(px + x, py + 6, cushion);
                draw_pixel(px + x, py + 7, cushion);
            }
            if (col == 2 || col == 4 || col == 6) {
                draw_pixel(px, py + 3, shadow);
                draw_pixel(px, py + 4, shadow);
            }
        }
    } else if (row == 1) {
        if (col == 0) {
            fill_tile(px, py, frame);
            draw_pixel(px + 7, py + 0, shadow);
            draw_pixel(px + 7, py + 1, shadow);
            draw_pixel(px + 7, py + 6, shadow);
            draw_pixel(px + 7, py + 7, shadow);
        } else if (col == 7) {
            fill_tile(px, py, frame);
            draw_pixel(px, py + 0, shadow);
            draw_pixel(px, py + 1, shadow);
            draw_pixel(px, py + 6, shadow);
            draw_pixel(px, py + 7, shadow);
        } else {
            fill_tile(px, py, cushion);
            if (col == 2 || col == 4 || col == 6) {
                for (int y2 = 1; y2 <= 6; y2++) draw_pixel(px, py + y2, shadow);
            }
            draw_pixel(px + 3, py + 3, highlight);
            draw_pixel(px + 4, py + 3, highlight);
            draw_pixel(px + 3, py + 4, highlight);
        }
    } else if (row == 2) {
        if (col == 0) {
            fill_tile(px, py, frame);
            draw_pixel(px + 7, py + 0, shadow);
            draw_pixel(px + 7, py + 1, shadow);
            draw_pixel(px + 7, py + 6, shadow);
            draw_pixel(px + 7, py + 7, shadow);
        } else if (col == 7) {
            fill_tile(px, py, frame);
            draw_pixel(px, py + 0, shadow);
            draw_pixel(px, py + 1, shadow);
            draw_pixel(px, py + 6, shadow);
            draw_pixel(px, py + 7, shadow);
        } else {
            fill_tile(px, py, cushion);
            if (col == 2 || col == 4 || col == 6) {
                for (int y2 = 1; y2 <= 6; y2++) draw_pixel(px, py + y2, shadow);
            }
            draw_pixel(px + 4, py + 2, highlight);
            draw_pixel(px + 5, py + 2, highlight);
        }
    } else if (row == 3) {
        fill_tile(px, py, frame);
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py + 6, shadow);
            draw_pixel(px + x, py + 7, shadow);
        }
        if (col == 0 || col == 7) {
            for (int y2 = 0; y2 < 6; y2++) {
                draw_pixel(px + 3, py + y2, highlight);
                draw_pixel(px + 4, py + y2, highlight);
            }
        } else {
            for (int x = 0; x < TILE_SIZE; x++) {
                draw_pixel(px + x, py, cushion);
                draw_pixel(px + x, py + 1, cushion);
            }
        }
    }
}

// Draw desk - 6 tiles wide (48px), 3 tiles tall (24px)
// Top-down 3/4 view: surface on top two rows, front edge with legs at bottom
// Variants: row*6 + col
static void draw_desk(int px, int py, int variant) {
    int col = variant % 6;
    int row = variant / 6;

    Color surface = PALETTE[1];
    Color edge = PALETTE[2];
    Color shadow = PALETTE[0];
    Color highlight = PALETTE[3];

    if (row == 0) {
        fill_tile(px, py, surface);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py, edge);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py + y2, edge);
            draw_pixel(px + 1, py + 1, highlight);
        }
        if (col == 5) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px + 7, py + y2, edge);
            draw_pixel(px + 6, py + 1, highlight);
        }
        if (col >= 1 && col <= 4) {
            draw_pixel(px + 2, py + 3, edge);
            draw_pixel(px + 3, py + 4, edge);
            draw_pixel(px + 5, py + 5, edge);
            if (col == 2 || col == 3) {
                draw_pixel(px + 6, py + 3, edge);
                draw_pixel(px + 1, py + 6, edge);
            }
        }
    } else if (row == 1) {
        fill_tile(px, py, surface);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py + y2, edge);
        }
        if (col == 5) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px + 7, py + y2, edge);
        }
        if (col >= 1 && col <= 4) {
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 5, edge);
            if (col == 2 || col == 3) {
                draw_pixel(px + 3, py + 6, shadow);
                draw_pixel(px + 4, py + 6, shadow);
            }
            draw_pixel(px + 4, py + 2, edge);
            draw_pixel(px + 5, py + 3, edge);
        }
    } else if (row == 2) {
        fill_tile(px, py, edge);
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py, surface);
            draw_pixel(px + x, py + 1, surface);
            draw_pixel(px + x, py + 6, shadow);
            draw_pixel(px + x, py + 7, shadow);
        }
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py + y2, edge);
            draw_pixel(px + 1, py + 5, shadow);
            draw_pixel(px + 1, py + 6, shadow);
            draw_pixel(px + 1, py + 7, shadow);
            draw_pixel(px, py + 6, shadow);
            draw_pixel(px, py + 7, shadow);
        }
        if (col == 5) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px + 7, py + y2, edge);
            draw_pixel(px + 6, py + 5, shadow);
            draw_pixel(px + 6, py + 6, shadow);
            draw_pixel(px + 6, py + 7, shadow);
            draw_pixel(px + 7, py + 6, shadow);
            draw_pixel(px + 7, py + 7, shadow);
        }
        if (col >= 1 && col <= 4) {
            draw_pixel(px + 3, py + 3, highlight);
            draw_pixel(px + 4, py + 3, highlight);
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

// Draw interior wall - clean horizontal line, lighter than brick
static void draw_interior_wall(int px, int py, int variant) {
    (void)variant;
    Color base = PALETTE[2];
    Color line_color = PALETTE[1];
    Color highlight = PALETTE[3];

    fill_tile(px, py, base);
    for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py, highlight);
    for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 4, line_color);
    for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py + 7, line_color);
}

// Draw rug - edge-flag encoding: bit0=top, bit1=bottom, bit2=left, bit3=right
static void draw_rug(int px, int py, int variant) {
    Color base = PALETTE[1];
    Color border = PALETTE[2];
    Color pattern = PALETTE[0];
    Color accent = PALETTE[3];

    int top    = variant & 1;
    int bottom = (variant >> 1) & 1;
    int left   = (variant >> 2) & 1;
    int right  = (variant >> 3) & 1;

    fill_tile(px, py, base);

    if (!top && !bottom && !left && !right) {
        draw_pixel(px + 3, py + 1, pattern);
        draw_pixel(px + 4, py + 1, pattern);
        draw_pixel(px + 2, py + 2, pattern);
        draw_pixel(px + 5, py + 2, pattern);
        draw_pixel(px + 2, py + 5, pattern);
        draw_pixel(px + 5, py + 5, pattern);
        draw_pixel(px + 3, py + 6, pattern);
        draw_pixel(px + 4, py + 6, pattern);
        draw_pixel(px + 3, py + 3, accent);
        draw_pixel(px + 4, py + 4, accent);
    } else {
        draw_pixel(px + 3, py + 3, pattern);
        draw_pixel(px + 4, py + 4, pattern);
        draw_pixel(px + 3, py + 4, accent);
        draw_pixel(px + 4, py + 3, accent);
    }

    if (top) {
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py, border);
            draw_pixel(px + x, py + 1, pattern);
        }
    }
    if (bottom) {
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px + x, py + 7, border);
            draw_pixel(px + x, py + 6, pattern);
        }
    }
    if (left) {
        for (int y2 = 0; y2 < TILE_SIZE; y2++) {
            draw_pixel(px, py + y2, border);
            draw_pixel(px + 1, py + y2, pattern);
        }
    }
    if (right) {
        for (int y2 = 0; y2 < TILE_SIZE; y2++) {
            draw_pixel(px + 7, py + y2, border);
            draw_pixel(px + 6, py + y2, pattern);
        }
    }

    if (top && left)     { draw_pixel(px, py, accent); }
    if (top && right)    { draw_pixel(px + 7, py, accent); }
    if (bottom && left)  { draw_pixel(px, py + 7, accent); }
    if (bottom && right) { draw_pixel(px + 7, py + 7, accent); }
}

// Draw plant - 2 tiles wide, 3 tiles tall, variant = row*2 + col
static void draw_plant(int px, int py, int variant) {
    int col = variant % 2;
    int row = variant / 2;

    Color leaf_dark = PALETTE[1];
    Color leaf_light = PALETTE[3];
    Color leaf_mid = PALETTE[2];
    Color pot = PALETTE[1];
    Color pot_rim = PALETTE[0];
    Color bg = PALETTE[0];

    if (row == 0) {
        fill_tile(px, py, bg);
        if (col == 0) {
            draw_pixel(px+4, py+0, leaf_mid); draw_pixel(px+5, py+0, leaf_light);
            draw_pixel(px+6, py+1, leaf_mid);
            draw_pixel(px+2, py+2, leaf_mid); draw_pixel(px+3, py+2, leaf_light);
            draw_pixel(px+4, py+2, leaf_mid); draw_pixel(px+5, py+2, leaf_light);
            draw_pixel(px+6, py+2, leaf_dark); draw_pixel(px+7, py+2, leaf_mid);
            draw_pixel(px+1, py+3, leaf_dark); draw_pixel(px+2, py+3, leaf_light);
            draw_pixel(px+3, py+3, leaf_mid); draw_pixel(px+4, py+3, leaf_light);
            draw_pixel(px+5, py+3, leaf_mid); draw_pixel(px+6, py+3, leaf_light);
            draw_pixel(px+7, py+3, leaf_mid);
            draw_pixel(px+1, py+4, leaf_mid); draw_pixel(px+2, py+4, leaf_light);
            draw_pixel(px+3, py+4, leaf_dark); draw_pixel(px+4, py+4, leaf_light);
            draw_pixel(px+5, py+4, leaf_mid); draw_pixel(px+6, py+4, leaf_light);
            draw_pixel(px+7, py+4, leaf_dark);
            draw_pixel(px+2, py+5, leaf_mid); draw_pixel(px+3, py+5, leaf_light);
            draw_pixel(px+4, py+5, leaf_mid); draw_pixel(px+5, py+5, leaf_light);
            draw_pixel(px+6, py+5, leaf_mid); draw_pixel(px+7, py+5, leaf_light);
            draw_pixel(px+3, py+6, leaf_dark); draw_pixel(px+4, py+6, leaf_mid);
            draw_pixel(px+5, py+6, leaf_light); draw_pixel(px+6, py+6, leaf_mid);
            draw_pixel(px+7, py+6, leaf_dark);
            draw_pixel(px+4, py+7, leaf_mid); draw_pixel(px+5, py+7, leaf_dark);
            draw_pixel(px+6, py+7, leaf_mid); draw_pixel(px+7, py+7, leaf_mid);
        } else {
            draw_pixel(px+1, py+0, leaf_mid); draw_pixel(px+2, py+0, leaf_light);
            draw_pixel(px+3, py+1, leaf_mid); draw_pixel(px+4, py+1, leaf_dark);
            draw_pixel(px+0, py+2, leaf_mid); draw_pixel(px+1, py+2, leaf_light);
            draw_pixel(px+2, py+2, leaf_mid); draw_pixel(px+3, py+2, leaf_light);
            draw_pixel(px+4, py+2, leaf_dark); draw_pixel(px+5, py+2, leaf_mid);
            draw_pixel(px+0, py+3, leaf_mid); draw_pixel(px+1, py+3, leaf_mid);
            draw_pixel(px+2, py+3, leaf_light); draw_pixel(px+3, py+3, leaf_mid);
            draw_pixel(px+4, py+3, leaf_light); draw_pixel(px+5, py+3, leaf_dark);
            draw_pixel(px+6, py+3, leaf_mid);
            draw_pixel(px+0, py+4, leaf_dark); draw_pixel(px+1, py+4, leaf_light);
            draw_pixel(px+2, py+4, leaf_mid); draw_pixel(px+3, py+4, leaf_light);
            draw_pixel(px+4, py+4, leaf_dark); draw_pixel(px+5, py+4, leaf_mid);
            draw_pixel(px+0, py+5, leaf_light); draw_pixel(px+1, py+5, leaf_mid);
            draw_pixel(px+2, py+5, leaf_light); draw_pixel(px+3, py+5, leaf_dark);
            draw_pixel(px+4, py+5, leaf_mid);
            draw_pixel(px+0, py+6, leaf_dark); draw_pixel(px+1, py+6, leaf_mid);
            draw_pixel(px+2, py+6, leaf_light); draw_pixel(px+3, py+6, leaf_mid);
            draw_pixel(px+0, py+7, leaf_mid); draw_pixel(px+1, py+7, leaf_mid);
            draw_pixel(px+2, py+7, leaf_dark);
        }
    } else if (row == 1) {
        fill_tile(px, py, bg);
        if (col == 0) {
            draw_pixel(px+3, py+0, leaf_mid); draw_pixel(px+4, py+0, leaf_light);
            draw_pixel(px+5, py+0, leaf_mid); draw_pixel(px+6, py+0, leaf_dark);
            draw_pixel(px+7, py+0, leaf_mid);
            draw_pixel(px+4, py+1, leaf_mid); draw_pixel(px+5, py+1, leaf_dark);
            draw_pixel(px+6, py+1, leaf_mid); draw_pixel(px+7, py+1, leaf_light);
            draw_pixel(px+1, py+1, leaf_mid);
            draw_pixel(px+0, py+2, leaf_light); draw_pixel(px+1, py+2, leaf_dark);
            draw_pixel(px+5, py+2, leaf_mid); draw_pixel(px+6, py+2, leaf_dark);
            draw_pixel(px+7, py+2, leaf_mid);
            draw_pixel(px+6, py+3, leaf_mid); draw_pixel(px+7, py+3, leaf_dark);
            draw_pixel(px+6, py+4, leaf_dark); draw_pixel(px+7, py+4, leaf_dark);
            draw_pixel(px+6, py+5, bg); draw_pixel(px+7, py+5, leaf_dark);
            draw_pixel(px+6, py+6, bg); draw_pixel(px+7, py+6, bg);
            draw_pixel(px+6, py+7, bg); draw_pixel(px+7, py+7, bg);
        } else {
            draw_pixel(px+0, py+0, leaf_mid); draw_pixel(px+1, py+0, leaf_dark);
            draw_pixel(px+2, py+0, leaf_light); draw_pixel(px+3, py+0, leaf_mid);
            draw_pixel(px+0, py+1, leaf_light); draw_pixel(px+1, py+1, leaf_mid);
            draw_pixel(px+2, py+1, leaf_dark); draw_pixel(px+3, py+1, leaf_light);
            draw_pixel(px+5, py+1, leaf_mid);
            draw_pixel(px+6, py+2, leaf_light); draw_pixel(px+7, py+2, leaf_mid);
            draw_pixel(px+0, py+2, leaf_mid); draw_pixel(px+1, py+2, leaf_dark);
            draw_pixel(px+0, py+3, leaf_dark); draw_pixel(px+1, py+3, leaf_mid);
            draw_pixel(px+0, py+4, leaf_dark); draw_pixel(px+1, py+4, leaf_dark);
            draw_pixel(px+0, py+5, leaf_dark); draw_pixel(px+1, py+5, bg);
            draw_pixel(px+0, py+6, bg); draw_pixel(px+1, py+6, bg);
            draw_pixel(px+0, py+7, bg); draw_pixel(px+1, py+7, bg);
        }
    } else {
        fill_tile(px, py, bg);
        if (col == 0) {
            draw_pixel(px+4, py+0, pot_rim); draw_pixel(px+5, py+0, pot_rim);
            draw_pixel(px+6, py+0, pot_rim); draw_pixel(px+7, py+0, pot_rim);
            draw_pixel(px+3, py+1, pot_rim); draw_pixel(px+4, py+1, pot);
            draw_pixel(px+5, py+1, pot); draw_pixel(px+6, py+1, pot);
            draw_pixel(px+7, py+1, pot);
            for (int y2 = 2; y2 < 7; y2++) {
                int indent = (y2 - 2) / 3;
                for (int x = 4 + indent; x < TILE_SIZE; x++)
                    draw_pixel(px + x, py + y2, pot);
                draw_pixel(px + 4 + indent, py + y2, pot_rim);
            }
            draw_pixel(px+6, py+3, leaf_mid); draw_pixel(px+6, py+4, leaf_mid);
            draw_pixel(px+5, py+7, pot_rim); draw_pixel(px+6, py+7, pot);
            draw_pixel(px+7, py+7, pot_rim);
        } else {
            draw_pixel(px+0, py+0, pot_rim); draw_pixel(px+1, py+0, pot_rim);
            draw_pixel(px+2, py+0, pot_rim); draw_pixel(px+3, py+0, pot_rim);
            draw_pixel(px+0, py+1, pot); draw_pixel(px+1, py+1, pot);
            draw_pixel(px+2, py+1, pot); draw_pixel(px+3, py+1, pot);
            draw_pixel(px+4, py+1, pot_rim);
            for (int y2 = 2; y2 < 7; y2++) {
                int indent = (y2 - 2) / 3;
                for (int x = 0; x < 4 - indent; x++)
                    draw_pixel(px + x, py + y2, pot);
                draw_pixel(px + 3 - indent, py + y2, pot_rim);
            }
            draw_pixel(px+1, py+3, leaf_mid); draw_pixel(px+1, py+4, leaf_mid);
            draw_pixel(px+0, py+7, pot_rim); draw_pixel(px+1, py+7, pot);
            draw_pixel(px+2, py+7, pot_rim);
        }
    }
}

// Draw nightstand - 2 tiles wide, 2 tiles tall, variant = row*2 + col
static void draw_nightstand(int px, int py, int variant) {
    int col = variant % 2;
    int row = variant / 2;

    Color surface = PALETTE[2];
    Color body = PALETTE[1];
    Color shadow = PALETTE[0];
    Color highlight = PALETTE[3];

    if (row == 0) {
        fill_tile(px, py, surface);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px + x, py, highlight);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py + y2, body);
            draw_pixel(px+3, py+1, highlight); draw_pixel(px+4, py+1, highlight);
            draw_pixel(px+5, py+1, highlight); draw_pixel(px+6, py+1, highlight);
            draw_pixel(px+7, py+1, highlight);
            draw_pixel(px+4, py+2, highlight); draw_pixel(px+5, py+2, surface);
            draw_pixel(px+6, py+2, highlight);
            draw_pixel(px+5, py+3, shadow);
            draw_pixel(px+4, py+4, shadow); draw_pixel(px+5, py+4, shadow);
            draw_pixel(px+6, py+4, shadow);
            draw_pixel(px+4, py+5, shadow); draw_pixel(px+5, py+5, body);
            draw_pixel(px+6, py+5, shadow);
        } else {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px+7, py+y2, body);
            draw_pixel(px+1, py+3, shadow); draw_pixel(px+2, py+3, shadow);
            draw_pixel(px+3, py+3, shadow);
            draw_pixel(px+1, py+4, body); draw_pixel(px+2, py+4, body);
            draw_pixel(px+3, py+4, shadow);
            draw_pixel(px+1, py+5, body); draw_pixel(px+2, py+5, body);
            draw_pixel(px+3, py+5, shadow);
        }
    } else {
        fill_tile(px, py, body);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py+y2, shadow);
        }
        if (col == 1) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px+7, py+y2, shadow);
        }
        for (int x = (col==0?1:0); x < (col==1?7:TILE_SIZE); x++)
            draw_pixel(px+x, py+3, shadow);
        if (col == 0) {
            draw_pixel(px+6, py+1, highlight); draw_pixel(px+7, py+1, highlight);
            draw_pixel(px+6, py+5, highlight); draw_pixel(px+7, py+5, highlight);
        } else {
            draw_pixel(px+0, py+1, highlight); draw_pixel(px+1, py+1, highlight);
            draw_pixel(px+0, py+5, highlight); draw_pixel(px+1, py+5, highlight);
        }
        for (int x = (col==0?1:0); x < (col==1?7:TILE_SIZE); x++)
            draw_pixel(px+x, py+7, shadow);
        if (col == 0) draw_pixel(px+1, py+7, surface);
        if (col == 1) draw_pixel(px+6, py+7, surface);
    }
}

// Draw coffee table - 4 tiles wide, 2 tiles tall, variant = row*4 + col
static void draw_coffee_table(int px, int py, int variant) {
    int col = variant % 4;
    int row = variant / 4;

    Color surface = PALETTE[3];
    Color wood = PALETTE[2];
    Color shadow = PALETTE[0];
    Color leg = PALETTE[1];

    if (row == 0) {
        fill_tile(px, py, surface);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, wood);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py+y2, wood);
        }
        if (col == 3) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px+7, py+y2, wood);
        }
        if (col == 1) {
            draw_pixel(px+2, py+3, wood); draw_pixel(px+3, py+4, wood);
            draw_pixel(px+6, py+5, wood);
            draw_pixel(px+5, py+2, leg); draw_pixel(px+6, py+2, leg);
            draw_pixel(px+5, py+3, leg); draw_pixel(px+6, py+3, leg);
        }
        if (col == 2) {
            draw_pixel(px+1, py+2, wood); draw_pixel(px+4, py+4, wood);
            draw_pixel(px+5, py+6, wood);
        }
    } else {
        fill_tile(px, py, shadow);
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px+x, py, surface);
            draw_pixel(px+x, py+1, wood);
        }
        if (col == 0) {
            draw_pixel(px, py, wood); draw_pixel(px, py+1, wood);
            for (int y2 = 2; y2 <= 6; y2++) draw_pixel(px+1, py+y2, leg);
            draw_pixel(px+1, py+7, wood);
        }
        if (col == 3) {
            draw_pixel(px+7, py, wood); draw_pixel(px+7, py+1, wood);
            for (int y2 = 2; y2 <= 6; y2++) draw_pixel(px+6, py+y2, leg);
            draw_pixel(px+6, py+7, wood);
        }
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py+7, shadow);
    }
}

// Draw fridge - 2 tiles wide, 3 tiles tall, variant = row*2 + col
static void draw_fridge(int px, int py, int variant) {
    int col = variant % 2;
    int row = variant / 2;

    Color body = PALETTE[2];
    Color edge_c = PALETTE[1];
    Color shadow = PALETTE[0];
    Color highlight = PALETTE[3];

    if (row == 0) {
        fill_tile(px, py, body);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, highlight);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py+y2, edge_c);
            draw_pixel(px+3, py+3, edge_c); draw_pixel(px+5, py+3, edge_c);
            draw_pixel(px+3, py+5, edge_c); draw_pixel(px+5, py+5, edge_c);
        }
        if (col == 1) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px+7, py+y2, edge_c);
            draw_pixel(px+6, py+2, highlight); draw_pixel(px+6, py+3, highlight);
            draw_pixel(px+6, py+4, highlight); draw_pixel(px+6, py+5, highlight);
            draw_pixel(px+5, py+3, shadow); draw_pixel(px+5, py+4, shadow);
        }
        for (int x = (col==0?1:0); x < (col==1?7:TILE_SIZE); x++)
            draw_pixel(px+x, py+7, shadow);
    } else if (row == 1) {
        fill_tile(px, py, body);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py+y2, edge_c);
            draw_pixel(px+3, py+2, edge_c); draw_pixel(px+4, py+2, edge_c);
            draw_pixel(px+5, py+2, edge_c);
            draw_pixel(px+3, py+3, highlight); draw_pixel(px+4, py+3, highlight);
            draw_pixel(px+5, py+3, edge_c);
        }
        if (col == 1) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px+7, py+y2, edge_c);
            for (int y2 = 1; y2 <= 6; y2++) draw_pixel(px+6, py+y2, highlight);
            for (int y2 = 2; y2 <= 5; y2++) draw_pixel(px+5, py+y2, shadow);
        }
    } else {
        fill_tile(px, py, body);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px, py+y2, edge_c);
        }
        if (col == 1) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) draw_pixel(px+7, py+y2, edge_c);
            draw_pixel(px+6, py+0, highlight); draw_pixel(px+6, py+1, highlight);
            draw_pixel(px+5, py+0, shadow);
        }
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px+x, py+6, edge_c);
            draw_pixel(px+x, py+7, shadow);
        }
        if (col == 0) {
            draw_pixel(px+1, py+7, edge_c); draw_pixel(px+2, py+7, edge_c);
        }
        if (col == 1) {
            draw_pixel(px+5, py+7, edge_c); draw_pixel(px+6, py+7, edge_c);
        }
    }
}

// Draw bookshelf - 12 tiles wide, 2 tiles tall, variant = row*12 + col
static void draw_bookshelf(int px, int py, int variant) {
    int col = variant % 12;
    int row = variant / 12;

    Color dark = PALETTE[0];
    Color mid = PALETTE[1];
    Color light = PALETTE[2];
    Color bright = PALETTE[3];

    if (row == 0) {
        fill_tile(px, py, mid);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, light);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, dark);
            }
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, light);
            draw_pixel(px+3, py+3, dark); draw_pixel(px+4, py+5, dark);
            for (int y2 = 2; y2 < 7; y2++) {
                draw_pixel(px+6, py+y2, bright); draw_pixel(px+7, py+y2, light);
            }
            return;
        }
        if (col == 11) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px+6, py+y2, dark); draw_pixel(px+7, py+y2, dark);
            }
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, light);
            draw_pixel(px+4, py+3, dark); draw_pixel(px+3, py+5, dark);
            for (int y2 = 2; y2 < 7; y2++) {
                draw_pixel(px, py+y2, light); draw_pixel(px+1, py+y2, bright);
            }
            return;
        }
        for (int y2 = 2; y2 < TILE_SIZE; y2++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                int book_id = (col * TILE_SIZE + x) % 7;
                Color spine;
                switch (book_id) {
                    case 0: case 1: spine = dark; break;
                    case 2: case 3: spine = bright; break;
                    case 4:         spine = light; break;
                    default:        spine = mid; break;
                }
                draw_pixel(px+x, py+y2, spine);
            }
        }
        draw_pixel(px+2, py+3, dark); draw_pixel(px+2, py+4, dark);
        draw_pixel(px+5, py+3, dark); draw_pixel(px+5, py+4, dark);
        if (col % 3 == 1) {
            draw_pixel(px+3, py+1, bright); draw_pixel(px+4, py+1, bright);
        }
    } else {
        fill_tile(px, py, dark);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, dark);
            }
            for (int x = 2; x < TILE_SIZE; x++)
                for (int y2 = 0; y2 < 6; y2++) draw_pixel(px+x, py+y2, mid);
            for (int x = 0; x < TILE_SIZE; x++) {
                draw_pixel(px+x, py+6, dark); draw_pixel(px+x, py+7, dark);
            }
            return;
        }
        if (col == 11) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px+6, py+y2, dark); draw_pixel(px+7, py+y2, dark);
            }
            for (int x = 0; x < 6; x++)
                for (int y2 = 0; y2 < 6; y2++) draw_pixel(px+x, py+y2, mid);
            for (int x = 0; x < TILE_SIZE; x++) {
                draw_pixel(px+x, py+6, dark); draw_pixel(px+x, py+7, dark);
            }
            return;
        }
        for (int x = 0; x < TILE_SIZE; x++) {
            int book_id = (col * TILE_SIZE + x + 3) % 6;
            Color spine;
            switch (book_id) {
                case 0:         spine = bright; break;
                case 1: case 2: spine = light; break;
                case 3:         spine = mid; break;
                default:        spine = dark; break;
            }
            for (int y2 = 0; y2 < 6; y2++) draw_pixel(px+x, py+y2, spine);
        }
        for (int x = 0; x < TILE_SIZE; x++) {
            draw_pixel(px+x, py+6, dark); draw_pixel(px+x, py+7, dark);
        }
        if (col % 2 == 0) {
            draw_pixel(px+1, py+2, bright); draw_pixel(px+1, py+3, bright);
            draw_pixel(px+6, py+1, dark); draw_pixel(px+6, py+2, dark);
        } else {
            draw_pixel(px+3, py+2, dark); draw_pixel(px+4, py+2, dark);
            draw_pixel(px+3, py+3, dark);
        }
    }
}

// Draw counter - 12 tiles wide, 2 tiles tall, variant = row*12 + col
static void draw_counter(int px, int py, int variant) {
    int col = variant % 12;
    int row = variant / 12;

    Color dark = PALETTE[0];
    Color mid = PALETTE[1];
    Color light = PALETTE[2];
    Color bright = PALETTE[3];

    if (row == 0) {
        fill_tile(px, py, light);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, mid);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py+7, mid);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px, py+y2, mid); draw_pixel(px+1, py+y2, mid);
            }
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, mid);
            for (int x = 2; x < TILE_SIZE; x++)
                for (int y2 = 1; y2 < 7; y2++) draw_pixel(px+x, py+y2, light);
            return;
        }
        if (col == 11) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px+6, py+y2, mid); draw_pixel(px+7, py+y2, mid);
            }
            for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, mid);
            for (int x = 0; x < 6; x++)
                for (int y2 = 1; y2 < 7; y2++) draw_pixel(px+x, py+y2, light);
            return;
        }
        if (col == 4) {
            draw_pixel(px+2,py+2,dark); draw_pixel(px+3,py+2,dark);
            draw_pixel(px+4,py+2,dark);
            draw_pixel(px+1,py+3,dark); draw_pixel(px+5,py+3,dark);
            draw_pixel(px+1,py+4,dark); draw_pixel(px+5,py+4,dark);
            draw_pixel(px+2,py+5,dark); draw_pixel(px+3,py+5,dark);
            draw_pixel(px+4,py+5,dark);
            draw_pixel(px+3,py+3,mid); draw_pixel(px+3,py+4,mid);
        }
        if (col == 5) {
            draw_pixel(px+3,py+2,dark); draw_pixel(px+4,py+2,dark);
            draw_pixel(px+2,py+3,dark); draw_pixel(px+5,py+3,dark);
            draw_pixel(px+2,py+4,dark); draw_pixel(px+5,py+4,dark);
            draw_pixel(px+3,py+5,dark); draw_pixel(px+4,py+5,dark);
            draw_pixel(px+3,py+3,mid); draw_pixel(px+4,py+4,mid);
        }
        if (col == 8) {
            for (int x=1; x<TILE_SIZE; x++)
                for (int y2=2; y2<6; y2++) draw_pixel(px+x, py+y2, mid);
            for (int x=1; x<TILE_SIZE; x++) draw_pixel(px+x, py+1, bright);
            for (int x=1; x<TILE_SIZE; x++) draw_pixel(px+x, py+6, bright);
            for (int y2=1; y2<7; y2++) draw_pixel(px+1, py+y2, bright);
            draw_pixel(px+4,py+3,dark); draw_pixel(px+5,py+4,dark);
        }
        if (col == 9) {
            for (int x=0; x<7; x++)
                for (int y2=2; y2<6; y2++) draw_pixel(px+x, py+y2, mid);
            for (int x=0; x<7; x++) draw_pixel(px+x, py+1, bright);
            for (int x=0; x<7; x++) draw_pixel(px+x, py+6, bright);
            for (int y2=1; y2<7; y2++) draw_pixel(px+6, py+y2, bright);
            draw_pixel(px+3,py+2,dark); draw_pixel(px+3,py+3,dark);
            draw_pixel(px+4,py+2,mid);
        }
        if (col!=4 && col!=5 && col!=8 && col!=9 && col!=0 && col!=11) {
            if (col % 3 == 0) {
                draw_pixel(px+2, py+3, bright);
                draw_pixel(px+5, py+5, bright);
            }
        }
    } else {
        fill_tile(px, py, mid);
        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, dark);
            }
            for (int x=2; x<TILE_SIZE; x++)
                for (int y2=0; y2<TILE_SIZE; y2++) draw_pixel(px+x, py+y2, mid);
            for (int x=0; x<TILE_SIZE; x++) draw_pixel(px+x, py+7, dark);
            draw_pixel(px+5, py+3, bright); draw_pixel(px+5, py+4, bright);
            return;
        }
        if (col == 11) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px+6, py+y2, dark); draw_pixel(px+7, py+y2, dark);
            }
            for (int x=0; x<TILE_SIZE; x++) draw_pixel(px+x, py+7, dark);
            draw_pixel(px+2, py+3, bright); draw_pixel(px+2, py+4, bright);
            return;
        }
        for (int x=0; x<TILE_SIZE; x++) draw_pixel(px+x, py, light);
        for (int x=0; x<TILE_SIZE; x++) draw_pixel(px+x, py+7, dark);
        if (col % 2 == 1) {
            for (int y2=0; y2<7; y2++) draw_pixel(px+7, py+y2, dark);
        }
        if (col % 2 == 0) {
            draw_pixel(px+6, py+3, bright); draw_pixel(px+6, py+4, bright);
        } else {
            draw_pixel(px+1, py+3, bright); draw_pixel(px+1, py+4, bright);
        }
        draw_pixel(px+2, py+2, dark); draw_pixel(px+5, py+2, dark);
        draw_pixel(px+2, py+5, dark); draw_pixel(px+5, py+5, dark);
    }
}

// Draw TV - 6 tiles wide, 2 tiles tall, variant = row*6 + col
static void draw_tv(int px, int py, int variant) {
    int col = variant % 6;
    int row = variant / 6;

    Color dark = PALETTE[0];
    Color mid = PALETTE[1];
    Color light = PALETTE[2];
    Color bright = PALETTE[3];

    if (row == 0) {
        fill_tile(px, py, dark);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py, mid);
        for (int x = 0; x < TILE_SIZE; x++) draw_pixel(px+x, py+7, mid);

        if (col == 0) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px, py+y2, mid); draw_pixel(px+1, py+y2, mid);
            }
            draw_pixel(px+3, py+2, mid); draw_pixel(px+4, py+2, mid);
            draw_pixel(px+3, py+3, mid);
        } else if (col == 5) {
            for (int y2 = 0; y2 < TILE_SIZE; y2++) {
                draw_pixel(px+6, py+y2, mid); draw_pixel(px+7, py+y2, mid);
            }
            draw_pixel(px+5, py+7, bright);
        } else {
            if (col == 1) {
                draw_pixel(px+0, py+2, mid); draw_pixel(px+1, py+2, mid);
                draw_pixel(px+1, py+3, mid); draw_pixel(px+2, py+3, mid);
            }
            if (col == 2 || col == 3) {
                draw_pixel(px+1, py+3, mid); draw_pixel(px+2, py+3, mid);
                draw_pixel(px+3, py+3, mid); draw_pixel(px+4, py+3, mid);
                draw_pixel(px+5, py+3, mid);
                draw_pixel(px+2, py+5, mid); draw_pixel(px+3, py+5, mid);
                draw_pixel(px+4, py+5, mid);
            }
        }
    } else {
        fill_tile(px, py, dark);
        if (col == 2 || col == 3) {
            for (int x=1; x<7; x++) {
                draw_pixel(px+x, py, mid); draw_pixel(px+x, py+1, mid);
            }
            for (int x=2; x<6; x++) {
                draw_pixel(px+x, py+2, mid); draw_pixel(px+x, py+3, mid);
            }
            for (int x=0; x<TILE_SIZE; x++) {
                draw_pixel(px+x, py+4, mid); draw_pixel(px+x, py+5, mid);
            }
            draw_pixel(px+3, py+1, light); draw_pixel(px+4, py+1, light);
        } else if (col == 1 || col == 4) {
            for (int x=0; x<TILE_SIZE; x++) draw_pixel(px+x, py, mid);
        }
    }
}

// Draw bed - 8 tiles wide, 8 tiles tall, variant = row*8 + col
static void draw_bed(int px, int py, int variant) {
    int col = variant % 8;
    int row = variant / 8;

    Color dark = PALETTE[0];
    Color mid = PALETTE[1];
    Color light = PALETTE[2];
    Color bright = PALETTE[3];

    switch (row) {
    case 0: {
        fill_tile(px, py, mid);
        for (int x=0; x<TILE_SIZE; x++) {
            draw_pixel(px+x, py, light); draw_pixel(px+x, py+1, light);
        }
        draw_pixel(px+2, py+3, dark); draw_pixel(px+2, py+4, dark); draw_pixel(px+2, py+5, dark);
        draw_pixel(px+5, py+3, dark); draw_pixel(px+5, py+4, dark); draw_pixel(px+5, py+5, dark);
        if (col == 0) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, dark); }
            draw_pixel(px, py, light); draw_pixel(px+1, py, light);
            draw_pixel(px, py+1, light); draw_pixel(px+1, py+1, light);
        }
        if (col == 7) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px+6, py+y2, dark); draw_pixel(px+7, py+y2, dark); }
            draw_pixel(px+6, py, light); draw_pixel(px+7, py, light);
            draw_pixel(px+6, py+1, light); draw_pixel(px+7, py+1, light);
        }
        break;
    }
    case 1: {
        fill_tile(px, py, bright);
        for (int x=0; x<TILE_SIZE; x++) { draw_pixel(px+x, py, light); draw_pixel(px+x, py+7, light); }
        if (col == 0) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, mid); }
            return;
        }
        if (col == 7) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px+7, py+y2, dark); draw_pixel(px+6, py+y2, mid); }
            return;
        }
        if (col == 3) { for (int y2=1; y2<7; y2++) draw_pixel(px+7, py+y2, light); }
        if (col == 4) { for (int y2=1; y2<7; y2++) draw_pixel(px, py+y2, light); }
        if (col == 2 || col == 5) {
            draw_pixel(px+3, py+3, bright); draw_pixel(px+4, py+3, bright);
            draw_pixel(px+3, py+4, bright); draw_pixel(px+4, py+4, bright);
        }
        if (col == 1 || col == 6) {
            draw_pixel(px+4, py+4, light); draw_pixel(px+5, py+5, light);
        }
        break;
    }
    case 2: case 3: case 4: case 5: {
        fill_tile(px, py, light);
        if (col == 0) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, mid); draw_pixel(px+2, py+y2, mid); }
            return;
        }
        if (col == 7) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px+7, py+y2, dark); draw_pixel(px+6, py+y2, mid); draw_pixel(px+5, py+y2, mid); }
            return;
        }
        if (row == 2) {
            for (int x=0; x<TILE_SIZE; x++) { draw_pixel(px+x, py, mid); draw_pixel(px+x, py+1, mid); }
        }
        if (row == 3 || row == 4) {
            for (int x=0; x<TILE_SIZE; x++) draw_pixel(px+x, py+4, mid);
        }
        if ((col + row) % 2 == 0) {
            draw_pixel(px+3, py+1, bright); draw_pixel(px+4, py+1, bright);
            draw_pixel(px+2, py+2, mid); draw_pixel(px+5, py+2, mid);
            draw_pixel(px+2, py+5, mid); draw_pixel(px+5, py+5, mid);
            draw_pixel(px+3, py+6, bright); draw_pixel(px+4, py+6, bright);
        } else {
            draw_pixel(px+1, py+3, mid); draw_pixel(px+6, py+3, mid);
            draw_pixel(px+3, py+3, bright); draw_pixel(px+4, py+3, bright);
        }
        break;
    }
    case 6: {
        fill_tile(px, py, light);
        if (col == 0) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, mid); }
            return;
        }
        if (col == 7) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px+7, py+y2, dark); draw_pixel(px+6, py+y2, mid); }
            return;
        }
        for (int x=0; x<TILE_SIZE; x++) {
            draw_pixel(px+x, py+3, bright);
            draw_pixel(px+x, py+4, mid); draw_pixel(px+x, py+5, mid);
            draw_pixel(px+x, py+6, mid); draw_pixel(px+x, py+7, dark);
        }
        if (col % 2 == 0) { draw_pixel(px+3, py+5, dark); draw_pixel(px+4, py+5, dark); }
        break;
    }
    case 7: {
        fill_tile(px, py, mid);
        for (int x=0; x<TILE_SIZE; x++) draw_pixel(px+x, py+2, light);
        for (int x=0; x<TILE_SIZE; x++) {
            draw_pixel(px+x, py+5, dark); draw_pixel(px+x, py+6, dark); draw_pixel(px+x, py+7, dark);
        }
        if (col == 0) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px, py+y2, dark); draw_pixel(px+1, py+y2, dark); }
            draw_pixel(px, py, light); draw_pixel(px+1, py, light);
        }
        if (col == 7) {
            for (int y2=0; y2<TILE_SIZE; y2++) { draw_pixel(px+6, py+y2, dark); draw_pixel(px+7, py+y2, dark); }
            draw_pixel(px+6, py, light); draw_pixel(px+7, py, light);
        }
        if (col > 0 && col < 7) {
            draw_pixel(px+3, py+1, dark); draw_pixel(px+5, py+3, dark);
        }
        break;
    }
    }
}

// Draw cat bed - 3 tiles wide, 3 tiles tall, variant = row*3 + col
static void draw_catbed(int px, int py, int variant) {
    int col = variant % 3;
    int row = variant / 3;

    Color dark = PALETTE[0];
    Color mid = PALETTE[1];
    Color light = PALETTE[2];
    Color bright = PALETTE[3];

    fill_tile(px, py, dark);

    if (row == 0) {
        if (col == 0) {
            draw_pixel(px+4,py+3,mid); draw_pixel(px+5,py+3,mid);
            draw_pixel(px+3,py+4,mid); draw_pixel(px+4,py+4,light);
            draw_pixel(px+5,py+4,light); draw_pixel(px+6,py+4,mid);
            draw_pixel(px+2,py+5,mid); draw_pixel(px+3,py+5,light);
            draw_pixel(px+4,py+5,bright); draw_pixel(px+5,py+5,bright);
            draw_pixel(px+6,py+5,light); draw_pixel(px+7,py+5,mid);
            draw_pixel(px+2,py+6,mid); draw_pixel(px+3,py+6,light);
            draw_pixel(px+4,py+6,bright); draw_pixel(px+5,py+6,bright);
            draw_pixel(px+6,py+6,light); draw_pixel(px+7,py+6,mid);
            draw_pixel(px+1,py+7,mid); draw_pixel(px+2,py+7,light);
            draw_pixel(px+3,py+7,light); draw_pixel(px+4,py+7,mid);
            draw_pixel(px+5,py+7,mid); draw_pixel(px+6,py+7,mid);
            draw_pixel(px+7,py+7,mid);
        } else if (col == 1) {
            for (int x=0; x<TILE_SIZE; x++) {
                draw_pixel(px+x, py+2, mid); draw_pixel(px+x, py+3, light);
                draw_pixel(px+x, py+4, bright); draw_pixel(px+x, py+5, light);
                draw_pixel(px+x, py+6, mid); draw_pixel(px+x, py+7, mid);
            }
        } else {
            draw_pixel(px+2,py+3,mid); draw_pixel(px+3,py+3,mid);
            draw_pixel(px+1,py+4,mid); draw_pixel(px+2,py+4,light);
            draw_pixel(px+3,py+4,light); draw_pixel(px+4,py+4,mid);
            draw_pixel(px+0,py+5,mid); draw_pixel(px+1,py+5,light);
            draw_pixel(px+2,py+5,bright); draw_pixel(px+3,py+5,bright);
            draw_pixel(px+4,py+5,light); draw_pixel(px+5,py+5,mid);
            draw_pixel(px+0,py+6,mid); draw_pixel(px+1,py+6,light);
            draw_pixel(px+2,py+6,bright); draw_pixel(px+3,py+6,bright);
            draw_pixel(px+4,py+6,light); draw_pixel(px+5,py+6,mid);
            draw_pixel(px+0,py+7,mid); draw_pixel(px+1,py+7,mid);
            draw_pixel(px+2,py+7,mid); draw_pixel(px+3,py+7,mid);
            draw_pixel(px+4,py+7,light); draw_pixel(px+5,py+7,light);
            draw_pixel(px+6,py+7,mid);
        }
    } else if (row == 1) {
        if (col == 0) {
            draw_pixel(px+0,py+0,mid); draw_pixel(px+1,py+0,light);
            draw_pixel(px+2,py+0,light); draw_pixel(px+3,py+0,mid);
            for (int y2=1; y2<7; y2++) {
                draw_pixel(px, py+y2, mid); draw_pixel(px+1, py+y2, light);
                draw_pixel(px+2, py+y2, mid);
            }
            draw_pixel(px+0,py+7,mid); draw_pixel(px+1,py+7,light);
            draw_pixel(px+2,py+7,light); draw_pixel(px+3,py+7,mid);
            for (int y2=0; y2<TILE_SIZE; y2++)
                for (int x=4; x<TILE_SIZE; x++) draw_pixel(px+x, py+y2, mid);
            draw_pixel(px+4,py+5,dark); draw_pixel(px+5,py+5,dark);
            draw_pixel(px+5,py+4,dark); draw_pixel(px+6,py+4,dark);
            draw_pixel(px+7,py+4,dark); draw_pixel(px+7,py+3,dark);
        } else if (col == 1) {
            fill_tile(px, py, mid);
            draw_pixel(px+1,py+0,dark); draw_pixel(px+2,py+0,dark);
            draw_pixel(px+5,py+0,dark); draw_pixel(px+6,py+0,dark);
            for (int x=1; x<=6; x++) draw_pixel(px+x, py+1, dark);
            draw_pixel(px+2,py+2,dark); draw_pixel(px+3,py+2,mid);
            draw_pixel(px+4,py+2,mid); draw_pixel(px+5,py+2,dark);
            for (int x=0; x<TILE_SIZE; x++) {
                draw_pixel(px+x, py+3, dark); draw_pixel(px+x, py+4, dark);
            }
            for (int x=0; x<=6; x++) draw_pixel(px+x, py+5, dark);
            draw_pixel(px+2,py+6,dark); draw_pixel(px+3,py+6,dark);
            draw_pixel(px+5,py+6,dark);
            draw_pixel(px+0,py+6,dark);
            draw_pixel(px+0,py+7,dark); draw_pixel(px+1,py+7,dark);
            draw_pixel(px+2,py+7,dark);
        } else {
            draw_pixel(px+4,py+0,mid); draw_pixel(px+5,py+0,light);
            draw_pixel(px+6,py+0,light); draw_pixel(px+7,py+0,mid);
            for (int y2=1; y2<7; y2++) {
                draw_pixel(px+5, py+y2, mid); draw_pixel(px+6, py+y2, light);
                draw_pixel(px+7, py+y2, mid);
            }
            draw_pixel(px+4,py+7,mid); draw_pixel(px+5,py+7,light);
            draw_pixel(px+6,py+7,light); draw_pixel(px+7,py+7,mid);
            for (int y2=0; y2<TILE_SIZE; y2++)
                for (int x=0; x<4; x++) draw_pixel(px+x, py+y2, mid);
            draw_pixel(px+0,py+3,dark); draw_pixel(px+0,py+4,dark);
            draw_pixel(px+1,py+4,dark); draw_pixel(px+0,py+5,dark);
        }
    } else {
        if (col == 0) {
            draw_pixel(px+1,py+0,mid); draw_pixel(px+2,py+0,light);
            draw_pixel(px+3,py+0,mid); draw_pixel(px+4,py+0,mid);
            draw_pixel(px+5,py+0,mid); draw_pixel(px+6,py+0,mid);
            draw_pixel(px+7,py+0,mid);
            draw_pixel(px+2,py+1,mid); draw_pixel(px+3,py+1,light);
            draw_pixel(px+4,py+1,bright); draw_pixel(px+5,py+1,bright);
            draw_pixel(px+6,py+1,light); draw_pixel(px+7,py+1,mid);
            draw_pixel(px+2,py+2,mid); draw_pixel(px+3,py+2,light);
            draw_pixel(px+4,py+2,bright); draw_pixel(px+5,py+2,bright);
            draw_pixel(px+6,py+2,light); draw_pixel(px+7,py+2,mid);
            draw_pixel(px+3,py+3,mid); draw_pixel(px+4,py+3,light);
            draw_pixel(px+5,py+3,light); draw_pixel(px+6,py+3,mid);
            draw_pixel(px+4,py+4,mid); draw_pixel(px+5,py+4,mid);
        } else if (col == 1) {
            for (int x=0; x<TILE_SIZE; x++) {
                draw_pixel(px+x, py+0, mid); draw_pixel(px+x, py+1, mid);
                draw_pixel(px+x, py+2, light); draw_pixel(px+x, py+3, bright);
                draw_pixel(px+x, py+4, light); draw_pixel(px+x, py+5, mid);
            }
        } else {
            draw_pixel(px+0,py+0,mid); draw_pixel(px+1,py+0,mid);
            draw_pixel(px+2,py+0,mid); draw_pixel(px+3,py+0,mid);
            draw_pixel(px+4,py+0,mid); draw_pixel(px+5,py+0,light);
            draw_pixel(px+6,py+0,mid);
            draw_pixel(px+0,py+1,mid); draw_pixel(px+1,py+1,light);
            draw_pixel(px+2,py+1,bright); draw_pixel(px+3,py+1,bright);
            draw_pixel(px+4,py+1,light); draw_pixel(px+5,py+1,mid);
            draw_pixel(px+0,py+2,mid); draw_pixel(px+1,py+2,light);
            draw_pixel(px+2,py+2,bright); draw_pixel(px+3,py+2,bright);
            draw_pixel(px+4,py+2,light); draw_pixel(px+5,py+2,mid);
            draw_pixel(px+1,py+3,mid); draw_pixel(px+2,py+3,light);
            draw_pixel(px+3,py+3,light); draw_pixel(px+4,py+3,mid);
            draw_pixel(px+2,py+4,mid); draw_pixel(px+3,py+4,mid);
        }
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
        case TILE_BOOKSHELF:
            draw_bookshelf(px, py, tile->variant);
            break;
        case TILE_RUG:
            draw_rug(px, py, tile->variant);
            break;
        case TILE_TV:
            draw_tv(px, py, tile->variant);
            break;
        case TILE_COFFEE_TABLE:
            draw_coffee_table(px, py, tile->variant);
            break;
        case TILE_COUNTER:
            draw_counter(px, py, tile->variant);
            break;
        case TILE_FRIDGE:
            draw_fridge(px, py, tile->variant);
            break;
        case TILE_CATBED:
            draw_catbed(px, py, tile->variant);
            break;
        case TILE_PLANT:
            draw_plant(px, py, tile->variant);
            break;
        case TILE_BED:
            draw_bed(px, py, tile->variant);
            break;
        case TILE_NIGHTSTAND:
            draw_nightstand(px, py, tile->variant);
            break;
        case TILE_INTERIOR_WALL:
            draw_interior_wall(px, py, tile->variant);
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
