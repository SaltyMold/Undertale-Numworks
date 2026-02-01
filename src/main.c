#include "libs/eadk.h"
#include "libs/storage.h"
#include "app.h"
#include "display.h"
#include "assets/font.h"
#include "game.h"
#include "utils.h"
#include "entity.h"

#define SIMULATOR 1 // no syscall and storage

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Undertale";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

eadk_keyboard_state_t keyboard_state;


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
	
	#else
	display_screen_bg();
	for (int i = 0; i < 20; ++i) {
		eadk_timing_msleep(50); 
		eadk_keyboard_scan();
	}
	#endif

	init_entities();
	int entity = create_entity(true, 170, 120, 16, 16, 20, NULL);
	
	//display_string_transparant("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789\n.:,;(*!?}^)#${%&-+@", (eadk_point_t){0, 0}, eadk_color_white, 0);

	while (1) {
		start_frame_ts_ms = eadk_timing_millis();
		keyboard_state = eadk_keyboard_scan();

		//-----------------------------

		last_player_stats = player_stats;
		last_game_stats = game_stats;

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
		sleep_ms = (1000 / TARGET_FPS) > frame_duration_ms ? (1000 / TARGET_FPS) - frame_duration_ms : 0;
		
		eadk_timing_msleep(sleep_ms);

	}

	return 0;
}

