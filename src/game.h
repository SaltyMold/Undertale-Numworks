#ifndef GAME_H
#define GAME_H

#include "libs/eadk.h"
#include "display.h"

typedef enum {
	red_heart = 0,
	blue_heart = 1,
	green_heart = 2,
	yellow_heart = 3,
	purple_heart = 4
} PlayerState;

typedef struct {
	PlayerState state;
	eadk_point_t pos;
    int8_t dx;
    int8_t dy;
	eadk_size_t box_size;
	uint8_t hp;
} Player;

extern Player player;
extern eadk_keyboard_state_t keyboard_state;
extern uint8_t target_fps;

extern uint64_t start_frame_ts_ms;
extern uint64_t end_frame_ts_ms;
extern int frame_duration_ms;
extern int sleep_ms;

void update_screen();
void update_game();
void debug_mode();

#endif