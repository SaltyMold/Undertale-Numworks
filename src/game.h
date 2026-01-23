#ifndef GAME_H
#define GAME_H

#include "libs/eadk.h"
#include "display.h"

#define BOX_SPEED 1

typedef enum {
	red_heart = 0,
	blue_heart = 1,
	green_heart = 2,
	yellow_heart = 3,
	purple_heart = 4
} PlayerState;

// max hp = 16 + 4lv
// lv = (hp - 16) / 4
typedef struct {
	PlayerState state;
	eadk_point_t pos;
    int8_t dx;
    int8_t dy;
	uint8_t hp;
	uint8_t lv;
} Player;

typedef struct {
	eadk_size_t box_size;
	uint8_t stats_y;
} Game;

extern Player player_stats;
extern Player last_player_stats;

extern Game game_stats;
extern Game last_game_stats;
extern Game target_game_stats;

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