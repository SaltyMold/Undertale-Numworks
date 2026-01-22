#include "libs/eadk.h"
#include "libs/storage.h"
#include "app.h"
#include "display.h"
#include "assets/font.h"
#include "game.h"
#include "utils.h"
#include <string.h>

#define SIMULATOR 1 // no syscall and storage

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Undertale";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

// app.c
SaveData game_save;

// game.c
Player player = {
	.state = red_heart,
	.pos = {160, 120},
	.box_size = {100, 100}
};

eadk_keyboard_state_t keyboard_state;

uint8_t target_fps = 120;

uint64_t start_frame_ts_ms;
uint64_t end_frame_ts_ms;
int frame_duration_ms;
int sleep_ms;

int main(void) {
	#if SIMULATOR == 0
	// save and quit with home
	// Disable On/Off and Home kernel handling
	__asm__ volatile("svc #54"); // SVC_USB_WILL_EXECUTE_DFU
	__asm__ volatile("svc #44"); // SVC_POWER_SUSPEND

	load_data();
	#endif
	
	//display_string_transparant("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789\n.:,;(*!?}^)#${%&-+@", (eadk_point_t){0, 0}, eadk_color_white, 1);

	while (1) {
		start_frame_ts_ms = eadk_timing_millis();
		keyboard_state = eadk_keyboard_scan();

		//-----------------------------

		update_game();
		update_screen();
		
		
		// ----------------------------

		if (eadk_keyboard_key_down(keyboard_state, eadk_key_home)) {
			#if SIMULATOR == 0
			save_data();
			// Restore On/Off and Home kernel handling
			__asm__ volatile("svc #51"); // SVC_USB_DID_EXECUTE_DFU
			#endif

			return 0;
		}
		if (eadk_keyboard_key_down(keyboard_state, eadk_key_sqrt)) {
			volatile int *ptr = (int *)0xFFFFFFFF; // crash
			*ptr = 0;
		}
		if (eadk_keyboard_key_down(keyboard_state, eadk_key_exe)) debug_mode();

		// cap fps
		end_frame_ts_ms = eadk_timing_millis();
		frame_duration_ms = end_frame_ts_ms - start_frame_ts_ms;
		sleep_ms = (1000 / target_fps) > frame_duration_ms ? (1000 / target_fps) - frame_duration_ms : 0;
		
		eadk_timing_msleep(sleep_ms);

	}

	return 0;
}

