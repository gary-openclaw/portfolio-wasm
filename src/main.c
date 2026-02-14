/**
 * portfolio-wasm
 * Kashish Grover's portfolio - C + SDL2 + WebAssembly
 * 
 * wasm.kashishgrover.com
 */

#include "game.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    game_init();
    game_run();
    game_shutdown();
    
    return 0;
}
