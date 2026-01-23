#include "display.h"
#include "assets/heart.h"
#include "assets/font.h"
#include "game.h"

//---------------------

// Map uppercase A-Z
static const struct { const bool *data; int width; } uppercase_map[26] = {
	{ &font_uppercase_a[0][0], 10 }, { &font_uppercase_b[0][0], 11 }, { &font_uppercase_c[0][0], 10 }, { &font_uppercase_d[0][0], 10 }, { &font_uppercase_e[0][0], 10 },
	{ &font_uppercase_f[0][0], 10 }, { &font_uppercase_g[0][0], 10 }, { &font_uppercase_h[0][0], 10 }, { &font_uppercase_i[0][0], 10 }, { &font_uppercase_j[0][0], 10 },
	{ &font_uppercase_k[0][0], 10 }, { &font_uppercase_l[0][0], 10 }, { &font_uppercase_m[0][0], 12 }, { &font_uppercase_n[0][0], 10 }, { &font_uppercase_o[0][0], 10 },
	{ &font_uppercase_p[0][0], 10 }, { &font_uppercase_q[0][0], 10 }, { &font_uppercase_r[0][0], 10 }, { &font_uppercase_s[0][0], 10 }, { &font_uppercase_t[0][0], 10 },
	{ &font_uppercase_u[0][0], 10 }, { &font_uppercase_v[0][0], 10 }, { &font_uppercase_w[0][0], 12 }, { &font_uppercase_x[0][0], 10 }, { &font_uppercase_y[0][0], 9 },
	{ &font_uppercase_z[0][0], 10 }
};
// Map lowercase a-z
static const struct { const bool *data; int width; } lowercase_map[26] = {
	{ &font_lowercase_a[0][0], 10 }, { &font_lowercase_b[0][0], 11 }, { &font_lowercase_c[0][0], 10 }, { &font_lowercase_d[0][0], 10 }, { &font_lowercase_e[0][0], 10 },
	{ &font_lowercase_f[0][0], 10 }, { &font_lowercase_g[0][0], 10 }, { &font_lowercase_h[0][0], 10 }, { &font_lowercase_i[0][0], 10 }, { &font_lowercase_j[0][0], 10 },
	{ &font_lowercase_k[0][0], 10 }, { &font_lowercase_l[0][0], 10 }, { &font_lowercase_m[0][0], 12 }, { &font_lowercase_n[0][0], 10 }, { &font_lowercase_o[0][0], 10 },
	{ &font_lowercase_p[0][0], 10 }, { &font_lowercase_q[0][0], 10 }, { &font_lowercase_r[0][0], 10 }, { &font_lowercase_s[0][0], 10 }, { &font_lowercase_t[0][0], 10 },
	{ &font_lowercase_u[0][0], 10 }, { &font_lowercase_v[0][0], 10 }, { &font_lowercase_w[0][0], 12 }, { &font_lowercase_x[0][0], 9 }, { &font_lowercase_y[0][0], 10 },
	{ &font_lowercase_z[0][0], 10 }
};
// Map digits 0-9
static const struct { const bool *data; int width; } digit_map[10] = {
	{ &font_zero[0][0], 10 }, { &font_one[0][0], 11 }, { &font_two[0][0], 10 }, { &font_three[0][0], 10 }, { &font_four[0][0], 10 },
	{ &font_five[0][0], 10 }, { &font_six[0][0], 10 }, { &font_seven[0][0], 10 }, { &font_eight[0][0], 10 }, { &font_nine[0][0], 10 }
};
static const bool *get_font_data(char c, int *out_width) {
	if (c >= 'A' && c <= 'Z') {
		int idx = c - 'A';
		*out_width = uppercase_map[idx].width;
		return uppercase_map[idx].data;
	}
	if (c >= 'a' && c <= 'z') {
		int idx = c - 'a';
		*out_width = lowercase_map[idx].width;
		return lowercase_map[idx].data;
	}
	if (c >= '0' && c <= '9') {
		int idx = c - '0';
		*out_width = digit_map[idx].width;
		return digit_map[idx].data;
	}

	switch (c) {
		case ' ': *out_width = 8; return NULL;
		case '.': *out_width = 4; return &font_point[0][0];
		case ':': *out_width = 4; return &font_colon[0][0];
		case ',': *out_width = 4; return &font_comma[0][0];
		case ';': *out_width = 4; return &font_semicolon[0][0];
		case '(': *out_width = 7; return &font_left_parenthesis[0][0];
		case '*': *out_width = 12; return &font_multiplication[0][0];
		case '!': *out_width = 7; return &font_exclamation[0][0];
		case '?': *out_width = 10; return &font_question[0][0];
		case '}': *out_width = 9; return &font_right_bracket[0][0];
		case '^': *out_width = 10; return &font_circumflex[0][0];
		case ')': *out_width = 8; return &font_right_parenthesis[0][0];
		case '#': *out_width = 12; return &font_hashtag[0][0];
		case '$': *out_width = 10; return &font_dollar[0][0];
		case '{': *out_width = 9; return &font_left_bracket[0][0];
		case '%': *out_width = 11; return &font_pourcent[0][0];
		case '&': *out_width = 11; return &font_ampersand[0][0];
		case '-': *out_width = 7; return &font_minus[0][0];
		case '+': *out_width = 10; return &font_plus[0][0];
		case '@': *out_width = 10; return &font_at[0][0];
		case '/': *out_width = 11; return &font_slash[0][0];
		default: *out_width = 6; return NULL;
	}
}
void display_string(const char *s, eadk_point_t point, eadk_color_t fg, eadk_color_t bg, int spacing) {
	if (!s || !*s) return;
	if (spacing < 0) spacing = 0;
	int h = FONT_HEIGHT;

	int line_idx = 0;
	int cur_x = 0;
	for (const char *p = s; ; ++p) {
		char c = *p;
		if (c == '\n' || c == '\0') {
			++line_idx;
			cur_x = 0;
			if (c == '\0') break;
			continue;
		}

		int w = 0;
		const bool *data = get_font_data(c, &w);

		if (!data || w <= 0) {
			if (w > 0) {
				eadk_display_push_rect_uniform((eadk_rect_t){(uint16_t)(point.x + cur_x), (uint16_t)(point.y + line_idx * h), (uint16_t)w, (uint16_t)h}, bg);
			}
			cur_x += w + spacing;
			continue;
		}

		size_t n = (size_t)w * (size_t)h;
		eadk_color_t *pixels = malloc(n * sizeof *pixels);
		if (!pixels) return;

		for (size_t i = 0; i < n; ++i) pixels[i] = bg;

		for (int yy = 0; yy < h; ++yy) {
			for (int xx = 0; xx < w; ++xx) {
				size_t src_idx = (size_t)yy * (size_t)w + (size_t)xx;
				if (data[src_idx]) {
					size_t dst_idx = (size_t)yy * (size_t)w + (size_t)xx;
					pixels[dst_idx] = fg;
				}
			}
		}

		eadk_display_push_rect((eadk_rect_t){(uint16_t)(point.x + cur_x), (uint16_t)(point.y + line_idx * h), (uint16_t)w, (uint16_t)h}, pixels);
		free(pixels);

		cur_x += w + spacing;
	}
}
void display_string_transparant(const char *s, eadk_point_t point, eadk_color_t fg, int spacing) {
	if (!s || !*s) return;
	if (spacing < 0) spacing = 0;
	int h = FONT_HEIGHT;

	int line_idx = 0;
	int cur_x = 0;
	for (const char *p = s; ; ++p) {
		char c = *p;
		if (c == '\n' || c == '\0') {
			++line_idx;
			cur_x = 0;
			if (c == '\0') break;
			continue;
		}

		int w = 0;
		const bool *data = get_font_data(c, &w);

		if (!data || w <= 0) {
			if (w > 0) {
			}
			cur_x += w + spacing;
			continue;
		}

		size_t n = (size_t)w * (size_t)h;
		eadk_color_t *pixels = malloc(n * sizeof *pixels);
		if (!pixels) return;

		eadk_display_pull_rect((eadk_rect_t){(uint16_t)(point.x + cur_x), (uint16_t)(point.y + line_idx * h), (uint16_t)w, (uint16_t)h}, pixels);

		for (int yy = 0; yy < h; ++yy) {
			for (int xx = 0; xx < w; ++xx) {
				size_t src_idx = (size_t)yy * (size_t)w + (size_t)xx;
				if (data[src_idx]) {
					size_t dst_idx = (size_t)yy * (size_t)w + (size_t)xx;
					pixels[dst_idx] = fg;
				}
			}
		}

		eadk_display_push_rect((eadk_rect_t){(uint16_t)(point.x + cur_x), (uint16_t)(point.y + line_idx * h), (uint16_t)w, (uint16_t)h}, pixels);
		free(pixels);

		cur_x += w + spacing;
	}
}

//---------------------

void display_screen_bg() {
	eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_black);
}

void display_red_heart(eadk_point_t point) {
	eadk_display_push_rect((eadk_rect_t){point.x-8, point.y-8, 16, 16}, red_heart_pixels);
}
void display_blue_heart(eadk_point_t point) {
	eadk_display_push_rect((eadk_rect_t){point.x-8, point.y-8, 16, 16}, blue_heart_pixels);
}
void display_green_heart(eadk_point_t point) {
	eadk_display_push_rect((eadk_rect_t){point.x-8, point.y-8, 16, 16}, green_heart_pixels);
}
void display_yellow_heart(eadk_point_t point) {
	eadk_display_push_rect((eadk_rect_t){point.x-8, point.y-8, 16, 16}, yellow_heart_pixels);
}
void display_purple_heart(eadk_point_t point) {
	eadk_display_push_rect((eadk_rect_t){point.x-8, point.y-8, 16, 16}, purple_heart_pixels);
}

void display_stats() {
	int max_hp = 16 + 4 * player_stats.lv;

	display_string("SALTY", (eadk_point_t){5, game_stats.stats_y}, eadk_color_white, eadk_color_black, 0);
	char buf[16];
	snprintf(buf, sizeof(buf), "LV%d  HP", player_stats.lv);
	display_string(buf, (eadk_point_t){62, game_stats.stats_y}, eadk_color_white, eadk_color_black, 0);

	float ratio = 0.0f;
	if (max_hp > 0) ratio = (float)player_stats.hp / (float)max_hp;
	int filled = (int)(ratio * 110.0f + 0.5f);
	if (filled < 0) filled = 0;
	if (filled > 110) filled = 110;
	for (int i = 0; i < 110; ++i) {
		eadk_color_t col = (i < filled) ? eadk_color_yellow : eadk_color_red;
		eadk_display_push_rect_uniform((eadk_rect_t){145 + i, game_stats.stats_y - 4, 1, 20}, col);
	}

	snprintf(buf, sizeof(buf), "%d/%d", player_stats.hp, max_hp);
	display_string(buf, (eadk_point_t){265, game_stats.stats_y}, eadk_color_white, eadk_color_black, 0);
}

/*
---------------------
|    -----------    |
|    |         |    |
|    |         |    |
|    -----------    |
---------------------
*/

// Draw box using a specific stats Y (so we can compute overlap for old frames)
void display_box_at(eadk_size_t size, int stats_y, eadk_color_t color) {
	// move box up if overlapping with stats
	int center_x = MIDDLE_X;
	int center_y = MIDDLE_Y;
	int half_w = (int)size.w / 2;
	int half_h = (int)size.h / 2;

	int bottom = center_y + half_h + 5;
	if (bottom >= stats_y) {
		int overlap = bottom - (stats_y - 1);
		center_y -= overlap;
	}

	eadk_rect_t top_rect = {(uint16_t)(center_x - half_w), (uint16_t)(center_y - half_h), size.w, 2};
	eadk_rect_t bottom_rect = {(uint16_t)(center_x - half_w), (uint16_t)(center_y + half_h - 2), size.w, 2};
	eadk_rect_t left_rect = {(uint16_t)(center_x - half_w), (uint16_t)(center_y - half_h), 2, size.h};
	eadk_rect_t right_rect = {(uint16_t)(center_x + half_w - 2), (uint16_t)(center_y - half_h), 2, size.h};

	eadk_display_push_rect_uniform(top_rect, color);
	eadk_display_push_rect_uniform(bottom_rect, color);
	eadk_display_push_rect_uniform(left_rect, color);
	eadk_display_push_rect_uniform(right_rect, color);
}

void display_box(eadk_size_t size, eadk_color_t color) {
	display_box_at(size, game_stats.stats_y, color);
}