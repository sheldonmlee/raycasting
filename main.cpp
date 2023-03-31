#include <stdio.h>

#include "view.h"

int main()
{
	view_init();
	while (view_update());
	view_end();

    return 0;
}
