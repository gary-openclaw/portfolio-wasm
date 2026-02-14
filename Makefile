# portfolio-wasm Makefile
# Builds C + SDL2 to WebAssembly via Emscripten

CC = emcc
SRC = src/main.c
OUT = build/index.html

# Emscripten flags
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -s USE_SDL=2 \
          -s ALLOW_MEMORY_GROWTH=1 \
          -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
          --shell-file shell.html

.PHONY: all clean serve

all: $(OUT)

$(OUT): $(SRC) shell.html
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)
	@cp CNAME build/CNAME 2>/dev/null || true
	@echo "Build complete: $(OUT)"

clean:
	rm -rf build/

serve:
	cd build && python3 -m http.server 8080

# Native build for testing (optional)
native:
	gcc -O2 -Wall -Wextra $(SRC) -o build/portfolio-native -lSDL2
