#ifndef CHIP_H
#  define CHIP_H

#  include <stdlib.h>
#  include <stdio.h>
#  include <string.h>
#  include <time.h>
#  include <unistd.h>
#  include "kbhit.h"
#  include "pixel_operations.h"

#  define IGNORE_UNUSED(x) { x = x; }

#  define SCREEN_EXPANSION 10
#  define GAME_DIRECTORY   "games/"
#  define SCREEN_HEIGHT    32 
#  define SCREEN_WIDTH     64
#  define WARNING          "\033[33;1m[WARNING]\033[0m"
#  define ERROR            "\033[31;1m[ERROR  ]\033[0m"
#  define DEBUG            "\033[35;1m[DEBUG  ]\033[0m"
#  define WHITE            255
#  define DARK             0

struct chip
{
    /*
     0x0000 - 0x01FF = CHIP-8 Interpreter
     0x0050 - 0x00A0 = 4x5 pixel font size
     0x0200 - 0x0E9F = Stuff
     0x0EA0 - 0x0EFF = Call stack, internal use, variables
     0x0F00 - 0x0FFF = Display refresh
     */
    
    size_t draw_flag;
    
    unsigned short I;          // instructions pointer
    unsigned short pc;         // program counter
    unsigned short sp;         // stack pointer
    unsigned short opcode;
    unsigned short stack[16];
    
    SDL_Surface *screen;
    
    unsigned char V[16];      // V0-VF regs
    unsigned char keys[16];   // keypad
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned char memory[4096];
    unsigned char gfx[SCREEN_WIDTH *SCREEN_HEIGHT];
};

void clear_screen(struct chip *chip);

void load_fontset(struct chip *chip);
void setup_graphics(struct chip *chip);
void setup_input(struct chip *chip);
void init_chip(struct chip *chip);
int load_game(struct chip *chip, char *game);

unsigned short fetch_opcode(struct chip *chip);
void decode_opcode(struct chip *chip, unsigned short opcode);
int emulate_cycle(struct chip *chip);
void update_graphics(struct chip *chip);
void set_keys(struct chip *chip);
// void key_down(unsigned char key, int x, int y);
// void key_up(unsigned char key, int x, int y);

#endif /* CHIP_H */
