#define N 0x10000000
#include <stdio.h>

union A{
	float f;
	unsigned int i;
};

struct bitfield{
	unsigned int rt:6;
	unsigned int rs:6;
	unsigned int rd:6;
	unsigned int  j:1;
	unsigned int dm:1;
	unsigned int out:1;
	unsigned int in:1;
	unsigned int st:1;
	unsigned int ld:1;
	unsigned int  f:1;
	unsigned int op:4;
	unsigned int  b:1;
	unsigned int it:1;
	unsigned int is:1;
};

union B{
	unsigned int inst;
	struct bitfield bf;
};

int check_sgn(unsigned int n, int size){
	unsigned int msb = (1<<(size-1));
	if (n&msb) n-= (msb<<1);
	return n;
}

int main(){
	//int k = N;
	union A tmp;
	tmp.i = 0xbf800000;

	struct bitfield inst ;
	printf("%ld\n",sizeof(inst));
	union B x;
	x.inst =  0x0f000000;
	int y = 0xffffffff;
	printf("max-size = %d\n",y);
	printf("%d\n",x.bf.is);
	printf("%d\n",x.bf.it);
	printf("%d\n",x.bf.b);
	printf("%d\n",x.bf.op);
	printf("%d\n",x.bf.f);
	printf("%d\n",x.bf.ld);
	printf("%d\n",x.bf.st);
	printf("%d\n",x.bf.in);
	printf("%d\n",x.bf.out);
	printf("%d\n",x.bf.dm);
	printf("%d\n",x.bf.j);
	printf("%d\n",x.bf.rd);
	printf("%d\n",x.bf.rs);
	printf("%d\n",x.bf.rt);

	int a = 0b1111111;
	a = check_sgn(a,7);
	printf("sgn-> %d\n",a);

	//printf("%d %f\n",tmp.i,tmp.f);

	return 0;
}
