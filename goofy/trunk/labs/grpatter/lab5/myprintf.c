/*    /u/c335/labs/lab2/print.c    */
#include <stdarg.h>

extern int printchar(int);

void printstring(char *s){
  while(* s != '\0'){
    putchar(*s);
    *++s;
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
void printint(int i){
  if(i < 0){
    putchar('-');
    printinthelp(i*-1);
  }else{
    printinthelp(i);
  }
}

void printhex(int i){
  int val;
  if(i < 0){
    putchar('-');
    printhex(i*-1);
  }else if(i < 16){
    putchar('0');
    putchar('x');
    if(i < 10){
      putchar(i+'0');
    }else{
      putchar(i+'7');
    }
  }else{
    val = i%16;
    if(val < 10){
      val = val + '0';
    }else{
      val = val + '7';
    }
    printhex(i/16);
    putchar(val);
  }
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

