#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define DUMBCOPY for(i = 0; i < 65536; i++) \
		destination[i] = source[i];

#define SMARTCOPY i=0; \
		memmove(destination, source, 65536);


int main(void)
{
	char source[65536], destination[65536];
	int i, j;
	for(j = 0; j < 100; j++)
		SMARTCOPY;
	return -1;
}