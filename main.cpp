#include <stdio.h>

#include "view.h"

int main()
{
	if (!view_init()) return 1;
	while (view_update());
	view_end();

    return 0;
}
