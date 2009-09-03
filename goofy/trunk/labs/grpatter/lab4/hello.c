#include <bsp/ggbsp.h>
int main()
{
char hello[] = "hello world\r\n";
int i;
gg_init(1);
while (1)
{
/* print string */
for(i = 0; hello[i]; i++)
putchar(hello[i]);
}
}
