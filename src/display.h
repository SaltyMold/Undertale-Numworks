#ifndef DISPLAY_H
#define DISPLAY_H

#include "libs/eadk.h"

#define MIDDLE_X 160
#define MIDDLE_Y 120

void display_screen_bg();
void display_heart(eadk_point_t point);
void display_box(int w, int h);

#endif