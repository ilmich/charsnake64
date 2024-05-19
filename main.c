#include <cbm.h>
#include <conio.h>
#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>

#include "game.h"

void build_level() {
    unsigned int vidx=80;
    unsigned char code,count,chr;

    for (;;) {
        code = *pup.level_arr++;
        if (code == 0xff) {
            count = *pup.level_arr++;
            chr = *pup.level_arr++;
            for (; count>0;count--) {
                POKE(VIDEO_MEMORY + vidx, chr);
                ++vidx;
            }
            continue;
        }
        break;
    }
}

unsigned char frames;
#if (__C64__)
void raster_routine(void) {
    asm ("dec $d019");//        ; acknowledge IRQ
    frames++;
    asm ("jmp $ea31");//        ; return to kernel interrupt routine
}

void raster_irq(void) {
    asm ("sei");//      ; set interrupt disable flag
    asm ("lda #$7f");// ; $7f = %01111111
    asm ("sta $dc0d");//; Turn off CIAs Timer interrupts
    asm ("sta $dd0d");

    asm ("lda #$01");// ; Set Interrupt Request Mask...
    asm ("sta $d01a");//; ...we want IRQ by Rasterbeam

    asm ("lda #<%v", raster_routine);//; point IRQ Vector to our custom irq routine
    asm ("ldx #>%v", raster_routine);
    asm ("sta $314");//   ; store in $314/$315
    asm ("stx $315");

    asm ("lda #$18");// ; trigger first interrupt at row zero
    asm ("sta $d012");

    asm ("lda $d011");//; Bit#0 of $d011 is basically...
    asm ("and #$7f");// ; ...the 9th Bit for $d012
    asm ("sta $d011");//; we need to make sure it is set to zero 

    asm ("cli");//      ; clear interrupt disable flag
    asm ("rts");
}
#elif (__C16__)
void raster_routine(void) {
    asm("lda $ff09");
    asm("sta $ff09");
    frames++;
    asm("jmp $fcbe");
}

void raster_irq(void) {
    asm("sei");
    asm("lda #<%v", raster_routine);
    asm("sta $314");
    asm("lda #>%v", raster_routine);
    asm("sta $315");

    asm("lda #$02");
    asm("sta $ff0a");

    asm("lda #64");
    asm("sta $ff0b");

    asm("lda $ff09");
    asm("sta $ff09");
    asm("cli");
    asm("rts");
}
#endif

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
        i= (rand() % 840) + 120; // 1000-160
        if (PEEK(VIDEO_MEMORY+i) == EMPTY) {
            POKE(VIDEO_MEMORY+i, APPLE);
            POKE(COLOR_RAM+i, COLOR_RED);
            return;
        }
    }
}

void open_door(void) {
    POKE(VIDEO_MEMORY + (40 * 12 ) + 39,80);
    POKE(VIDEO_MEMORY + (40 * 13 ) + 39,103);
    POKE(VIDEO_MEMORY + (40 * 14 ) + 39,103);
    POKE(VIDEO_MEMORY + (40 * 15 ) + 39,122);
}

char update() {
    unsigned int go_to = pup.snake.head + pup.snake.direction;
    unsigned char ch_go_to = PEEK(VIDEO_MEMORY + go_to); 
    unsigned char tmp;

    if ((frames - pup.snake.updated) < pup.snake.speed) {
        return ACTION_SNAKE_NOTHING;
    } 

    pup.snake.updated = frames;
    //check events 
    if (ch_go_to == APPLE) {
        pup.snake.grow += 4;
        pup.score += 10;
        ++pup.snake.apples;
        // TODO make speed calculation better
        if (pup.snake.speed > 3) {
            if (pup.snake.apples % 8 == 0) {
                tmp = pup.snake.apples/8;
                pup.snake.speed = 6 - tmp;
            }
        }
        update_score();
        if (!pup.type)  {
            new_apple();
        } else if (pup.snake.apples == 24) {
            open_door();
        }
    } else if (ch_go_to == 80 || ch_go_to == 103 || ch_go_to == 122) {
        return ACTION_SNAKE_LEVELUP;
    } else if (ch_go_to != EMPTY) {
        return ACTION_SNAKE_DEAD;
    }

    // set the change of direction    
    POKE(COLOR_RAM + pup.snake.head, COLOR_GREEN);
    POKE(VIDEO_MEMORY + pup.snake.head, pup.snake.body_chr);
    if (pup.snake.body_chr!= SNAKE_BODY) {
        pup.snake.body_chr = SNAKE_BODY;
    } 

    //move head
    pup.snake.head = go_to;
    POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
    POKE(COLOR_RAM + pup.snake.head, COLOR_GREEN);

    if (pup.snake.grow > 0) {
        --pup.snake.grow;
        return ACTION_SNAKE_NOTHING;
    }

    POKE(VIDEO_MEMORY + pup.snake.tail, 32);
    pup.snake.tail += pup.snake.tail_direction;
    ch_go_to = PEEK(VIDEO_MEMORY + pup.snake.tail);

    if (pup.snake.tail_direction == SNAKE_RIGHT) {
        if (ch_go_to == 126) {
            pup.snake.tail_direction = SNAKE_UP;
        } else if (ch_go_to == 123) {
            pup.snake.tail_direction = SNAKE_DOWN;
        }
    } else if (pup.snake.tail_direction == SNAKE_UP) {
        if (ch_go_to == 108) {
            pup.snake.tail_direction = SNAKE_RIGHT;
        } else if (ch_go_to == 123) {
            pup.snake.tail_direction = SNAKE_LEFT;
        }
    } else if (pup.snake.tail_direction == SNAKE_LEFT) {
        if (ch_go_to == 124) {
            pup.snake.tail_direction = SNAKE_UP;
        } else if (ch_go_to == 108) {
            pup.snake.tail_direction = SNAKE_DOWN;
        }
    } else if (pup.snake.tail_direction == SNAKE_DOWN) {
        if (ch_go_to == 124) {
            pup.snake.tail_direction = SNAKE_RIGHT;
        } else if (ch_go_to == 126) {
            pup.snake.tail_direction = SNAKE_LEFT;
        }
    }

    POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

    return ACTION_SNAKE_NOTHING;

}

void init_level(void) {
    unsigned char x = 4;
    if (pup.type)
        x = 24;

    clrscr();
    pup.level_arr = levels[ (pup.level-1) % (sizeof(levels)/2)];
    build_level();
    if (pup.type)
        update_level();

    pup.snake.head = 530;
    pup.snake.tail = 529;
    pup.snake.grow = 0;
    pup.snake.direction = SNAKE_RIGHT;
    pup.snake.tail_direction = SNAKE_RIGHT;
    pup.snake.updated = frames; 
    pup.snake.speed = 6;
    pup.snake.apples = 0;

    POKE(VIDEO_MEMORY + pup.snake.head, SNAKE_HEAD);
    POKE(VIDEO_MEMORY + pup.snake.tail, SNAKE_TAIL);

    for (; x > 0; --x) {
        new_apple();
    }

    update_score();
    update_lives();
}

void game_intro(void) {

    clrscr();
    cputsxy(12,4,"-=charsnake64=-");
    cputsxy(6,16,"select type of challenge and");
    cputsxy(6,18,"press fire on joy 2 to start");

    for (;;) {
        char fat = joy_read(JOY_2);
        revers(pup.type == 0);
        cputsxy(14,9," eat or die ");
        revers(pup.type == 1);
        cputsxy(11,11," climb the tower ");
        if (pup.type == 1 && JOY_UP(fat)) {
            pup.type = 0;
        }
        if (pup.type == 0 && JOY_DOWN(fat)) {
                pup.type = 1;
        }
        if (JOY_BTN_1(fat)) {
            revers(0);
            return;
        }
    }
}


void control_snake(void) {
        char fat = joy_read(JOY_2);

        if (pup.snake.direction == SNAKE_RIGHT) {
            if (JOY_UP(fat)) {
                pup.snake.body_chr = 126;
                pup.snake.direction = SNAKE_UP;
                return;
            }
            if (JOY_DOWN(fat)) {
                pup.snake.body_chr = 123;
                pup.snake.direction = SNAKE_DOWN;
                return;
            }
        } else if (pup.snake.direction == SNAKE_LEFT) {
            if (JOY_UP(fat)) {
                pup.snake.body_chr = 124;
                pup.snake.direction = SNAKE_UP;
                return;
            }
            if (JOY_DOWN(fat)) {
                pup.snake.body_chr = 108;
                pup.snake.direction = SNAKE_DOWN;
                return;
            }
        } else if (pup.snake.direction == SNAKE_DOWN) {
            if (JOY_RIGHT(fat)) {
                pup.snake.body_chr = 124;
                pup.snake.direction = SNAKE_RIGHT;
                return;
            }
            if (JOY_LEFT(fat)) {
                pup.snake.body_chr = 126;
                pup.snake.direction = SNAKE_LEFT;
                return;
            }
        } else if (pup.snake.direction == SNAKE_UP) {
            if (JOY_RIGHT(fat)) {
                pup.snake.body_chr = 108;
                pup.snake.direction = SNAKE_RIGHT;
                return;
            }
            if (JOY_LEFT(fat)) {
                pup.snake.body_chr = 123;
                pup.snake.direction = SNAKE_LEFT;
                return;
            }
        }

}

void sleep(char cicles) {
    int end = frames + cicles;
    do {} while (frames != end);
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
            frames = 0;
            sleep(30);
            if (pup.lives == 0) {
                return;
            }
            init_level();
        }
    }
}

int main(void) {
#if (__C64__)
    VIC.bordercolor = COLOR_BLACK;
    VIC.bgcolor0 = COLOR_BLACK;
    VIC.addr = 0x15;
#elif (__C16__)
    TED.bordercolor = COLOR_BLACK;
    TED.bgcolor = COLOR_BLACK;
    TED.char_addr =  ((0xD000 >> 8) & 0xFC);
    POKE(0x053B, COLOR_WHITE);
#endif
    raster_irq();

    joy_install (joy_static_stddrv);
    for (;;) {
        game_intro();
        game_play();
    }

    return 0;
}
