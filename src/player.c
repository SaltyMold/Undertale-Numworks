#include "player.h"
#include "entity.h"

player_t player_stats = {
	.state = red_heart,
	.pos = {160, 120},
	.hp = 75,
	.lv = 19,
	.damage_cooldown = DAMAGE_COOLDOWN
};
player_t last_player_stats = {
	.state = red_heart,
	.pos = {160, 120},
	.hp = 75,
	.lv = 19,
	.damage_cooldown = DAMAGE_COOLDOWN
};

//---------------------


void keep_in_box(){
	int center_x = MIDDLE_X;
	int center_y = MIDDLE_Y;
	int half_w = (int)game_stats.box_size.w / 2;
	int half_h = (int)game_stats.box_size.h / 2;
	
	int bottom = center_y + half_h + 5;
	if (bottom >= (int)game_stats.stats_y) {
		int overlap = bottom - ((int)game_stats.stats_y - 1);
		center_y -= overlap;
	}

	// left
	if (player_stats.pos.x - 8 - 2 < (center_x - half_w)) player_stats.pos.x = center_x - half_w + 8 + 2;
	// right
	if (player_stats.pos.x + 8 + 2 > (center_x + half_w)) player_stats.pos.x = center_x + half_w - 8 - 2;
	// top
	if (player_stats.pos.y - 8 - 2 < (center_y - half_h)) player_stats.pos.y = center_y - half_h + 8 + 2;
	// bottom
	if (player_stats.pos.y + 8 + 2 > (center_y + half_h)) player_stats.pos.y = center_y + half_h - 8 - 2;
}

// returns damage if collision else 0
int check_entity_collisions() {
    for (int i = 0; i < MAX_ENTITY; ++i) {
        if (!entity_list[i].is_active) continue;
        
        if (player_stats.pos.x + 8 > entity_list[i].pos.x && player_stats.pos.x - 8 < entity_list[i].pos.x + entity_list[i].size.w && player_stats.pos.y + 8 > entity_list[i].pos.y && player_stats.pos.y - 8 < entity_list[i].pos.y + entity_list[i].size.h) {

			return entity_list[i].damage;
		}
    }
    return 0;
}

void input(){
	if (eadk_keyboard_key_down(keyboard_state, eadk_key_left)) player_stats.dx = -1;
	if (eadk_keyboard_key_down(keyboard_state, eadk_key_right)) player_stats.dx = 1;
	if (eadk_keyboard_key_down(keyboard_state, eadk_key_up)) player_stats.dy = -1;
	if (eadk_keyboard_key_down(keyboard_state, eadk_key_down)) player_stats.dy = 1;
}
