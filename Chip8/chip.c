#include "chip.h"

void load_fontset(struct chip *chip)
{
    unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
    for (int i = 0; i < 80; ++i)
        chip->memory[i] = chip8_fontset[i];
    // chip->memory[0x50 + i] = chip8_fontset[i];
    
    printf("%s Font loaded.\n", DEBUG);
}

void clear_screen(struct chip *chip)
{
    memset(chip->gfx, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    update_graphics(chip);
}

void setup_graphics(struct chip *chip)
{
    chip->screen = create_screen(SCREEN_WIDTH * SCREEN_EXPANSION, SCREEN_HEIGHT * SCREEN_EXPANSION);
    display_image(chip->screen);
    printf("%s Graphics set up.\n", DEBUG);
}

void setup_input(struct chip *chip)
{
    // Create buttons
    chip->keys[0] = chip->keys[0] + 1 - 1;; //TMP
    printf("%s Input set up.\n", DEBUG);
}

void init_chip(struct chip *chip)
{
    // Initialize the chip
    
    chip->pc = 0x200;
    chip->opcode = 0;
    chip->I = 0;
    chip->sp = 0;
    
    clear_screen(chip);
    
    memset(chip->V, 0, 16);
    memset(chip->keys, 0, 16);
    memset(chip->stack, 0, 16);
    memset(chip->memory, 0, 4096);
    
    load_fontset(chip);
    
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    chip->draw_flag = 1;
    
    printf("%s Chip initialized.\n", DEBUG);
}

int load_game(struct chip *chip, char *game)
{
    printf("%s Loading \033[34;1m%s\033[0m\n", DEBUG, game);
    char *game_dir = malloc(strlen(GAME_DIRECTORY) + strlen(game) + 1);
    strcpy(game_dir, GAME_DIRECTORY);
    strcat(game_dir, game);
    FILE *rom = fopen(game_dir, "rb");
    if (!rom)
    {
        printf("%s Couldn't open file\n", ERROR);
        exit(-1);
    }
    
    fseek(rom, 0L, SEEK_END);
    size_t sz = ftell(rom);
    fseek(rom, 0L, SEEK_SET);
    printf("%s size of rom: %lu bytes\n", DEBUG, sz);
    
    char *buffer = (char *)malloc(sizeof (char) *sz);
    if (!buffer)
    {
        printf("%s Couldn't allocate buffer\n", ERROR);
        exit(-1);
    }
    
    size_t result = fread(buffer, 1, sz, rom);
    if (result != sz)
    {
        printf("%s while reading rom\n", ERROR);
        exit(-1);
    }
    
    if ((4096 - 512) > sz)
        for (size_t i = 0; i < sz; ++i)
            chip->memory[i + 512] = buffer[i];
    else
    {
        printf("%s ROM too big \n", ERROR);
        exit(-1);
    }
    
    fclose(rom);
    free(buffer);
    return 0;
}

void update_graphics(struct chip *chip)
{
    for(int i = 0; i < SCREEN_WIDTH; ++i)
    {
        for (int j = 0; j < SCREEN_HEIGHT; ++j)
        {
            // printf("{%d, %d, (%d)} = %d\n", i, j, index, chip->gfx[index]);
            int index = i + j * SCREEN_WIDTH;
            Uint32 pixel = chip->gfx[index]
            ? SDL_MapRGB(chip->screen->format, WHITE, WHITE, WHITE)
            : SDL_MapRGB(chip->screen->format, DARK, DARK, DARK);
            for (int k = 0; k < SCREEN_EXPANSION; ++k)
                for (int l = 0; l < SCREEN_EXPANSION; ++l)
                    putpixel(chip->screen, i * SCREEN_EXPANSION + k, j * SCREEN_EXPANSION + l, pixel);
        }
    }
    display_image(chip->screen);
    chip->draw_flag = 0;
}

int emulate_cycle(struct chip *chip)
{
    unsigned short op = fetch_opcode(chip);
    decode_opcode(chip, op);
    return (int)chip->draw_flag;
}

unsigned short fetch_opcode(struct chip *chip)
{
    return chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
}

void decode_opcode(struct chip *chip, unsigned short opcode)
{
    // printf("%s opcode: %04X\n", DEBUG, opcode);
    switch(opcode & 0xF000)
    {
        case 0x0000:
            switch(opcode & 0x00FF)
        {
            case 0x00E0:
                // Clears the screen.
                clear_screen(chip);
                chip->pc += 2;
                break;
                
            case 0x00EE:
                // return from subroutine
                chip->pc = chip->stack[--chip->sp] + 2;
                break;
                
            case 0x0000:
                printf("%s Invalid instruction (opcode 0x%04X)\n", ERROR, opcode);
                exit(-1);
                break;
                
            default:
                printf("%s RCA_1802 (not implemented)\n", ERROR);
                // rca_1802(chip, (opcode & 0xFFF));
                chip->pc += 2;
                break;
        }
            break;
        case 0x1000:
            // 1NNN Jumps to address NNN.
            chip->pc = (opcode & 0x0FFF);
            break;
            
        case 0x2000:
            // 2NNN Calls subroutine at NNN.
            chip->stack[chip->sp] = chip->pc;
            ++chip->sp;
            chip->pc = (opcode & 0x0FFF);
            break;
            
        case 0x3000:
            // 3XNN Skips the next instruction if VX equals NN.
            if (chip->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                chip->pc += 4;
            else
                chip->pc += 2;
            break;
            
        case 0x4000:
            // 4XNN Skips the next instruction if VX doesn't equal NN.
            if (chip->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                chip-> pc += 4;
            else
                chip->pc += 2;
            break;
            
        case 0x5000:
            // 5XY0 Skips the next instruction if VX equals VY.
            if (chip->V[(opcode & 0x0F00) >> 8] == chip->V[(opcode & 0x00F0) >> 4])
                chip->pc += 4;
            else
                chip->pc += 2;
            break;
            
        case 0x6000:
            // 6XNN Sets VX to NN.
            chip->V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            chip->pc += 2;
            break;
            
        case 0x7000:
            // 7XNN Adds NN to VX.
            chip->V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            chip->pc += 2;
            break;
            
        case 0x8000:
            switch((opcode & 0xF))
        {
            case 0:
                // 8XY0 Sets VX to the value of VY.
                chip->V[(opcode & 0x0F00) >> 8] = chip->V[(opcode & 0x00F0) >> 4];
                chip->pc += 2;
                break;
                
            case 1:
                // 8XY1 Sets VX to VX or VY.
                chip->V[(opcode & 0x0F00) >> 8] |= chip->V[(opcode & 0x00F0) >> 4];
                chip->pc += 2;
                break;
                
            case 2:
                // 8XY2 Sets VX to VX and VY.
                chip->V[(opcode & 0x0F00) >> 8] &= chip->V[(opcode & 0x00F0) >> 4];
                chip->pc += 2;
                break;
                
            case 3:
                // 8XY3 Sets VX to VX xor VY.
                chip->V[(opcode & 0x0F00) >> 8] ^= chip->V[(opcode & 0x00F0) >> 4];
                chip->pc += 2;
                break;
                
            case 4:
                // 8XY4 Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
                if (chip->V[(opcode & 0x00F0) >> 4] > (0xFF - chip->V[(opcode & 0x0F00) >> 8]))
                    chip->V[0xF] = 1;
                else
                    chip->V[0xF] = 0;
                chip->V[(opcode & 0x0F00) >> 8] += chip->V[(opcode & 0x00F0) >> 4];
                chip->pc += 2;
                break;
                
            case 5:
                // 8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                if (chip->V[(opcode & 0x00F0) >> 4] > chip->V[(opcode & 0x0F00) >> 8])
                    chip->V[0xF] = 0;
                else
                    chip->V[0xF] = 1;
                chip->V[(opcode & 0x0F00) >> 8] -= chip->V[(opcode & 0x00F0) >> 4];
                chip->pc += 2;
                break;
                
            case 6:
                // 8XY6 Shifts VX >> 1. VF is set to the value of the least significant bit of VX before the shift.
                chip->V[0xF] = (chip->V[(opcode & 0x0F00) >> 8] & 0x1);
                chip->V[(opcode & 0x0F00) >> 8] >>= 1;
                chip->pc += 2;
                break;
                
            case 7:
                // 8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                if (chip->V[(opcode & 0x0F00) >> 8] > chip->V[(opcode & 0x00F0) >> 4])
                    chip->V[0xF] = 0;
                else
                    chip->V[0xF] = 1;
                chip->V[(opcode & 0x0F00) >> 8] = chip->V[(opcode & 0x00F0) >> 4] - chip->V[(opcode & 0x0F00) >> 8];
                chip->pc += 2;
                break;
                
            case 0xE:
                // 8XYE Shifts VX << 1. VF is set to the value of the most significant bit of VX before the shift
                chip->V[0xF] = (chip->V[(opcode & 0x0F00) >> 8] >> 7);
                chip->V[(opcode & 0x0F00) >> 8] <<= 1;
                chip->pc += 2;
                break;
                
            default:
                printf("%s Invalid instruction (opcode 0x%04X)\n", ERROR, opcode);
                exit(-1);
        }
            break;
            
        case 0x9000:
            // 9XY0 Skips the next instruction if VX doesn't equal VY.
            if (chip->V[(opcode & 0x0F00) >> 8] != chip->V[(opcode & 0x00F0) >> 4])
                chip->pc += 4;
            else
                chip->pc += 2;
            break;
            
        case 0xA000:
            // ANNN Sets I to the address NNN.
            chip->I = (opcode & 0xFFF);
            chip->pc += 2;
            break;
            
        case 0xB000:
            // BNNN Jumps to the address NNN plus V0.
            chip->pc = chip->V[0] + (opcode & 0x0FFF);
            break;
            
        case 0xC000:
            // CXNN Sets VX to the result of a bitwise and operation on a random number and NN.
            chip->V[(0x0FFF & opcode) >> 8] = ((rand() % 0x00FF) & (opcode & 0x00FF));
            chip->pc += 2;
            break;
            
        case 0xD000:
        {
            /* Sprites stored in memory at location in index register (I), 8bits wide.
             *Wraps around the screen.
             *If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero.
             *All drawing is XOR drawing (i.e. it toggles the screen pixels).
             *Sprites are drawn starting at position VX, VY.
             *N is the number of 8bit rows that need to be drawn.
             *If N is greater than 1, second line continues at position VX, VY+1, and so on.
             */
            unsigned short x = (opcode & 0x0F00) >> 8;
            unsigned short y = (opcode & 0x00F0) >> 4;
            unsigned short length = (opcode & 0x000F);
            unsigned short pixel;
            
            chip->V[0xF] = 0;
            for (int j = 0; j < length; j++)
            {
                pixel = chip->memory[chip->I + j];
                for(int i = 0; i < 8; i++)
                {
                    if((pixel & (0x80 >> i)) != 0)
                    {
                        if(chip->gfx[(x + i + ((y + j) * SCREEN_WIDTH))] == 1)
                        {
                            chip->V[0xF] = 1;
                        }
                        chip->gfx[x + i + ((y + j) * SCREEN_WIDTH)] ^= 1;
                    }
                }
            }
            chip->draw_flag = 1;
            chip->pc += 2;
        }
            break;
            
        case 0xE000:
            switch(opcode & 0x00FF)
        {
            case 0x009E:
                // Skips the next instruction if the key stored in VX is pressed.
                if (chip->keys[chip->V[(opcode & 0x0FFF) >> 8]] == 1)
                    chip->pc += 4;
                else
                    chip->pc += 2;
                break;
                
            case 0x00A1:
                // Skips the next instruction if the key stored in VX isn't pressed.
                if (chip->keys[chip->V[(opcode & 0x0FFF) >> 8]] == 0)
                    chip->pc += 4;
                else
                    chip->pc += 2;
                break;
                
            default:
                printf("%s Invalid instruction (opcode 0x%04X)\n", ERROR, opcode);
                exit(-1);
                break;
        }
            break;
            
        case 0xF000:
            switch(opcode & 0x00FF)
        {
            case 0x0007:
                // FX07 Sets VX to the value of the delay timer.
                chip->V[(opcode & 0x0F00) >> 8] = chip->delay_timer;
                chip->pc += 2;
                break;
                
            case 0x000A:
            {
                // FX0A A key press is awaited, and then stored in VX.
                int key_pressed = 0;
                for (int i = 0; i < 16; ++i)
                {
                    if (chip->keys[i] != 0) 
                    {
                        chip->V[(opcode & 0x0F00) >> 8] = i;
                        key_pressed = 1;
                    }
                }
                if (!key_pressed)
                    return;
                
                printf("%s Key pressed (keys[%d])\n", DEBUG, chip->V[(opcode & 0x0F00) >> 8]);
                chip->pc += 2;
            }
                break;
                
            case 0x0015:
                // FX15 Sets the delay timer to VX.
                chip->delay_timer = chip->V[(opcode & 0x0F00) >> 8];
                chip->pc += 2;
                break;
                
            case 0x0018:
                // FX18 Sets the sound timer to VX.
                chip->sound_timer = chip->V[(opcode & 0x0F00) >> 8];
                chip->pc += 2;
                break;
                
            case 0x001E:
                // FX1E Adds VX to I.
                if (chip->I + chip->V[(opcode & 0x0F00) >> 8] > 0xFFF)
                    chip->V[0xF] = 1;
                else
                    chip->V[0xF] = 0;
                
                chip->I += chip->V[(opcode & 0x0F00) >> 8];
                chip->pc += 2;
                break;
                
            case 0x0029:
                // Sets I to the loc of the sprite for the char in VX. Characters 0-F are represented by a 4x5 font.
                chip->I = chip->V[(opcode & 0x0F00) >> 8] * 0x5;
                chip->pc += 2;
                break;
                
            case 0x0033:
                // Stores the Binary-coded decimal representation of VX,
                // with the most significant of three digits at the address in I,
                // the middle digit at I plus 1, and the least significant digit at I plus 2.
                chip->memory[chip->I]     = (chip->V[(opcode & 0x0F00) >> 8] / 100);
                chip->memory[chip->I + 1] = (chip->V[(opcode & 0x0F00) >> 8] / 10) % 10;
                chip->memory[chip->I + 2] = (chip->V[(opcode & 0x0F00) >> 8] % 100) % 10;
                chip->pc += 2;
                break;
                
            case 0x0055:
                // FX55 Stores V0 to VX in memory starting at address I
                for (int i = 0; i < (((opcode & 0x0F00) >> 8) + 1); ++i)
                    chip->memory[chip->I + i] = chip->V[i];
                chip->pc += 2;
                // On the original interpreter, when the operation is done, I = I + X + 1.
                // chip->I += ((opcode & 0x0F00) >> 8) + 1;
                break;
                
            case 0x0065:
                // FX65 Fills V0 to VX with values from memory starting at address I
                for (int i = 0; i < (((opcode & 0x0F00) >> 8) + 1); ++i)
                    chip->V[i] = chip->memory[chip->I + i];
                chip->pc += 2;
                // On the original interpreter, when the operation is done, I = I + X + 1.
                // chip->I += ((opcode & 0x0F00) >> 8) + 1;
                break;
                
            default:
                printf("%s Invalid instruction (opcode 0x%04X)\n", ERROR, opcode);
                exit(-1);
                break;
        }
            break;
            
        default:
            printf("%s Invalid instruction (opcode 0x%04X)\n", ERROR, opcode);
            exit(-1);
    }
    
    if (chip->delay_timer > 0)
        --chip->delay_timer;
    
    if (chip->sound_timer > 0)
    {
        if (chip->sound_timer == 1)
            printf("%s BEEP\n", DEBUG);
        --chip->sound_timer;
    }
}

void set_keys(struct chip *chip)
{
    for (int i = 0; i < 16; ++i)
        chip->keys[i] = 0;

    int c;
    if(!kbhit())
        return;

    c = getchar();
    switch(c)
    {
        case 'x':
            chip->keys[0] = 1;
        case '1':
            chip->keys[1] = 1;
        case '2':
            chip->keys[2] = 1;
        case '3':
            chip->keys[3] = 1;

        case 'q':
            chip->keys[4] = 1;
        case 'w':
            chip->keys[5] = 1;
        case 'e':
            chip->keys[6] = 1;
        case 'a':
            chip->keys[7] = 1;

        case 's':
            chip->keys[8] = 1;
        case 'd':
            chip->keys[9] = 1;
        case 'z':
            chip->keys[0xA] = 1;
        case 'c':
            chip->keys[0xB] = 1;

        case '4':
            chip->keys[0xC] = 1;
        case 'r':
            chip->keys[0xD] = 1;
        case 'f':
            chip->keys[0xE] = 1;
        case 'v':
            chip->keys[0xF] = 1;
    }
}

int rca_1802(struct chip *chip, unsigned char opcode)
{
    return chip->draw_flag *opcode;  //TMP
}
