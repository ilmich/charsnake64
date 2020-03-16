#include <conio.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
#define APPLE 1
#define SNAKE_RIGHT 2
#define SNAKE_DOWN 3
#define SNAKE_LEFT 4
#define SNAKE_UP 5
#define WALL 6


#define poke(addr, value)  POKE(addr, value)
#define peek(addr) PEEK(addr)

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

// TODO: fallo meglio.. non ha senso sprecare byte per due righe in meno
unsigned short screen[1024];

void build_level(unsigned int level[], int size) {
	int i=0;
	for (; i < size;i++) {
		poke(VIDEO_MEMORY + level[i], 102);
		screen[level[i]] = WALL;
	}
}

// TODO: fallo meglio.. evitare che si oltrepassi l'area di gioco
// TODO: le mele dovrebbero essere sempre 4 ma invece ogni tanto ne sparisce una :( --> dipende dal fatto che alle volte
//	 spuntano fuori dal campo di gioco

void new_apple(void) {
	int i;	
	for (;;) {
		i=rand() % (1024 - 80 - 80 -80);
		if (screen[i + 160] == EMPTY) {
			poke(VIDEO_MEMORY + 160 + i, 83);
			screen[i + 160] = APPLE;
			return;
		}		
	}	
	
}

int move_offs(int direction) {
	if (direction == SNAKE_RIGHT)
		return 1;
	if (direction == SNAKE_LEFT)
		return -1;
	if (direction == SNAKE_UP)
		return -40;
	if (direction == SNAKE_DOWN)
		return 40;
}

int update(snake *pup) {

	int go_to;

	go_to = pup->head + move_offs(pup->direction);
	
	//check events 
	if (screen[go_to] > APPLE) { // collision with body or wall
		return 1;
	} else if (screen[go_to] == APPLE) {
		pup->grow = 1;
		new_apple();		
	}
		
	// set the change of direction
	screen[pup->head]=pup->direction;	
	poke(VIDEO_MEMORY + pup->head, SNAKE_BODY);
	
	//move head
	pup->head = go_to;	
	screen[pup->head]=pup->direction;
	poke(VIDEO_MEMORY + pup->head, SNAKE_HEAD);
		
	if (pup->grow-- > 0) {		
		return 0;
	}
	
	//move tail
	go_to = move_offs(screen[pup->tail]);
	screen[pup->tail]=EMPTY;
	poke(VIDEO_MEMORY + pup->tail, 32);	
	pup->tail += go_to;
	poke(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);
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
	
	bgcolor(0);
	bordercolor(0);
	clrscr();
	
	for (x=0;x< 1024;x++) {
		screen[x]=EMPTY;
	}
	build_level(level1, sizeof(level1) / 2);
			
	init_snake(pup);
	screen[pup->head] = SNAKE_RIGHT;
	screen[pup->tail] = SNAKE_RIGHT;
	poke(VIDEO_MEMORY + pup->head, SNAKE_HEAD);
	poke(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);
	
	new_apple();
	new_apple();
	new_apple();
	new_apple();
}

int main(void) {	
	unsigned int i = 0;
	unsigned int sleeps = 610;
	snake pup;
		
	joy_install (joy_static_stddrv);	
	poke(0xd018, 0x15);	
	init_level(&pup);
	
	do {
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
		for ( i = 0; i<sleeps ; i++) {
		}		
		
	} while(true);
	
	return EXIT_SUCCESS;
}

