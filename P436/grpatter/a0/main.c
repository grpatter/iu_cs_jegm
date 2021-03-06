//Greg Patterson
//grpatter at indiana.edu
#include "main.h"

int main()
{
  a b;
  void (*f[2]) (char*, char*);
  int (*f2) (char*, char) = &find;
  int c;
  int c2;
  int d = 0;
  int e = 0;
  int stringCount = 2;
  char** strings = (char**) malloc(sizeof(char*) * stringCount);
  f[0] = &copy;
  f[1] = &ypoc;

  for (c = 0; c < stringCount; c++)
  {
    strings[c] = (char*) malloc(sizeof(char) * 2 * SIZE);
  }

  for (c = 0; c < stringCount; c++)
  {
    switch (c)
    {
      case 0:
        f[c](strings[c], "Hello Dave");
      break;
      case 1:
        f[c](strings[c], "What in the world");
      break;
    }
  }

  for (c = 0; c < stringCount; c++)
  {
    d = f2(strings[c], (char) 32);
    strings[c][d + 1] = 0;
    f[c](&b.array[e], strings[c]);
	
    // Set e = length of prev string so we know 
	//where 'world' will go.  -1 removes extra space.
    e = strlen(strings[c]) - 1;
  }
  
  printf("%s\n", b.array);

  return 0;
}

int find(char* src, char t)
{
  int c;
  int len = strlen(src);

  for (c = 0; c < len; c++)
    if (src[c] == t)
      return c;

  return -1;
}

void copy(char* into, char* from)
{
  int c;
  int len = strlen(from);

  for (c = 0; c < len; c++)
  {
    into[c] = from[c];
  }
}

void ypoc(char* into, char* from)
{
  int c;
  int len = strlen(from);

  for (c = 0; c < len; c++)
  {
    into[len - c - 1] = from[c];
  }
  
  // Add terminating \0
  into[len] = 0;
}
