#define LEVEL_WIDTH 10
#define LEVEL_HEIGHT 30

const short int level_map0[30][10] = {
{ 4, 4, 4, 4, 4, 4, 4, 2, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 3, 4, 4, 4, 1, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 2, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 1, 4, 4, 3, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 1, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 2, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 3, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 1, 4, 4 },
{ 4, 2, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 3, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 2, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 1, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 1, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 2, 4, 4, 4, 4, 3, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
{ 4, 4, 1, 4, 4, 4, 4, 4, 4, 4 },
};

const short int level_sand_map0[30][10] = {
{ 3, 1, 1, 1, 1, 1, 1, 1, 7, 1 },
{ 1, 1, 4, 1, 1, 1, 1, 8, 1, 1 },
{ 1, 7, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 3, 1, 1 },
{ 1, 7, 7, 7, 1, 1, 7, 1, 9, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 8, 1, 1 },
{ 1, 1, 9, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 3, 4, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 3, 1, 1, 1 },
{ 1, 2, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 7, 1, 1, 9, 8, 1, 3, 2, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 7, 1, 1 },
{ 1, 3, 1, 3, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 5, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 8, 9, 3, 1, 1 },
{ 1, 2, 1, 1, 1, 1, 1, 3, 1, 1 },
{ 5, 5, 1, 1, 1, 1, 1, 3, 1, 1 },
{ 8, 9, 6, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 3, 1, 1, 1, 3, 1, 1, 7, 1, 1 },
{ 1, 1, 1, 1, 7, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 1, 3, 1, 5, 4, 1, 1, 7, 1 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 3, 7, 1, 1, 1, 1, 1, 8, 5 },
{ 1, 1, 1, 1, 1, 7, 3, 1, 1, 1 },
{ 1, 1, 1, 6, 1, 1, 1, 1, 1, 1 },
{ 9, 7, 1, 1, 1, 1, 1, 1, 7, 1 },
{ 4, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};