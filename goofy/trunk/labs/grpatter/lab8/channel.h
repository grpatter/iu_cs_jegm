// channel.h
#define Full(c) (c != -1)
#define Empty(c) (c == -1)
#define CONCAT2(arg1, arg2) arg1 ## arg2
#define CONCAT(arg1, arg2) CONCAT2(arg1, arg2)
#define Begin() static void *pc = 0; if (pc) goto *(pc);
#define Return(x) \
do { \
pc = CONCAT(&&_L, __LINE__ ); \
return x; \
CONCAT (_L, __LINE__) :; \
} while (0)
#define Finish() return 0;
#define Send(ch,v) while(Full(ch)) Return(1); ch = v
#define Receive(ch,v) while(Empty(ch)) Return(1); v = ch; ch = -1
