#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef void (*timer_callback)(void);

struct TIMER
{
	int ms;
	int start_frame;
	int current_frame;
	timer_callback callback;
};

struct TIME_MANAGER
{
	struct TIMER *head;
	int frame_count;
	int fps;
	int frame_length;
};

struct TIME_MANAGER *time_manager = NULL;

void timemgr_init(int fps)
{
	time_manager = (struct TIME_MANAGER *)malloc(sizeof(struct TIME_MANAGER));
	memset(time_manager, 0, sizeof(struct TIME_MANAGER));
	time_manager->fps = fps;
	time_manager->frame_length = 1000 / fps;
	time_manager->frame_count = 0;
}

void timemgr_create_timer(int ms, timer_callback callback)
{

}

void timemgr_update() 
{
	time_manager->frame_count++;
	if (time_manager->frame_count >= INT_MAX)
	{
		time_manager->frame_count = 0;
	}
}

void timemgr_quit()
{
}