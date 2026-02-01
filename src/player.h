#ifndef PLAYER_H
#define PLAYER_H

#include "libs/eadk.h"
#include "game.h"

#define DAMAGE_COOLDOWN TARGET_FPS // 1s

typedef enum {
	red_heart = 0,
	blue_heart = 1,
	green_heart = 2,
	yellow_heart = 3,
	purple_heart = 4
} player_state_t;

// max hp = 16 + 4lv
// lv = (hp - 16) / 4
typedef struct {
	player_state_t state;
	eadk_point_t pos;
	int damage_cooldown;
    int dx;
    int dy;
	int hp;
	int lv;
} player_t;

extern player_t player_stats;
extern player_t last_player_stats;

void keep_in_box();
int check_entity_collisions();
void input();


#endif