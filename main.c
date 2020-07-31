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
#define APPLE 83
#define WALL 102

#define GAME_INTRO 1
#define GAME_PLAY 2

#define SNAKE_RIGHT 1
#define SNAKE_DOWN 40
#define SNAKE_LEFT -1
#define SNAKE_UP -40

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

typedef struct  {
	unsigned int head;
	unsigned int tail;
	unsigned char grow;
	signed char direction;
    clock_t speed; //in jiffy clock cicles
	clock_t updated; //in jiffy clock cicles
} snake_t;

struct {
	snake_t snake;
	unsigned int score;
	char lives;
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

void new_apple(void) {
	unsigned int i;
	for (;;) {
		i= (rand() % 840) + 160; // 1000-160
		if (pup.screen[i] == EMPTY) {
			POKE(VIDEO_MEMORY+i, 83);
			POKE(COLOR_RAM+i, COLOR_YELLOW);
			pup.screen[i] = APPLE;
            return;
		}
	}
}

char update(clock_t jiffy) {
	unsigned int go_to = 0;

	if ((jiffy - pup.snake.updated) < pup.snake.speed) {
		return 0;
	}

	go_to = pup.snake.head + pup.snake.direction;
	pup.snake.updated = clock();
	//check events 
	if (pup.screen[go_to] == APPLE) {
		pup.snake.grow = 1;
		pup.score += 10;
		updateScoreLives();
		new_apple();
	} else if (pup.screen[go_to] != EMPTY) {
		return 1;
	}

    // set the change of direction    
    pup.screen[pup.snake.head]=pup.snake.direction;
    POKE(COLOR_RAM + pup.snake.head, COLOR_BROWN);
    POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_BODY);        

	//move head
	pup.snake.head = go_to;
	pup.screen[pup.snake.head]=pup.snake.direction;
	POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
	POKE(COLOR_RAM + pup.snake.head, COLOR_GREEN);

	if (pup.snake.grow == 1) {
		pup.snake.grow = 0;
		return 0;
	}

	go_to = pup.screen[pup.snake.tail];
	pup.screen[pup.snake.tail]=EMPTY;
	POKE(VIDEO_MEMORY + pup.snake.tail, 32);
	pup.snake.tail += go_to;
	POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

	return 0;

}

void init_level(void) {
	unsigned int x = 0;

	clrscr();
	VIC.addr = 0x15;

	for (x=0;x<1000;x++) {
		pup.screen[x]=EMPTY;
	}
	build_level(level1);

	pup.snake.head = 380;
	pup.snake.tail = 379;
	pup.snake.grow = 0;
	pup.snake.direction = SNAKE_RIGHT;
	pup.snake.updated = clock();
	pup.snake.speed = 6;

	pup.screen[pup.snake.head] = pup.snake.direction;
	pup.screen[pup.snake.tail] = pup.snake.direction;
	POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
	POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

	new_apple();
    new_apple();
    new_apple();
    new_apple();
    
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

void game_play(void) {
	pup.lives = 3;
	pup.score = 0;
	init_level();

	for (;;) {
		char fat = joy_read(JOY_2);
		if (JOY_UP(fat) && pup.snake.direction != SNAKE_DOWN) {
			pup.snake.direction = SNAKE_UP;            
		} else if (JOY_DOWN(fat) && pup.snake.direction != SNAKE_UP) {
			pup.snake.direction = SNAKE_DOWN;            
		} else if (JOY_LEFT(fat) && pup.snake.direction != SNAKE_RIGHT) {
			pup.snake.direction = SNAKE_LEFT;            
		} else if (JOY_RIGHT(fat) && pup.snake.direction != SNAKE_LEFT) {
			pup.snake.direction = SNAKE_RIGHT;            
		}
		if (update (clock())) {
			--pup.lives;
			updateScoreLives();
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
