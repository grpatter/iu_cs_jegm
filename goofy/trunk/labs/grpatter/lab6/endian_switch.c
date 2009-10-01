
#include <stdio.h> 
#include <stdlib.h> 
main(int argc, char *argv[])
{
	unsigned int x = 0xE0843005;
	printf("x is:%ud\n",x);
	x = endian_swap(x);
	printf("x is:%ud\n",x);
	
	inline void endian_swap(unsigned int& x)
	{
		x = (x>>24) | 
			((x<<8) & 0x00FF0000) |
			((x>>8) & 0x0000FF00) |
			(x<<24);
	}

}



void do_work(unsigned int x){
char *regnames[] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","sp","lr","pc"};
char *opcodes[] = {"AND","EOR","SUB","RSB","ADD","ADC","SBC","RSC","TST","TEQ","CMP","CMN","ORR","MOV","BIC","MVN"};
char *cndcodes[] = {"EQ","NE","CS/HS","CC/LO","MI","PL","VS","VC","HI","LS","GE","LT","GT","LE","{AL}"};
char *shftcodes[] = {"LSL ","","","","","","","","","","",""};
//check 25th bit
//if(b_25 == 0){
	//check bit 4
//	if(b_4 == 0){
		//do top option on b_25
//	}else{
		//lower section
//	}
//}else{
	//immediate shift
//}
}
