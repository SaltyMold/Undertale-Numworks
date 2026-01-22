#ifndef DISPLAY_H
#define DISPLAY_H

#include "libs/eadk.h"

#define MIDDLE_X 160
#define MIDDLE_Y 120

void display_screen_bg();
void display_heart(eadk_point_t point);
void display_box(int w, int h);

void display_string(const char *s, eadk_point_t point, eadk_color_t fg, eadk_color_t bg, int spacing);
void display_string_transparant(const char *s, eadk_point_t point, eadk_color_t fg, int spacing);

#endif