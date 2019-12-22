enum ANIMATE_TYPE
{
	ANIMATE_BOMB,
	ANIMATE_EXPLODE,
	ANIMATE_BOMB_TORPEDO,
	ANIMATE_SUBMARINE_RISE
};

struct VECTOR2D
{
	int xv;
	int yv;
};

struct PARTICLE
{
	int x;
	int y;
	struct VECTOR2D speed;
};



struct BOMB_TORPEDO
{
	int x, y;
	
	SDL_Texture* texture_bomb_torpedo;
	int w;
	int h;

	int past_frames_count;
	int current_frame;
	int max_frame;
	int one_row_frames;
	int tile_width;
	int tile_height;
};

struct SUBMARINE_RISE
{
	int x, y;

	SDL_Texture* texture_submarine_rise;
	int w;
	int h;

	int past_frames_count;
	int current_frame;
	int max_frame;
	int one_row_frames;
	int tile_width;
	int tile_height;
};

struct ANIMATE
{
	struct ANIMATE* next;
	int type;
	void *p;
};



void animatemgr_init(SDL_Renderer* renderer);
void animatemgr_quit();

struct BOMB* animatemgr_bomb_create(int x, int y);
struct BOOM* animatemgr_boom_create(int x, int y);
void animatemgr_update();
void animatemgr_load(char* filename, int type);
void animatemgr_draw();