#include "game.h"
#include "display.h"
#include "player.h"
#include "entity.h"

//----------------------------

game_t game_stats = {
	.box_size = {250, 100},
	.stats_y = 150
};
game_t last_game_stats = {
	.box_size = {250, 100},
	.stats_y = 150
};
game_t target_game_stats = {
	.box_size = {50, 150},
	.stats_y = 220
};

//----------------------------

void update_screen(){
	display_heart(player_stats.pos, player_stats.state);

	if (last_game_stats.box_size.w != game_stats.box_size.w || last_game_stats.box_size.h != game_stats.box_size.h){ // if box size changed
		display_box_at(last_game_stats.box_size, last_game_stats.stats_y, eadk_color_black);
		display_box(game_stats.box_size, eadk_color_white);
	}

	if ((last_game_stats.stats_y != game_stats.stats_y) || (player_stats.hp != last_player_stats.hp)){ // if stats position changed or player hp changed
		eadk_display_push_rect_uniform((eadk_rect_t){0, last_game_stats.stats_y - 4, EADK_SCREEN_WIDTH, FONT_HEIGHT + 8}, eadk_color_black);
		display_stats();
	}

	if (player_stats.pos.x != last_player_stats.pos.x || player_stats.pos.y != last_player_stats.pos.y){ // if player position changed
		eadk_display_push_rect_uniform((eadk_rect_t){last_player_stats.pos.x - 8, last_player_stats.pos.y - 8, 16, 16}, eadk_color_black);
		display_red_heart(player_stats.pos);
	}
	
}

void update_game(){
	// update player position
	input();
	player_stats.pos.x += player_stats.dx;
	player_stats.pos.y += player_stats.dy;
	player_stats.dx = 0;
	player_stats.dy = 0;
	
	keep_in_box();

	// check collisions
	int damage = check_entity_collisions();

	if (player_stats.damage_cooldown > 0) {
		player_stats.damage_cooldown--;
	}

	if (damage > 0 && player_stats.damage_cooldown <= 0) {
		player_stats.hp -= damage;
		player_stats.damage_cooldown = DAMAGE_COOLDOWN;
	}

	if (player_stats.hp <= 0) game_over();

	// update box size
	if (game_stats.box_size.w < target_game_stats.box_size.w) game_stats.box_size.w += BOX_SPEED;
	if (game_stats.box_size.w > target_game_stats.box_size.w) game_stats.box_size.w -= BOX_SPEED;
	if (game_stats.box_size.h < target_game_stats.box_size.h) game_stats.box_size.h += BOX_SPEED;
	if (game_stats.box_size.h > target_game_stats.box_size.h) game_stats.box_size.h -= BOX_SPEED;
	
	// update stats position
	if (game_stats.stats_y < target_game_stats.stats_y) game_stats.stats_y += BOX_SPEED;
	if (game_stats.stats_y > target_game_stats.stats_y) game_stats.stats_y -= BOX_SPEED;

	display_entities();
}

void debug_mode(){
	char buf[64];
	snprintf(buf, sizeof(buf), "start_frame_ts_ms: %d", (int)(start_frame_ts_ms));
	eadk_display_draw_string(buf, (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);
					
	snprintf(buf, sizeof(buf), "end_frame_ts_ms: %d", (int)(end_frame_ts_ms));
	eadk_display_draw_string(buf, (eadk_point_t){0, 12}, false, eadk_color_black, eadk_color_white);
	
	snprintf(buf, sizeof(buf), "frame_duration_ms: %d", (int)(frame_duration_ms));
	eadk_display_draw_string(buf, (eadk_point_t){0, 24}, false, eadk_color_black, eadk_color_white);

	snprintf(buf, sizeof(buf), "TARGET_FPS: %d", TARGET_FPS);
	eadk_display_draw_string(buf, (eadk_point_t){0, 36}, false, eadk_color_black, eadk_color_white);

	snprintf(buf, sizeof(buf), "sleep_ms: %d", (int)(sleep_ms));
	eadk_display_draw_string(buf, (eadk_point_t){0, 48}, false, eadk_color_black, eadk_color_white);

	int fps_no_cap = frame_duration_ms ? (int)(1000 / frame_duration_ms) : 0;
	int fps_capped = (frame_duration_ms + sleep_ms) ? (int)(1000 / (frame_duration_ms + sleep_ms)) : 0;
	snprintf(buf, sizeof(buf), "fps_no_cap: %d", fps_no_cap);
	eadk_display_draw_string(buf, (eadk_point_t){0, 60}, false, eadk_color_black, eadk_color_white);

	snprintf(buf, sizeof(buf), "fps_capped: %d", fps_capped);
	eadk_display_draw_string(buf, (eadk_point_t){0, 72}, false, eadk_color_black, eadk_color_white);
}
