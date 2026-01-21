#include "display.h"
#include "assets/heart.h"

void display_screen_bg() {
    eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_black);
}

void display_heart(eadk_point_t point) {
    eadk_display_push_rect((eadk_rect_t){point.x-8, point.y-8, 16, 16}, heart_pixels);
}

/*
---------------------
|    -----------    |
|    |         |    |
|    |         |    |
|    -----------    |
---------------------
*/

void display_box(int w, int h) {
    eadk_display_push_rect_uniform((eadk_rect_t){MIDDLE_X - w/2, MIDDLE_Y - h/2, w, 2}, eadk_color_white); // top
    eadk_display_push_rect_uniform((eadk_rect_t){MIDDLE_X - w/2, MIDDLE_Y + h/2 - 2, w, 2}, eadk_color_white); // bottom
    eadk_display_push_rect_uniform((eadk_rect_t){MIDDLE_X - w/2, MIDDLE_Y - h/2, 2, h}, eadk_color_white); // left
    eadk_display_push_rect_uniform((eadk_rect_t){MIDDLE_X + w/2 - 2, MIDDLE_Y - h/2, 2, h}, eadk_color_white); // right
}