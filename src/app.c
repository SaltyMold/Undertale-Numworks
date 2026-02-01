#include "app.h"
#include "libs/eadk.h"
#include "libs/storage.h"

//----------------------------

SaveData game_save;

//----------------------------

void first_menu(){
    eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);

	eadk_display_draw_string("Press home to save and quit at any moment", (eadk_point_t){0, 0}, false, eadk_color_white, eadk_color_black);
    eadk_display_draw_string("Press EXE to enable debug info", (eadk_point_t){0, 20}, false, eadk_color_white, eadk_color_black);
	eadk_display_draw_string("Press OK to continue", (eadk_point_t){0, 220}, true, eadk_color_white, eadk_color_black);
	
	eadk_timing_msleep(2000);
	while (!eadk_keyboard_key_down(eadk_keyboard_scan(), eadk_key_ok));
}


void load_data(){
    if (extapp_fileExists(SAVE_FILE)) {
        size_t file_len = 0;
        const char *data = extapp_fileRead(SAVE_FILE, &file_len);
        if (data != NULL && file_len == sizeof(SaveData)) {
            memcpy(&game_save, data, sizeof(SaveData));
            return;
        }
    }

    first_menu();
    game_save.var1 = 0;
    game_save.var2 = 0;
    game_save.var3 = 0;
    game_save.var4 = 0;
    game_save.var5 = 0;
}

void save_data() {
    if (extapp_fileExists(SAVE_FILE)) {
        extapp_fileErase(SAVE_FILE);
    }

    extapp_fileWrite(SAVE_FILE, (const char *)&game_save, sizeof(game_save));
}