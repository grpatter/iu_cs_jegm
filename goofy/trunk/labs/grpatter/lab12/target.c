#include <stdio.h>

void gg_init(int);

void never_called()
{
	while(1)
	puts("Hijacked !\n");
}

char *target_procedure(char *c)
{
	char buf[8];
	return gets(buf);
}

int main()
{
	char buf[64];
	puts("Welcome to Goofy Giggles\n");
	target_procedure(buf);
	return 1;
}
