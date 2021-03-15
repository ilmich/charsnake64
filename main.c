#include <cbm.h>
#include <conio.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <time.h>

#include "game.h"

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
                screen[vidx] = chr;
                ++vidx;
            }
            continue;
        }
        break;
    }
}

void update_score(void) {
    unsigned char str[15];

    sprintf(str,"score:%06d",pup.score);
    cputsxy(0,0,str);
}

void update_level(void) {
    unsigned char str[15];

    sprintf(str,"level:%02d",pup.level);
    cputsxy(32,0,str);
}

void update_lives(void) {
    cputsxy(0,1,"lives:");
    cputsxy(6,1, lives[pup.lives]);
}

void new_apple() {
    unsigned int i;
    for (;;) {
        i= (rand() % 840) + 160; // 1000-160
        if (screen[i] == EMPTY) {
            POKE(VIDEO_MEMORY+i, APPLE);
            POKE(COLOR_RAM+i, COLOR_RED);
            screen[i] = APPLE;
            return;
        }
    }
}

void open_door(void) {
    POKE(VIDEO_MEMORY + (40 * 12 ) + 39,80);    
    POKE(VIDEO_MEMORY + (40 * 13 ) + 39,103);
    POKE(VIDEO_MEMORY + (40 * 14 ) + 39,103);
    POKE(VIDEO_MEMORY + (40 * 15 ) + 39,122);
    screen[ (40*12) + 39] = 80;
    screen[ (40*13) + 39] = 103;
    screen[ (40*14) + 39] = 103;
    screen[ (40*15) + 39] = 122;
}

char update() {
    unsigned int go_to;
    unsigned char ch_go_to;

    if ((clock() - pup.snake.updated) < pup.snake.speed) {
        return ACTION_SNAKE_NOTHING;
    }

    go_to = pup.snake.head + pup.snake.direction;
    ch_go_to = screen[go_to];
    pup.snake.updated = clock();
    //check events 
    if (ch_go_to == APPLE) {
        pup.snake.grow += 4;
        pup.score += 10;
        ++pup.snake.apples;
        // TODO make speed calculation better
        pup.snake.speed = 7 - pup.snake.apples/8;
        update_score();
        if (pup.snake.apples == 24) {
            open_door();
        }
    } else if (ch_go_to == 80 || ch_go_to == 103 || ch_go_to == 122) {
        return ACTION_SNAKE_LEVELUP;
    } else if (ch_go_to != EMPTY) {
        return ACTION_SNAKE_DEAD;
    }

    // set the change of direction
    screen[pup.snake.head]=pup.snake.direction;
    POKE(COLOR_RAM + pup.snake.head, COLOR_GREEN);
    POKE(VIDEO_MEMORY + pup.snake.head, pup.snake.body_chr);
    if (pup.snake.body_chr!= SNAKE_BODY) {
        pup.snake.body_chr = SNAKE_BODY;
    } 

    //move head
    pup.snake.head = go_to;
    screen[pup.snake.head]=pup.snake.direction;
    POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
    POKE(COLOR_RAM + pup.snake.head, COLOR_GREEN);

    if (pup.snake.grow > 0) {
        --pup.snake.grow;
        return ACTION_SNAKE_NOTHING;
    }

    go_to = screen[pup.snake.tail];
    screen[pup.snake.tail]=EMPTY;
    POKE(VIDEO_MEMORY + pup.snake.tail, 32);
    pup.snake.tail += go_to;
    POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

    return ACTION_SNAKE_NOTHING;

}

void init_level(void) {
    unsigned int x;

    clrscr();
    VIC.addr = 0x15;

    do {
        screen[x]=EMPTY;
    } while (++x < 1024);

    build_level(levels[ (pup.level-1) % (sizeof(levels)/2)]);    
    update_level();
    pup.snake.head = 530;
    pup.snake.tail = 529;
    pup.snake.grow = 0;
    pup.snake.direction = SNAKE_RIGHT;
    pup.snake.updated = clock();
    pup.snake.speed = 7;
    pup.snake.apples = 0;

    screen[pup.snake.head] = pup.snake.direction;
    screen[pup.snake.tail] = pup.snake.direction;
    POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
    POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

    for (x=0; x<24; x++) {
        new_apple();
    }

    update_score();
    update_lives();
}

void game_intro(void) {
    clrscr();
    memcpy( (unsigned char*)VIDEO_MEMORY + 120, intro_packed, sizeof(intro_packed));
    VIC.addr = 0x15;
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
                pup.snake.body_chr = 126;
                pup.snake.direction = SNAKE_UP;
                return;
            }
            if (fat == JOY_DOWN_MASK) {
                pup.snake.body_chr = 123;
                pup.snake.direction = SNAKE_DOWN;
                return;
            }
        }

        if (pup.snake.direction == SNAKE_LEFT) {
            if (fat == JOY_UP_MASK) {
                pup.snake.body_chr = 124;
                pup.snake.direction = SNAKE_UP;
                return;
            }
            if (fat == JOY_DOWN_MASK) {
                pup.snake.body_chr = 108;
                pup.snake.direction = SNAKE_DOWN;
                return;
            }
        }

        if (pup.snake.direction == SNAKE_DOWN) {
            if (fat == JOY_RIGHT_MASK) {
                pup.snake.body_chr = 124;
                pup.snake.direction = SNAKE_RIGHT;
                return;
            }
            if (fat == JOY_LEFT_MASK) {
                pup.snake.body_chr = 126;
                pup.snake.direction = SNAKE_LEFT;
                return;
            }
        }

        if (pup.snake.direction == SNAKE_UP) {
            if (fat == JOY_RIGHT_MASK) {
                pup.snake.body_chr = 108;
                pup.snake.direction = SNAKE_RIGHT;
                return;
            }
            if (fat == JOY_LEFT_MASK) {
                pup.snake.body_chr = 123;
                pup.snake.direction = SNAKE_LEFT;
                return;
            }
        }

}

void sleep(char cicles) {
    clock_t end = clock() + cicles;
    do { } while (clock() < end);
}

void game_play(void) {
    char status;
    pup.lives = 3;
    pup.score = 0;
    pup.level = 1;
    init_level();

    for (;;) {
        control_snake();
        if (status = update ()) {
            if (status == ACTION_SNAKE_DEAD) {
                --pup.lives;
                update_lives();
            }
            if (status == ACTION_SNAKE_LEVELUP) {
                pup.level++;
            }
            sleep(60);
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

    return 0;
}
