#include <cbm.h>
#include <conio.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <time.h>
#include <unistd.h>

#define VIDEO_MEMORY 0x0400
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


unsigned char intro[] = {
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x43,0x08,0x01,0x12,0x53,0x0e,0x01,0x0b,0x05,0x36,0x34,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x2b,0x2a,0x2a,0x2a,0x2a,0x20,0x20,0x20,0x20,0x2a,0x2a,0x2a,0x2a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x2a,0x20,0x20,0x20,0x20,0x2a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x2a,0x20,0x20,0x20,0x20,0x2a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x2a,0x2a,0x2a,0x2a,0x2a,0x2a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x50,0x12,0x05,0x13,0x13,0x20,0x06,0x09,0x12,0x05,0x20,0x0f,0x0e,0x20,0x0a,0x0f,0x19,0x20,0x32,0x20,0x14,0x0f,0x20,0x13,0x14,0x01,0x12,0x14,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20	
};

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

void * levels[] = {&level1, &level2, &level3};

typedef struct  {
	unsigned int head;
	unsigned int tail;
	unsigned char grow;
    unsigned char apples;
	signed char direction;
    signed char ch_direction;
    clock_t speed; //in jiffy clock cicles
	clock_t updated; //in jiffy clock cicles
} snake_t;

struct {
	snake_t snake;
	unsigned int score;
	char lives;
    char level;
	signed char screen[1000];
} pup;

char lives[][4] = {" ", "# ", "## ", "###"};

void build_level(unsigned char *data) {
	unsigned int vidx=80;
	unsigned char code,count,chr;
	
    for (;;) {
	    code = *data++;
	    if (code == 0xff) {
            count = *data++;
            chr = *data++;
            for (; count>0;count--) {
                POKE(VIDEO_MEMORY + vidx, chr);
                pup.screen[vidx] = chr;            
                ++vidx;
            }
            continue;
	    }
	    break;
	}
}

void updateScoreLives(void) {
	char str[15];

	sprintf(str,"score:%06d",pup.score);
	cputsxy(0,0,str);
	cputsxy(0,1,"lives:");
	cputsxy(6,1, lives[pup.lives]);
}

void new_apple() {
	unsigned int i;
	for (;;) {
		i= (rand() % 840) + 160; // 1000-160
		if (pup.screen[i] == EMPTY) {
			POKE(VIDEO_MEMORY+i, 88);
			POKE(COLOR_RAM+i, COLOR_RED);
			pup.screen[i] = APPLE;
            return;
		}
	}
}

void open_door(void) {
    POKE(VIDEO_MEMORY + (40 * 12 ) + 39,80);    
    POKE(VIDEO_MEMORY + (40 * 13 ) + 39,103);
    POKE(VIDEO_MEMORY + (40 * 14 ) + 39,103);
    POKE(VIDEO_MEMORY + (40 * 15 ) + 39,122);
    pup.screen[ (40*12) + 39] = 80;
    pup.screen[ (40*13) + 39] = 103;
    pup.screen[ (40*14) + 39] = 103;
    pup.screen[ (40*15) + 39] = 122;
}

char update(clock_t jiffy) {
	unsigned int go_to = 0;

	if ((jiffy - pup.snake.updated) < pup.snake.speed) {
		return ACTION_SNAKE_NOTHING;
	}

	go_to = pup.snake.head + pup.snake.direction;
	pup.snake.updated = clock();
	//check events 
	if (pup.screen[go_to] == APPLE) {
		pup.snake.grow += 3;
		pup.score += 10;
        ++pup.snake.apples;
        // TODO make speed calculation better
        pup.snake.speed = 6 - pup.snake.apples/10;
		updateScoreLives();
        if (pup.snake.apples == 30) {
            open_door();
        }		
	} else if (pup.screen[go_to] == 80 || pup.screen[go_to] == 103 || pup.screen[go_to] == 122) {        
        return ACTION_SNAKE_LEVELUP;
    } else if (pup.screen[go_to] != EMPTY) {        
        return ACTION_SNAKE_DEAD;
	}

    // set the change of direction    
    pup.screen[pup.snake.head]=pup.snake.direction;
    POKE(COLOR_RAM + pup.snake.head, COLOR_GREEN);
    if (pup.snake.ch_direction) {
        POKE(VIDEO_MEMORY + pup.snake.head, pup.snake.ch_direction);
        pup.snake.ch_direction =0;
    } else {
        POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_BODY);
    }

	//move head
	pup.snake.head = go_to;
	pup.screen[pup.snake.head]=pup.snake.direction;
	POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
	POKE(COLOR_RAM + pup.snake.head, COLOR_GREEN);

	if (pup.snake.grow > 0) {
		--pup.snake.grow;
		return ACTION_SNAKE_NOTHING;
	}

	go_to = pup.screen[pup.snake.tail];
	pup.screen[pup.snake.tail]=EMPTY;
	POKE(VIDEO_MEMORY + pup.snake.tail, 32);
	pup.snake.tail += go_to;
	POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

	return ACTION_SNAKE_NOTHING;

}



void init_level(void) {
	unsigned int x = 0;

	clrscr();
	VIC.addr = 0x15;

	for (x=0;x<1000;x++) {
		pup.screen[x]=EMPTY;
	}
	build_level(levels[ (pup.level-1) % (sizeof(levels)/2)]);

	pup.snake.head = 460;
	pup.snake.tail = 459;
	pup.snake.grow = 0;
	pup.snake.direction = SNAKE_RIGHT;
	pup.snake.updated = clock();
	pup.snake.speed = 6;
    pup.snake.apples = 0;

	pup.screen[pup.snake.head] = pup.snake.direction;
	pup.screen[pup.snake.tail] = pup.snake.direction;
	POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
	POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

    for (x=0;x<30;x++) {
        new_apple();
    }

	updateScoreLives();
}

void game_intro(void) {
    clrscr();
	memcpy( (unsigned char*)VIDEO_MEMORY, intro, 1024);
	VIC.addr = 0x17;
	for (;;) {
		char fat = joy_read(JOY_2);
		if (JOY_BTN_1(fat)) {
			return;
		}
	}
}

void control_snake(void) {        
		char fat = joy_read(JOY_2);
        
        if (pup.snake.direction == SNAKE_RIGHT) {
            if (fat == JOY_UP_MASK) {
                pup.snake.ch_direction = 126;
                pup.snake.direction = SNAKE_UP;
                return;
            }
            if (fat == JOY_DOWN_MASK) {
                pup.snake.ch_direction = 123;
                pup.snake.direction = SNAKE_DOWN;
                return;
            }
        }
        
        if (pup.snake.direction == SNAKE_LEFT) {
            if (fat == JOY_UP_MASK) {
                pup.snake.ch_direction = 124;
                pup.snake.direction = SNAKE_UP;
                return;
            }
            if (fat == JOY_DOWN_MASK) {
                pup.snake.ch_direction = 108;
                pup.snake.direction = SNAKE_DOWN;
                return;
            }
        }
        
        if (pup.snake.direction == SNAKE_DOWN) {
            if (fat == JOY_RIGHT_MASK) {
                pup.snake.ch_direction = 124;
                pup.snake.direction = SNAKE_RIGHT;
                return;
            }
            if (fat == JOY_LEFT_MASK) {
                pup.snake.ch_direction = 126;
                pup.snake.direction = SNAKE_LEFT;
                return;
            }
        }
        
        if (pup.snake.direction == SNAKE_UP) {
            if (fat == JOY_RIGHT_MASK) {
                pup.snake.ch_direction = 108;
                pup.snake.direction = SNAKE_RIGHT;
                return;
            }
            if (fat == JOY_LEFT_MASK) {
                pup.snake.ch_direction = 123;
                pup.snake.direction = SNAKE_LEFT;
                return;
            }
        }       

}

void game_play(void) {
    char status = 0;
	pup.lives = 3;
	pup.score = 0;
    pup.level = 1;
	init_level();

	for (;;) {
        control_snake();   
		if (status = update (clock())) {
            if (status == ACTION_SNAKE_DEAD) {
                --pup.lives;
                updateScoreLives();
            }
            if (status == ACTION_SNAKE_LEVELUP) {
                pup.level++;                
            }
			sleep(1);
			if (pup.lives == 0) {
				return;
			}
			init_level();
		}
	}
}

int main(void) {
	VIC.bordercolor = COLOR_BLACK;
	VIC.bgcolor0 = COLOR_BLACK;

	joy_install (joy_static_stddrv);
	for (;;) {	
		game_intro();
		game_play();
	}

	return EXIT_SUCCESS;
}
