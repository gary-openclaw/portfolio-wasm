/**
 * portfolio-wasm
 * Kashish Grover's portfolio - C + SDL2 + WebAssembly
 * 
 * wasm.kashishgrover.com
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Window dimensions (portrait, mobile-friendly)
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 640

// Tile system
#define TILE_SIZE 8
#define GRID_WIDTH (WINDOW_WIDTH / TILE_SIZE)   // 50 tiles
#define GRID_HEIGHT (WINDOW_HEIGHT / TILE_SIZE) // 80 tiles

// Colors (Game Boy palette)
typedef struct { uint8_t r, g, b; } Color;
static const Color PALETTE[4] = {
    {0x0F, 0x38, 0x0F},  // 0: bg-dark (background)
    {0x30, 0x62, 0x30},  // 1: fg-mid (shadows/secondary)
    {0x8B, 0xAC, 0x0F},  // 2: bg-light (highlights)
    {0x9B, 0xBC, 0x0F},  // 3: fg-light (primary)
};

// Tile types
typedef enum {
    TILE_FLOOR = 0,     // Walkable
    TILE_WALL,          // Solid, blocks movement
    TILE_OBJECT,        // Interactive object (solid)
    TILE_DOOR,          // Room transition
} TileType;

// A single tile
typedef struct {
    TileType type;
    uint8_t variant;    // Visual variant (for texture)
} Tile;

// Room definition
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
} AppState;

static AppState app = {0};
static Room home_room = {0};

// Initialize home room with basic layout
void init_home_room(void) {
    home_room.name = "Home";
    
    // Fill with floor tiles
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            home_room.tiles[y][x].type = TILE_FLOOR;
            // Checkerboard pattern for visual interest
            home_room.tiles[y][x].variant = (x + y) % 2;
        }
    }
    
    // Walls around the perimeter
    for (int x = 0; x < GRID_WIDTH; x++) {
        home_room.tiles[0][x].type = TILE_WALL;           // Top
        home_room.tiles[GRID_HEIGHT-1][x].type = TILE_WALL; // Bottom
    }
    for (int y = 0; y < GRID_HEIGHT; y++) {
        home_room.tiles[y][0].type = TILE_WALL;           // Left
        home_room.tiles[y][GRID_WIDTH-1].type = TILE_WALL; // Right
    }
    
    // A door on the right wall (middle)
    home_room.tiles[GRID_HEIGHT/2][GRID_WIDTH-1].type = TILE_DOOR;
    home_room.tiles[GRID_HEIGHT/2 + 1][GRID_WIDTH-1].type = TILE_DOOR;
    
    // Some furniture placeholder (couch - 4x2 tiles)
    for (int dy = 0; dy < 2; dy++) {
        for (int dx = 0; dx < 4; dx++) {
            home_room.tiles[20 + dy][10 + dx].type = TILE_OBJECT;
        }
    }
    
    // Laptop desk (2x2)
    home_room.tiles[30][25].type = TILE_OBJECT;
    home_room.tiles[30][26].type = TILE_OBJECT;
    home_room.tiles[31][25].type = TILE_OBJECT;
    home_room.tiles[31][26].type = TILE_OBJECT;
}

void render_tile(int tile_x, int tile_y, const Tile *tile) {
    int px = tile_x * TILE_SIZE;
    int py = tile_y * TILE_SIZE;
    SDL_Rect rect = {px, py, TILE_SIZE, TILE_SIZE};
    
    Color c;
    switch (tile->type) {
        case TILE_WALL:
            c = PALETTE[1];  // Mid green for walls
            break;
        case TILE_OBJECT:
            c = PALETTE[2];  // Light green for objects
            break;
        case TILE_DOOR:
            c = PALETTE[3];  // Bright green for doors
            break;
        case TILE_FLOOR:
        default:
            // Subtle checkerboard
            c = tile->variant ? PALETTE[0] : (Color){0x12, 0x40, 0x12};
            break;
    }
    
    SDL_SetRenderDrawColor(app.renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(app.renderer, &rect);
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
    SDL_SetRenderDrawColor(app.renderer, PALETTE[0].r, PALETTE[0].g, PALETTE[0].b, 255);
    SDL_RenderClear(app.renderer);
    
    // Render current room
    if (app.current_room) {
        render_room(app.current_room);
    }
    
    SDL_RenderPresent(app.renderer);
    app.frame++;
}

void main_loop(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app.running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    app.running = false;
                }
                // Easter egg: Konami code could go here
                break;
        }
    }
    
    render_frame();
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    printf("portfolio-wasm starting...\n");
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    
    app.window = SDL_CreateWindow(
        "Kashish Grover",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!app.window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    app.renderer = SDL_CreateRenderer(
        app.window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!app.renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return 1;
    }
    
    app.running = true;
    app.frame = 0;
    
    // Initialize rooms
    init_home_room();
    app.current_room = &home_room;
    
    printf("Starting main loop (room: %s)...\n", app.current_room->name);
    
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (app.running) {
        main_loop();
        SDL_Delay(16); // ~60fps
    }
#endif
    
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
    
    return 0;
}
