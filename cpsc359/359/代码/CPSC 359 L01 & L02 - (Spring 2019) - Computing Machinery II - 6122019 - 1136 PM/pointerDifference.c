#include <stdio.h>
void main() {
	int n[5],i,*p1,*p2;
	
	p1 = n;
	for (i=0; i<5; i++) {
		p2 = &n[i];
		printf("The difference is %d\n", p2-p1);
	}
}