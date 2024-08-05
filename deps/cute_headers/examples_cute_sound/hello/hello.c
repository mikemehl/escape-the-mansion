// create header + implementation
#define CUTE_SOUND_IMPLEMENTATION
#include "cute_sound.h"

#include <stdio.h>

int main()
{
	HWND hwnd = GetConsoleWindow();
	cs_init(hwnd, 44100, 1024, NULL);
	cs_audio_source_t* jump = cs_load_wav("../jump.wav", NULL);
	cs_audio_source_t* piano = cs_load_wav("../piano2.wav", NULL);
	cs_sound_params_t params = cs_sound_params_default();
	cs_playing_sound_t jump_snd = CUTE_PLAYING_SOUND_INVALID;
	cs_playing_sound_t piano_snd = CUTE_PLAYING_SOUND_INVALID;
	printf("jump.wav has a sample rate of %d Hz.\n", jump->sample_rate);
	printf("piano2.wav has a sample rate of %d Hz.\n", piano->sample_rate);

	printf("Press the 1 or 2 keys!\n");
	printf("Press ESC to exit.\n");

	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		if (!cs_sound_is_active(jump_snd) && GetAsyncKeyState(0x31)) {
			jump_snd = cs_play_sound(jump, params);
		}

		if (!cs_sound_is_active(piano_snd) && GetAsyncKeyState(0x32))
			piano_snd = cs_play_sound(piano, params);

		cs_update(0);
	}

	cs_free_audio_source(jump);
	cs_free_audio_source(piano);
}
