#include "chip.h"


int main(int argc, char *argv[])
{
    IGNORE_UNUSED(argc);
    srand(time(NULL));
    struct chip *chip = malloc(sizeof (struct chip));
    
    char *game = argv[1] ? argv[1] : "pong";
    setup_graphics(chip);
    setup_input(chip);
    
    init_chip(chip);
    if(load_game(chip, game))
        return -1;

    int tmp = 200000 / SCREEN_EXPANSION;
    // int tmp = 0;

    printf("%s Launching emulation... (%d iterations)\n\n", DEBUG, tmp ? tmp : -1);
    while (--tmp)
    {
        int draw = emulate_cycle(chip);
        if (draw)
            update_graphics(chip);
        // set_keys(chip);
    }
    return 0;
}
