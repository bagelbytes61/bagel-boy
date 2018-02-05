#include "bagelboy.h"

int main(void)
{
	struct bagelboy* bagelboy = bagelboy_create("roms\\11-op a,(hl).gb");

	while (bagelboy->power)
	{
		bagelboy_emulate(bagelboy);
	}

	bagelboy_destroy(bagelboy);

	return 0;
}