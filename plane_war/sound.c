#include <stdio.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include "sound.h"
#include "list.h"

struct list l = { 0 };

int Sound_Init()
{
	//l = list_create();
	const int    TMP_FREQ = MIX_DEFAULT_FREQUENCY;
	const Uint16 TMP_FORMAT = MIX_DEFAULT_FORMAT;
	const int    TMP_CHAN = 2;
	const int    TMP_CHUNK_SIZE = 512;

	return Mix_OpenAudio(TMP_FREQ, TMP_FORMAT, TMP_CHAN, TMP_CHUNK_SIZE);
}

void Sound_Quit()
{
	Mix_CloseAudio();
	return;
}

int Sound_Play(const char *file_name)
{
	Mix_Music *mix_music;

	if ((mix_music = Mix_LoadMUS(file_name)) == NULL)
	{
		printf("call Mix_LoadMUS failed:%s\n", Mix_GetError());
		return -1;
	}

	if (Mix_PlayMusic(mix_music, -1) == -1)
	{
		printf("Mix_PlayMusic:%s\n", Mix_GetError());
		return -1;
	}
	printf("after call Mix_PlayMusic\n");

	return 0;
}

int Sound_Load(char *filename, char *str, int type)
{
	struct element *e = (struct element*)malloc(sizeof(struct element));
	unsigned int key = ELFhash(str);
	e->key = key;
	e->next = NULL;

	if (type == SOUND_MUSIC)
	{
		Mix_Music* pmusic = Mix_LoadMUS(filename);
		if (pmusic == 0)
		{
			printf("[sound::load]Cound not load music:%s", Mix_GetError());
			return 0;
		}
		e->ptr = pmusic;
		list_enqueue(&l, e);
	}
	else if (type == SOUND_SFX)
	{
		Mix_Chunk* pchunk = Mix_LoadWAV(filename);
		if (pchunk == 0)
		{
			printf("[sound::load]Cound not load sfx:%s", Mix_GetError());
			return 0;
		}
		e->ptr = pchunk;
		list_enqueue(&l, e);
	}
	return 0;
}

int Sound_PlaySFX(char *id, int loop)
{
	void *p = list_search(&l, id);
	if (p == NULL)
	{
		return 0;
	}
	if (Mix_PlayChannel(-1, p, loop) == -1)
	{
		printf("Mix_PlayChannel:%s\n", Mix_GetError());
		return 0;
	}
	return 1;
}

int Sound_PlayMusic(char *id, int loop)
{
	void *p = list_search(&l, id);
	if (p == NULL)
	{
		return 0;
	}
	if (Mix_PlayMusic(p, loop) == -1)
	{
		printf("Mix_PlayMusic:%s\n", Mix_GetError());
		return 0;
	}
	return 1;
}