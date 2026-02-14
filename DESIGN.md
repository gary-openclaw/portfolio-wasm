# portfolio-wasm Design

## Concept

A tiny explorable world. Walk around, examine things. Personality revealed through environment, not resume bullet points.

## Canvas & Grid

| Property | Value |
|----------|-------|
| Size | 400×640 (portrait, 5:8) |
| Tile size | 8×8 pixels |
| Grid | 50×80 tiles |

## Palette

| Name | Hex | RGB | Use |
|------|-----|-----|-----|
| bg-dark | `#0F380F` | 15, 56, 15 | Background |
| fg-light | `#9BBC0F` | 155, 188, 15 | Primary elements |
| fg-mid | `#306230` | 48, 98, 48 | Secondary/shadows |
| bg-light | `#8BAC0F` | 139, 172, 15 | Highlights |

## Rooms

| Room | Contains | Reveals |
|------|----------|---------|
| Home | Couch, cats, old laptop | Personal life |
| Workspace | Terminal, code, projects | Skills, craft |
| Library | Bookshelf, podcast corner | Curiosity, skepticism |
| Server Room | Blinking machines, cables | Self-hosting, infra |
| Outside | Paths, signs, mailbox | Links (GitHub, etc.) |
| ??? | Hidden | Easter egg |

## Character

- Size: 16×16 pixels (2×2 tiles)
- Walk animation: 2-4 frames
- Speed: 2 pixels/frame (or 1 tile per 4 frames)

## Mechanics

- Movement: Arrow keys / WASD (mobile: d-pad or swipe)
- Interaction: Walk to object → prompt → press Enter/tap
- Text boxes: 1-2 lines, personality not resume
- Doors: Walk into to transition rooms

## Constraints (for deterministic building)

- All positions: tile-aligned (multiples of 8)
- Objects defined as: `{ x, y, width, height, interaction_text }`
- Rooms defined as: 50×80 tile arrays + entry points
- Collision: tile-based (solid or walkable)

## Build Order

1. [x] Canvas resize to 400×640 ✓
2. [x] Tile map data structure ✓
3. [ ] Single room rendering (Home)
4. [ ] Character sprite + movement
5. [ ] Collision detection
6. [ ] Object interaction system
7. [ ] Additional rooms
8. [ ] Room transitions
9. [ ] Polish + easter eggs

## Verification

Each step verified by:
- Compiles: `make` succeeds
- Runs: no console errors
- Math: positions within bounds
