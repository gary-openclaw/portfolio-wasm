/**
 * room.h - Room management
 */

#ifndef ROOM_H
#define ROOM_H

#include "game.h"

// Initialize all rooms
void rooms_init(void);

// Get specific rooms
Room* room_get_home(void);

#endif // ROOM_H
