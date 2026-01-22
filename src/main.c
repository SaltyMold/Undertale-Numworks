#include "libs/eadk.h"
#include "libs/storage.h"
#include "app.h"
#include "display.h"
#include "assets/font.h"
#include <string.h>

#define SIMULATOR 1 // no syscall and storage

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Undertale";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

SaveData game_save;

eadk_keyboard_state_t state;

int target_fps = 30;

int main(void) {
	#if SIMULATOR == 0
	// save and quit with home
	// Disable On/Off and Home kernel handling
	__asm__ volatile("svc #54"); // SVC_USB_WILL_EXECUTE_DFU
	__asm__ volatile("svc #44"); // SVC_POWER_SUSPEND

	load_data();
	#endif

	display_screen_bg();
	display_heart((eadk_point_t){MIDDLE_X, MIDDLE_Y});
	display_box(100, 100);
	//display_string("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789\n.:,;(*!?}^)#${%&-+@", (eadk_point_t){0, 0}, eadk_color_white, eadk_color_black, 1);
	display_string_transparant("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789\n.:,;(*!?}^)#${%&-+@", (eadk_point_t){0, 0}, eadk_color_white, 1);

	while (1) {
		uint64_t start_frame_ts_ms = eadk_timing_millis();

		// ----------------------------

		state = eadk_keyboard_scan();

		if (eadk_keyboard_key_down(state, eadk_key_home)) {
			#if SIMULATOR == 0
			save_data();
			// Restore On/Off and Home kernel handling
			__asm__ volatile("svc #51"); // SVC_USB_DID_EXECUTE_DFU
			#endif

			return 0;
		}
		if (eadk_keyboard_key_down(state, eadk_key_sqrt)) {
			volatile int *ptr = (int *)0xFFFFFFFF;
			*ptr = 0;
		}


		
		
		// ----------------------------

		uint64_t end_frame_ts_ms = eadk_timing_millis();
		int frame_duration_ms = end_frame_ts_ms - start_frame_ts_ms;
		int sleep_ms = (1000 / target_fps) > frame_duration_ms ? (1000 / target_fps) - frame_duration_ms : 0;

		if (eadk_keyboard_key_down(state, eadk_key_exe)) {
		
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

		eadk_timing_msleep(sleep_ms);
	}

	return 0;
}

