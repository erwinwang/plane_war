#pragma once
typedef enum 
{
	SOUND_MUSIC = 0,
	SOUND_SFX = 1
}sound_type;

#define SFX_BOOM		0
#define	BGM_ENGINE		0

int Sound_Init();
void Sound_Quit();
int Sound_Play(const char *file_name);
int Sound_PlaySFX(char *id, int loop);
int Sound_Load(char *filename, char *str, int type);
