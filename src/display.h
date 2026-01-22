#ifndef DISPLAY_H
#define DISPLAY_H

#include "libs/eadk.h"

void display_screen_bg();
void display_heart(eadk_point_t point);
void display_box(eadk_size_t size);

void display_string(const char *s, eadk_point_t point, eadk_color_t fg, eadk_color_t bg, int spacing);
void display_string_transparant(const char *s, eadk_point_t point, eadk_color_t fg, int spacing);

#endif