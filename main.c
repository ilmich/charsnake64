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
	bool grow;
	int direction;
} snake ;

#define VIDEO_MEMORY 0x0400
#define SNAKE_HEAD 43
#define SNAKE_TAIL 42
#define SNAKE_BODY 87

#define WALL 4
#define APPLE 5

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
// TODO: fallo meglio.. ha senso avere una struttura di 4 interi (perchè i char non sono negativi) 
//	 meglio forse un metodo move per testa e coda
int direct[4] = {1, //left
		40, // down
		-1, // right
		-40}; // up

void build_level(unsigned int level[], int size) {
	int i=0;
	for (; i < size;i++) {
		poke(VIDEO_MEMORY + level[i], 102);
		screen[level[i]] = WALL;
	}
}

// TODO: fallo meglio.. evitare che si oltrepassi l'area di gioco
void new_apple(void) {
	int i;	
	i=rand() % (1024 - 80 - 80 );
	poke(VIDEO_MEMORY + 160 + i, 83);
	screen[i + 160] = APPLE;
}

int update(snake *pup) {

	//check events 
	// TODO: fallo meglio.. lo 0 corrisponde ad uno spostamento verso destra... meglio usarlo per indicare
	//	 il vuoto
	if (screen[pup->head + direct[pup->direction]] == WALL || 
		peek(VIDEO_MEMORY + pup->head + direct[pup->direction]) == SNAKE_BODY) {
		return 1;
	}
	
	if (screen[pup->head + direct[pup->direction]] == APPLE) {
		pup->grow = 1;
	} 

	// set the change direction
	screen[pup->head]=pup->direction;	
	poke(VIDEO_MEMORY + pup->head, SNAKE_BODY);
	
	//move
	pup->head = pup->head + direct[pup->direction];	
	screen[pup->head]=pup->direction;
	poke(VIDEO_MEMORY + pup->head, SNAKE_HEAD);
		
	if (pup->grow) {
		pup->grow = 0;
		new_apple();
		return 0;
	}
	
	//move tail		
	if (screen[pup->tail] == 0) { //right
		screen[pup->tail]=0;
		poke(VIDEO_MEMORY + pup->tail, 32);		
		pup->tail += 1;
		poke(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);
		return 0;
	}
	if (screen[pup->tail] == 1) { //down
		screen[pup->tail]=0;
		poke(VIDEO_MEMORY + pup -> tail, 32);		
		pup->tail += 40;
		poke(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);
		return 0;
		
	}
	if (screen[pup->tail] == 2) { //left
		screen[pup->tail]=0;
		poke(VIDEO_MEMORY + pup -> tail, 32);
		pup->tail -= 1;
		poke(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);
		return 0;
	}
	if (screen[pup->tail] == 3) { //up
		screen[pup->tail]=0;
		poke(VIDEO_MEMORY + pup -> tail, 32);
		pup->tail -= 40;
		poke(VIDEO_MEMORY + pup->tail, SNAKE_TAIL);	
		return 0;
	}
	return 0;
	
}

void init_snake(snake *snake) {	
	snake->head = 380;
	snake->tail = 379;
	snake->grow = 0;
	snake->direction = 0;
}

void init_level(snake *pup) {
	clrscr();
	build_level(level1, sizeof(level1) / 2);
		
	init_snake(pup);
	screen[pup->head] = 0;
	screen[pup->tail] = 0;
	poke(1024 + pup->head, 42);
	poke(1024 + pup->tail, 43);
	
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
		pup.grow = false;
		if (JOY_UP(fat) && pup.direction != 1) {
			pup.direction = 3;			
		}
		if (JOY_DOWN(fat) && pup.direction != 3) {
			pup.direction = 1;
		}
		if (JOY_LEFT(fat) && pup.direction != 0) {
			pup.direction = 2;
		}
		if (JOY_RIGHT(fat) && pup.direction != 2) {
			pup.direction = 0;
		}
		if (JOY_BTN_1(fat)) {
			pup.grow = true;
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

