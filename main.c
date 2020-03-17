#include <cbm.h>
#include <conio.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <unistd.h>


typedef struct  {
	int head;
	int tail;
	int grow;
	int direction;
} snake ;

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

unsigned short screen[1000];

void build_level(unsigned int level[], int size) {
	int i=0;
	for (; i < size;i++) {
		POKE(VIDEO_MEMORY + level[i], 102);
		screen[level[i]] = WALL;
	}
}

void new_apple(void) {
	int i;	
	for (;;) {
		i=rand() % (1000 - 80 - 80);
		if (screen[i + 160] == EMPTY) {
			POKE(VIDEO_MEMORY + 160 + i, 83);
			POKE(COLOR_RAM + 160 + i, COLOR_YELLOW);
			screen[i + 160] = APPLE;
			return;
		}		
	}	
}

int update(snake *pup) {

	int go_to = pup->head + pup->direction;
	
	//check events 
	if (screen[go_to] != EMPTY && screen[go_to] != APPLE) { // collision with body or wall
		return 1;
	} else if (screen[go_to] == APPLE) {
		pup->grow = 1;
		new_apple();		
	}
		
	// set the change of direction
	screen[pup->head]=pup->direction;	
	POKE(VIDEO_MEMORY + pup->head, SNAKE_BODY);
	POKE(COLOR_RAM + pup->head, COLOR_BROWN);
	
	//move head
	pup->head = go_to;	
	screen[pup->head]=pup->direction;
	POKE(VIDEO_MEMORY + pup->head, SNAKE_HEAD);
	POKE(COLOR_RAM + pup->head, COLOR_GREEN	);
		
	if (pup->grow-- > 0) {		
		return 0;
	}
	
	go_to = screen[pup->tail];
	screen[pup->tail]=EMPTY;
	POKE(VIDEO_MEMORY + pup->tail, 32);	
	pup->tail += go_to;
	POKE(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);
	return 0;
	
}

void init_snake(snake *snake) {	
	snake->head = 380;
	snake->tail = 379;
	snake->grow = 0;
	snake->direction = SNAKE_RIGHT;
}

void init_level(snake *pup) {
	int x;
	
	VIC.bordercolor = COLOR_BLACK;
	VIC.bgcolor0 = COLOR_BLACK;
	clrscr();
	
	for (x=0;x<1000;x++) {
		screen[x]=EMPTY;
	}
	build_level(level1, sizeof(level1) / 2);
			
	init_snake(pup);
	screen[pup->head] = pup->direction;
	screen[pup->tail] = pup->direction;
	POKE(VIDEO_MEMORY + pup->head, SNAKE_HEAD);
	POKE(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);
	
	new_apple();
	new_apple();
	new_apple();
	new_apple();
}

int main(void) {	
	unsigned int i = 0;
	unsigned int sleeps = 410;
		
	snake pup;
		
	joy_install (joy_static_stddrv);	
	VIC.addr = 0x15;
	init_level(&pup);
	
	for (;;) {
		char fat = joy_read(JOY_2);		
		if (JOY_UP(fat) && pup.direction != SNAKE_DOWN) {
			pup.direction = SNAKE_UP;			
		} else if (JOY_DOWN(fat) && pup.direction != SNAKE_UP) {
			pup.direction = SNAKE_DOWN;			
		} else if (JOY_LEFT(fat) && pup.direction != SNAKE_RIGHT) {
			pup.direction = SNAKE_LEFT;			
		} else if (JOY_RIGHT(fat) && pup.direction != SNAKE_LEFT) {
			pup.direction = SNAKE_RIGHT;			
		}		
		if (update (&pup)) {
			sleep(1);
			init_level(&pup);
		}
		for ( i = 0; i<sleeps ; i++) {	}		
		
	} 
	
	return EXIT_SUCCESS;
}

