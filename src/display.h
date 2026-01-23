#ifndef DISPLAY_H
#define DISPLAY_H

#include "libs/eadk.h"

#define FONT_HEIGHT 17

void display_screen_bg();

void display_red_heart(eadk_point_t point);
void display_blue_heart(eadk_point_t point);
void display_green_heart(eadk_point_t point);
void display_yellow_heart(eadk_point_t point);
void display_purple_heart(eadk_point_t point);

void display_box(eadk_size_t size, eadk_color_t color);
// Draw box using a specific stats Y (used to compute overlap with stats area)
void display_box_at(eadk_size_t size, int stats_y, eadk_color_t color);

void display_stats();

void display_string(const char *s, eadk_point_t point, eadk_color_t fg, eadk_color_t bg, int spacing);
void display_string_transparant(const char *s, eadk_point_t point, eadk_color_t fg, int spacing);

#endif