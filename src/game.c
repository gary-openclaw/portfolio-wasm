/**
 * game.c - Game state and main loop
 */

#include "game.h"
#include "render.h"
#include "room.h"
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------

GameState g_game = {0};

const Color PALETTE[4] = {
    {0x0F, 0x38, 0x0F},  // 0: bg-dark (background)
    {0x30, 0x62, 0x30},  // 1: fg-mid (shadows/secondary)
    {0x8B, 0xAC, 0x0F},  // 2: bg-light (highlights)
    {0x9B, 0xBC, 0x0F},  // 3: fg-light (primary)
};

// ----------------------------------------------------------------------------
// Main loop
// ----------------------------------------------------------------------------

static void handle_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                g_game.running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    g_game.running = false;
                }
                break;
        }
    }
}

static void update(void) {
    // Game logic goes here
    g_game.frame++;
}

static void main_loop(void) {
    handle_input();
    update();
    render_frame();
}

// ----------------------------------------------------------------------------
// Init / Shutdown / Run
// ----------------------------------------------------------------------------

void game_init(void) {
    printf("portfolio-wasm starting...\n");
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return;
    }
    
    g_game.window = SDL_CreateWindow(
        "Kashish Grover",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!g_game.window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    
    g_game.renderer = SDL_CreateRenderer(
        g_game.window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!g_game.renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_game.window);
        SDL_Quit();
        return;
    }
    
    g_game.running = true;
    g_game.frame = 0;
    
    // Initialize rooms
    rooms_init();
    g_game.current_room = room_get_home();
    
    printf("Ready (room: %s)\n", g_game.current_room->name);
}

void game_shutdown(void) {
    SDL_DestroyRenderer(g_game.renderer);
    SDL_DestroyWindow(g_game.window);
    SDL_Quit();
}

void game_run(void) {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (g_game.running) {
        main_loop();
        SDL_Delay(16);
    }
#endif
}
