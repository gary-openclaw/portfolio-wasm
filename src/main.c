/**
 * portfolio-wasm
 * Kashish Grover's portfolio - C + SDL2 + WebAssembly
 * 
 * wasm.kashishgrover.com
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

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

// Colors (Game Boy inspired)
#define COLOR_BG_R 0x0F
#define COLOR_BG_G 0x38
#define COLOR_BG_B 0x0F

#define COLOR_FG_R 0x9B
#define COLOR_FG_G 0xBC
#define COLOR_FG_B 0x0F

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    int frame;
} AppState;

static AppState app = {0};

void render_frame(void) {
    // Clear with Game Boy green
    SDL_SetRenderDrawColor(app.renderer, COLOR_BG_R, COLOR_BG_G, COLOR_BG_B, 255);
    SDL_RenderClear(app.renderer);
    
    // Draw a simple rectangle (placeholder)
    SDL_SetRenderDrawColor(app.renderer, COLOR_FG_R, COLOR_FG_G, COLOR_FG_B, 255);
    
    // Pulsing rectangle
    int pulse = (app.frame / 30) % 2;
    int size = 100 + (pulse * 20);
    SDL_Rect rect = {
        (WINDOW_WIDTH - size) / 2,
        (WINDOW_HEIGHT - size) / 2,
        size,
        size
    };
    SDL_RenderFillRect(app.renderer, &rect);
    
    // Border
    SDL_SetRenderDrawColor(app.renderer, COLOR_FG_R, COLOR_FG_G, COLOR_FG_B, 255);
    SDL_Rect border = {10, 10, WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20};
    SDL_RenderDrawRect(app.renderer, &border);
    
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
    
    printf("Starting main loop...\n");
    
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
