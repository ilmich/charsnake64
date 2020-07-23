#include <cbm.h>
#include <conio.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <unistd.h>

#define VIDEO_MEMORY 0x0400
#define SNAKE_HEAD 35
#define SNAKE_TAIL 43
#define SNAKE_BODY 87

#define EMPTY 0
#define APPLE 69
#define WALL 90

#define SNAKE_RIGHT 1
#define SNAKE_DOWN 40
#define SNAKE_LEFT -1
#define SNAKE_UP -40

#define GAME_INTRO 1
#define GAME_PLAY 2

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

unsigned int level1[] = {
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
		80 + 40, 80 +40 + 39,
		80 + (40 * 2), 80 + (40*2) + 39,
		80 + (40 * 3), 80 + (40*3) + 39,
		80 + (40 * 4), 80 + (40*4) + 39,
		80 + (40 * 5), 80 + (40*5) + 39,
		80 + (40 * 6), 80 + (40*6) + 39,
		80 + (40 * 7), 80 + (40*7) + 39,
		80 + (40 * 8), 80 + (40*8) + 39,
		80 + (40 * 9), 80 + (40*9) + 39,
		80 + (40 * 10), 80 + (40*10) + 39,
		80 + (40 * 11), 80 + (40*11) + 39,
		80 + (40 * 12), 80 + (40*12) + 39,
		80 + (40 * 13), 80 + (40*13) + 39,
		80 + (40 * 14), 80 + (40*14) + 39,
		80 + (40 * 15), 80 + (40*15) + 39,
		80 + (40 * 16), 80 + (40*16) + 39,
		80 + (40 * 17), 80 + (40*17) + 39,
		80 + (40 * 18), 80 + (40*18) + 39,
		80 + (40 * 19), 80 + (40*19) + 39,
		80 + (40 * 20), 80 + (40*20) + 39,
		80 + (40 * 21), 80 + (40*21) + 39,
		960,961,962,963,964,965,966,967,968,969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,999
		};

typedef struct  {
	int head;
	int tail;
	unsigned char grow;
	int direction;
} snake_t;

struct {
	snake_t snake;
	unsigned int score;
	unsigned char lives;
	signed char screen[1000];
} pup = { {},0,3 };

void build_level(unsigned int level[], int size) {
	int i=0;
	for (; i < size;i++) {
		POKE(VIDEO_MEMORY + level[i], 102);
		pup.screen[level[i]] = WALL;
	}
}

char lives[][4] = {" ", "# ", "## ", "###"};

void updateScoreLives(void) {
	char str[20];	
	
	sprintf(str,"score:%06d",pup.score);
	cputsxy(0,0,str);
	cputsxy(0,1,"lives:");
	cputsxy(6,1, lives[pup.lives]);
}

void new_apple(void) {
	int i;
	for (;;) {
		i=rand() % (1000 - 160);
		if (pup.screen[i + 160] == EMPTY) {
			POKE(VIDEO_MEMORY + 160 + i, 83);
			POKE(COLOR_RAM + 160 + i, COLOR_YELLOW);
			pup.screen[i + 160] = APPLE;
			return;
		}
	}
}

int update(void) {	
	int go_to = pup.snake.head + pup.snake.direction;

	//check events 
	if (pup.screen[go_to] != EMPTY && pup.screen[go_to] != APPLE) { // collision with body or wall
		return 1;
	} else if (pup.screen[go_to] == APPLE) {
		pup.snake.grow = 1;
		pup.score += 10;
		updateScoreLives();
		new_apple();
	}

	// set the change of direction
	pup.screen[pup.snake.head]=pup.snake.direction;	
	POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_BODY);
	POKE(COLOR_RAM + pup.snake.head, COLOR_BROWN);

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
	int x;    
	
	clrscr();
	VIC.addr = 0x15;
	
	for (x=0;x<1000;x++) {
		pup.screen[x]=EMPTY;
	}
	build_level(level1, sizeof(level1) / 2);

    pup.snake.head = 380;
    pup.snake.tail = 379;
    pup.snake.grow = 0;
    pup.snake.direction = SNAKE_RIGHT;
    
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
	unsigned int i = 0;
	unsigned int sleeps = 710;

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
		if (update ()) {
			pup.lives -= 1;
			updateScoreLives();
			sleep(1);
			if (pup.lives == 0) {
				return;
			}
			init_level();
		}
		for ( i = 0; i<sleeps ; i++) {}
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
