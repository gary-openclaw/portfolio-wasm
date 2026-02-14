/**
 * render.h - Drawing primitives and rendering
 */

#ifndef RENDER_H
#define RENDER_H

#include "game.h"

void render_frame(void);
void render_room(const Room *room);
void render_tile(int tile_x, int tile_y, const Tile *tile);

#endif // RENDER_H
