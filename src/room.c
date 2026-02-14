/**
 * room.c - Room management
 */

#include "room.h"

// Forward declarations for room initializers (defined in rooms/*.c)
void init_room_home(Room *room);

// Room storage
static Room home_room = {0};

void rooms_init(void) {
    init_room_home(&home_room);
}

Room* room_get_home(void) {
    return &home_room;
}
