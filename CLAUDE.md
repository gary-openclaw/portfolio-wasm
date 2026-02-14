# CLAUDE.md — AI Instructions for portfolio-wasm

## Project

Kashish Grover's portfolio as an explorable pixel art world. C + SDL2 + WebAssembly.

**Not a resume.** Personality revealed through environment — walk around, examine objects, discover things.

## Constraints (non-negotiable)

| Property | Value | Notes |
|----------|-------|-------|
| Canvas | 400×640 px | Portrait, mobile-friendly |
| Tile size | 8×8 px | All positions tile-aligned |
| Grid | 50×80 tiles | GRID_WIDTH=50, GRID_HEIGHT=80 |
| Character | 16×16 px | 2×2 tiles |
| Colors | 4 (Game Boy palette) | See DESIGN.md |

## Verification (how to know it's right)

Every change must pass:

1. **Compiles:** `./build.sh` succeeds (uses pyenv Python 3.11)
2. **Runs:** No console errors in browser
3. **Math checks:**
   - Position (x, y) where x ∈ [0, 49], y ∈ [0, 79]
   - Element fits: x + width ≤ 50, y + height ≤ 80
   - No unintended overlaps with solid tiles

**Do not rely on screenshots for verification.** Use coordinate math.

## Process

1. Read `DESIGN.md` for current state and build order
2. Pick the next unchecked task
3. Define exact spec before implementing:
   - What function/struct to add
   - What inputs/outputs
   - What positions/sizes (in tiles)
4. Implement within constraints
5. Verify with the three checks above
6. Mark task complete in DESIGN.md
7. Commit with descriptive message

## Sub-Agent Delegation

When spawning sub-agents for implementation:

**Include in prompt:**
- Relevant code snippets (embedded, not paths — sub-agents can't read files)
- Exact constraints (sizes, positions, types)
- Expected output format

**Sub-agent returns:**
- Code to add/change
- Verification that math checks pass

**Main session:**
- Applies the code
- Runs build
- Confirms verification
- Commits

## Build

```bash
./build.sh        # Builds to build/index.html
make serve        # Serves at localhost:8080
```

Requires: Emscripten SDK at ~/workspace/emsdk, pyenv Python 3.11

## Files

| File | Purpose |
|------|---------|
| src/main.c | All C code (single file for now) |
| shell.html | HTML template with styling |
| DESIGN.md | Design constraints, room specs, build order |
| build/ | Output (deployed to wasm.kashishgrover.com) |

## Room Definitions

Each room is a 50×80 tile array. Define objects as:

```c
typedef struct {
    int x, y;           // Position in tiles
    int width, height;  // Size in tiles
    const char *text;   // Interaction text (NULL if none)
    bool solid;         // Blocks movement?
} Object;
```

Room specs go in DESIGN.md or separate room/*.md files as complexity grows.

## Style

- Game Boy aesthetic: 4 colors, pixel-perfect, no anti-aliasing
- Interactions: brief, personality not resume ("The laptop is warm. It's been running for 13 years.")
- Easter eggs welcome, but hidden

## Working Model

**Heartbeats:** Verification only
- Does it compile?
- Any runtime errors?
- What's the next task?
- Any blockers?

**Dedicated sessions:** Actual implementation
- Pick task from DESIGN.md build order
- Implement with sub-agents if complex
- Verify (compile, run, math checks)
- Commit progress

**Checkpoints:** Human review at milestones
- Room complete → Kashish looks → feedback → continue
- Aesthetic decisions need human eyes
- Ping if stuck or need a call

**This is Gary's project to own.** Kashish gives feedback, not instructions.
