# portfolio-wasm

Kashish Grover's portfolio — rendered in C + WebAssembly.

**Live:** [wasm.kashishgrover.com](https://wasm.kashishgrover.com)

## Stack

- **Language:** C11
- **Rendering:** SDL2
- **Compiler:** Emscripten
- **Aesthetic:** Game Boy pixel art

## Build

```bash
# Prerequisites: Emscripten SDK
source ~/workspace/emsdk/emsdk_env.sh

# Build
make

# Serve locally
make serve
# Open http://localhost:8080
```

## Structure

```
src/main.c     - Main app code
shell.html     - HTML template
build/         - Output (index.html + .js + .wasm)
assets/        - Fonts, images (future)
```

## Features

- [ ] Hero section
- [ ] Links (GitHub, LinkedIn, email)
- [ ] Keyboard navigation
- [ ] Easter eggs
- [ ] Mini-games (???)

## Size

- `index.wasm`: ~620KB
- `index.js`: ~170KB  
- Total: <1MB

## License

MIT
