#include "bagelboy.h"

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    struct bagelboy bagelboy;
    bagelboy_initialize(&bagelboy, "../bagel-boy/roms/Tetris (World).gb");

    while (bagelboy.power)
    {
        bagelboy_emulate(&bagelboy);
    }

    bagelboy_destroy(&bagelboy);

    return 0;
}