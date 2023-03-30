#include <stdio.h>

#include "view.h"

int main()
{
	printf("Hello meme!\n");

	view_init();
	while (view_update());

    return 0;
}
