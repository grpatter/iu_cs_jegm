/*    /u/c335/labs/lab2/print.c    */
#include <stdarg.h>

extern int printchar(int);

void printstring(char *s){
}

void printint(int i){
  if(i < 0){
    putchar('-');
    printinthelp(i*-1);
  }else{
    printinthelp(i);
  }
}

void printinthelp(int i){
  if(i < 10){
    int digit = i;
    digit = digit + '0';
    putchar(digit);
  }else{
  int digit = i % 10;
  digit = digit + '0';
  printinthelp(i/10);
  putchar(digit);
  }
}

void printhex(int i){
}


void myprintf(const char *fmt, ...)
{
  const char *p;
  va_list argp;
  int i;
  char *s;
  char fmtbuf[256];

  va_start(argp, fmt);

  for(p = fmt; *p != '\0'; p++)
    {
      if(*p != '%')
        {
          putchar(*p);
          continue;
        }

      switch(*++p)
        {
        case 'c':
          i = va_arg(argp, int);
          putchar(i);
          break;

        case 'd':
          i = va_arg(argp, int);
          printint(i);
          break;

        case 's':
          s = va_arg(argp, char *);
          printstring(s);
          break;

        case 'x':
          i = va_arg(argp, int);
          printhex(i);
          break;

        case '%':
          putchar('%');
          break;
        }
    }

  va_end(argp);
}
