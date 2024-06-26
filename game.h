#if (__C64__)
#define VIDEO_MEMORY 0x0400
#elif (__C16__)
#define VIDEO_MEMORY 0x0c00
#endif
#define SNAKE_HEAD 35
#define SNAKE_TAIL 43
#define SNAKE_BODY 87

#define EMPTY 32
#define APPLE 88
#define WALL 102

#define GAME_INTRO 1
#define GAME_PLAY 2

#define SNAKE_RIGHT 1
#define SNAKE_DOWN 40
#define SNAKE_LEFT -1
#define SNAKE_UP -40

#define ACTION_SNAKE_NOTHING 0
#define ACTION_SNAKE_DEAD 1
#define ACTION_SNAKE_LEVELUP 2

unsigned char level1[] = {
    0xff,40,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,40,102,
    0x00,0x00,0x00
};

unsigned char level2[] = {
    0xff,40,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,40,102,
    0x00,0x00,0x00
};

unsigned char level3[] = {
    0xff,40,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,5,0x20, 0xff,28,102, 0xff,5,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,5,0x20, 0xff,28,102, 0xff,5,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,40,102,
    0x00,0x00,0x00
};

unsigned char level4[] = {
    0xff,40,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,8,0x20, 0xff,11,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,11,102,0xff,9,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,40,102,
    0x00,0x00,0x00
};

unsigned char level5[] = {
    0xff,40,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
   	0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,10,102,0xff,10,0x20,0xff,1,102,
   	0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
   	0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,18,0x20, 0xff,1,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,9,0x20, 0xff,10,102,0xff,19,0x20,0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,40,102,
    0x00,0x00,0x00
};

unsigned char level6[] = {
    0xff,40,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,5,0x20, 0xff,28,102, 0xff,5,0x20, 0xff,1,102,
    0xff,1,102, 0xff,5,0x20, 0xff,1,102, 0xff,32,0x20, 0xff,1,102,
    0xff,1,102, 0xff,5,0x20, 0xff,1,102, 0xff,32,0x20, 0xff,1,102,
    0xff,1,102, 0xff,5,0x20, 0xff,1,102, 0xff,32,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,32,0x20, 0xff,1,102, 0xff,5,0x20, 0xff,1,102,
    0xff,1,102, 0xff,32,0x20, 0xff,1,102, 0xff,5,0x20, 0xff,1,102,
    0xff,1,102, 0xff,32,0x20, 0xff,1,102, 0xff,5,0x20, 0xff,1,102,
    0xff,1,102,	0xff,5,0x20, 0xff,28,102, 0xff,5,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102, 0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,1,102,	0xff,38,0x20, 0xff,1,102,
    0xff,40,102,
    0x00,0x00,0x00
};

void *levels[] = {&level1, &level2, &level3, &level4, &level5, &level6};
char lives[][4] = {" ", "# ", "## ", "###"};
/*
#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
*/
unsigned char *curlevel;
typedef struct  {
    unsigned int head;
    unsigned int tail;
    unsigned char grow;
    unsigned char apples;
    signed char direction;
    signed char tail_direction;
    unsigned char body_chr;
    unsigned char speed; //in raster irq frames
    unsigned char updated; //in raster irq frames
} snake_t;

struct {
    snake_t snake;
    unsigned int score;
    char lives;
    char level;
    char type; // 0 -- snake or die ; 1 -- climb the tower
    unsigned char *level_arr;
} pup;
/*
#pragma bss-name (pop)
#pragma data-name (pop)
*/
