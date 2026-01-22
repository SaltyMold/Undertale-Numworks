#include "game.h"

// each frame render the whole screen, maybe optimise later
void update_screen(){
	display_screen_bg();
	display_heart(player.pos);
	display_box(player.box_size);
}

void is_inside_box(){
    if (player.pos.x - 8 - 2 < (MIDDLE_X - player.box_size.w / 2)) player.pos.x = MIDDLE_X - player.box_size.w / 2 + 8 + 2; // left
    if (player.pos.x + 8 + 2 > (MIDDLE_X + player.box_size.w / 2)) player.pos.x = MIDDLE_X + player.box_size.w / 2 - 8 - 2; // right
    if (player.pos.y - 8 - 2 < (MIDDLE_Y - player.box_size.h / 2)) player.pos.y = MIDDLE_Y - player.box_size.h / 2 + 8 + 2; // top
    if (player.pos.y + 8 + 2 > (MIDDLE_Y + player.box_size.h / 2)) player.pos.y = MIDDLE_Y + player.box_size.h / 2 - 8 - 2; // bottom
}

void input(){
    if (eadk_keyboard_key_down(keyboard_state, eadk_key_left)) player.dx = -1;
    if (eadk_keyboard_key_down(keyboard_state, eadk_key_right)) player.dx = 1;
    if (eadk_keyboard_key_down(keyboard_state, eadk_key_up)) player.dy = -1;
    if (eadk_keyboard_key_down(keyboard_state, eadk_key_down)) player.dy = 1;
}

void update_game(){
    // update player position
    input();
    player.pos.x += player.dx;
    player.pos.y += player.dy;
    player.dx = 0;
    player.dy = 0;
    is_inside_box();

    
}

void debug_mode(){
    char buf[64];
	snprintf(buf, sizeof(buf), "start_frame_ts_ms: %d", (int)(start_frame_ts_ms));
	eadk_display_draw_string(buf, (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);
					
	snprintf(buf, sizeof(buf), "end_frame_ts_ms: %d", (int)(end_frame_ts_ms));
	eadk_display_draw_string(buf, (eadk_point_t){0, 12}, false, eadk_color_black, eadk_color_white);
    
	snprintf(buf, sizeof(buf), "frame_duration_ms: %d", (int)(frame_duration_ms));
	eadk_display_draw_string(buf, (eadk_point_t){0, 24}, false, eadk_color_black, eadk_color_white);

	snprintf(buf, sizeof(buf), "target_fps: %d", target_fps);
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
