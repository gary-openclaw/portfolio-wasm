# portfolio-wasm Makefile
# Builds C + SDL2 to WebAssembly via Emscripten

CC = emcc
SOURCES = src/main.c src/game.c src/render.c src/room.c $(wildcard src/rooms/*.c)
OUT = build/index.html

# Emscripten flags
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -s USE_SDL=2 \
          -s ALLOW_MEMORY_GROWTH=1 \
          -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
          --shell-file shell.html

.PHONY: all clean serve native

all: $(OUT)

$(OUT): $(SOURCES) shell.html
	@mkdir -p build
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUT) $(LDFLAGS)
	@cp CNAME build/CNAME 2>/dev/null || true
	@echo "Build complete: $(OUT)"

clean:
	rm -rf build/

serve:
	cd build && python3 -m http.server 8080

# Native build for testing
native:
	@mkdir -p build
	gcc -O2 -Wall -Wextra $(SOURCES) -o build/portfolio-native -lSDL2
	@echo "Native build: build/portfolio-native"
