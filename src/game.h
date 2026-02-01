#ifndef GAME_H
#define GAME_H

#include "libs/eadk.h"

#define TARGET_FPS 120
#define BOX_SPEED 1

typedef struct {
	eadk_size_t box_size;
	uint8_t stats_y;
} game_t;

extern game_t game_stats;
extern game_t last_game_stats;
extern game_t target_game_stats;

extern eadk_keyboard_state_t keyboard_state;

extern uint64_t start_frame_ts_ms;
extern uint64_t end_frame_ts_ms;
extern int frame_duration_ms;
extern int sleep_ms;

void update_screen();
void update_game();
void debug_mode();

#endif