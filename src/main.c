/**
 * portfolio-wasm
 * Kashish Grover's portfolio - C + SDL2 + WebAssembly
 * 
 * wasm.kashishgrover.com
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Colors (Genecyst / Bloodlust Software inspired)
// Late 90s DOS emulator aesthetic - blood dripping menus
#define COLOR_BG_R 0x00
#define COLOR_BG_G 0x00
#define COLOR_BG_B 0x00  // Pure black

#define COLOR_BLOOD_DARK_R 0x8B
#define COLOR_BLOOD_DARK_G 0x00
#define COLOR_BLOOD_DARK_B 0x00

#define COLOR_BLOOD_R 0xCC
#define COLOR_BLOOD_G 0x00
#define COLOR_BLOOD_B 0x00

#define COLOR_BLOOD_BRIGHT_R 0xFF
#define COLOR_BLOOD_BRIGHT_G 0x22
#define COLOR_BLOOD_BRIGHT_B 0x22

#define COLOR_GRAY_DARK_R 0x33
#define COLOR_GRAY_DARK_G 0x33
#define COLOR_GRAY_DARK_B 0x33

#define COLOR_GRAY_R 0x88
#define COLOR_GRAY_G 0x88
#define COLOR_GRAY_B 0x88

#define COLOR_WHITE_R 0xCC
#define COLOR_WHITE_G 0xCC
#define COLOR_WHITE_B 0xCC

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    int frame;
} AppState;

static AppState app = {0};

// Blood drip state - positions and speeds
#define NUM_DRIPS 12
static int drip_x[NUM_DRIPS];
static int drip_y[NUM_DRIPS];
static int drip_speed[NUM_DRIPS];
static int drip_length[NUM_DRIPS];
static bool drips_initialized = false;

void init_drips(void) {
    for (int i = 0; i < NUM_DRIPS; i++) {
        drip_x[i] = 30 + (i * (WINDOW_WIDTH - 60) / NUM_DRIPS) + (rand() % 20 - 10);
        drip_y[i] = 24 + (rand() % 40);  // Start below menu bar
        drip_speed[i] = 1 + (rand() % 3);
        drip_length[i] = 20 + (rand() % 60);
    }
    drips_initialized = true;
}

void render_frame(void) {
    if (!drips_initialized) init_drips();
    
    // Pure black background
    SDL_SetRenderDrawColor(app.renderer, COLOR_BG_R, COLOR_BG_G, COLOR_BG_B, 255);
    SDL_RenderClear(app.renderer);
    
    // DOS-style menu bar at top (gray)
    SDL_SetRenderDrawColor(app.renderer, COLOR_GRAY_DARK_R, COLOR_GRAY_DARK_G, COLOR_GRAY_DARK_B, 255);
    SDL_Rect menubar = {0, 0, WINDOW_WIDTH, 24};
    SDL_RenderFillRect(app.renderer, &menubar);
    
    // Menu bar highlight
    SDL_SetRenderDrawColor(app.renderer, COLOR_GRAY_R, COLOR_GRAY_R, COLOR_GRAY_R, 255);
    SDL_RenderDrawLine(app.renderer, 0, 0, WINDOW_WIDTH, 0);
    SDL_RenderDrawLine(app.renderer, 0, 0, 0, 23);
    
    // Blood drips from menu bar
    for (int i = 0; i < NUM_DRIPS; i++) {
        // Draw the drip (vertical line with gradient)
        for (int dy = 0; dy < drip_length[i]; dy++) {
            int y = drip_y[i] + dy;
            if (y >= 24 && y < WINDOW_HEIGHT) {
                // Gradient from bright to dark as it goes down
                float t = (float)dy / drip_length[i];
                int r = (int)(COLOR_BLOOD_BRIGHT_R * (1 - t) + COLOR_BLOOD_DARK_R * t);
                int g = (int)(COLOR_BLOOD_BRIGHT_G * (1 - t) + COLOR_BLOOD_DARK_G * t);
                int b = (int)(COLOR_BLOOD_BRIGHT_B * (1 - t) + COLOR_BLOOD_DARK_B * t);
                SDL_SetRenderDrawColor(app.renderer, r, g, b, 255);
                
                // Drip is wider at top, narrower at bottom
                int width = (dy < 5) ? 3 : (dy < 15) ? 2 : 1;
                for (int dx = -width/2; dx <= width/2; dx++) {
                    SDL_RenderDrawPoint(app.renderer, drip_x[i] + dx, y);
                }
            }
        }
        
        // Blood pool/bulge at the bottom of drip
        int pool_y = drip_y[i] + drip_length[i];
        if (pool_y < WINDOW_HEIGHT - 10) {
            SDL_SetRenderDrawColor(app.renderer, COLOR_BLOOD_DARK_R, COLOR_BLOOD_DARK_G, COLOR_BLOOD_DARK_B, 255);
            for (int px = -2; px <= 2; px++) {
                for (int py = 0; py <= 3; py++) {
                    if (abs(px) + py <= 3) {
                        SDL_RenderDrawPoint(app.renderer, drip_x[i] + px, pool_y + py);
                    }
                }
            }
        }
        
        // Animate drips
        if (app.frame % 3 == 0) {
            drip_y[i] += drip_speed[i];
            if (drip_y[i] > WINDOW_HEIGHT) {
                // Reset drip
                drip_y[i] = 24;
                drip_speed[i] = 1 + (rand() % 3);
                drip_length[i] = 20 + (rand() % 60);
            }
        }
    }
    
    // Blood stain on menu bar (source of drips)
    SDL_SetRenderDrawColor(app.renderer, COLOR_BLOOD_R, COLOR_BLOOD_G, COLOR_BLOOD_B, 255);
    for (int x = 20; x < WINDOW_WIDTH - 20; x += 3) {
        int height = 3 + (int)(2 * sin(x * 0.1));
        for (int y = 22; y < 24 + height; y++) {
            SDL_RenderDrawPoint(app.renderer, x, y);
            SDL_RenderDrawPoint(app.renderer, x+1, y);
        }
    }
    
    // Center content area - placeholder text zone
    int cx = WINDOW_WIDTH / 2;
    int cy = WINDOW_HEIGHT / 2;
    
    // Simple pulsing rectangle (placeholder for content)
    int pulse = (int)(5.0 * sin(app.frame * 0.03));
    SDL_SetRenderDrawColor(app.renderer, COLOR_GRAY_DARK_R, COLOR_GRAY_DARK_G, COLOR_GRAY_DARK_B, 255);
    SDL_Rect content_box = {cx - 150 - pulse, cy - 80 - pulse, 300 + pulse*2, 160 + pulse*2};
    SDL_RenderDrawRect(app.renderer, &content_box);
    
    // Red accent lines
    SDL_SetRenderDrawColor(app.renderer, COLOR_BLOOD_R, COLOR_BLOOD_G, COLOR_BLOOD_B, 255);
    SDL_RenderDrawLine(app.renderer, cx - 100, cy - 40, cx + 100, cy - 40);
    SDL_RenderDrawLine(app.renderer, cx - 80, cy + 40, cx + 80, cy + 40);
    
    // Corner accents (Bloodlust style)
    SDL_SetRenderDrawColor(app.renderer, COLOR_BLOOD_DARK_R, COLOR_BLOOD_DARK_G, COLOR_BLOOD_DARK_B, 255);
    // Bottom corners
    for (int i = 0; i < 20; i++) {
        SDL_RenderDrawPoint(app.renderer, 10 + i, WINDOW_HEIGHT - 30 + (i/3));
        SDL_RenderDrawPoint(app.renderer, WINDOW_WIDTH - 10 - i, WINDOW_HEIGHT - 30 + (i/3));
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
