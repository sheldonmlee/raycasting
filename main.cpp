#include <stdio.h>

#include "view.h"

int main()
{
	view_init();
	while (view_update());

    return 0;
}
