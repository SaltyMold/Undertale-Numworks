#include "libs/eadk.h"
#include "libs/storage.h"
#include "file.h"

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Template";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

// Max ram : 128 KB

int main(void) {
    eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);
    eadk_display_draw_string("Press HOME to exit", (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);

    int a;
    a = add(2, 3);

    while (1) {
        if (eadk_keyboard_key_down(eadk_keyboard_scan(), eadk_key_home)) {
            break;
        }
    }

    return 0;
}

