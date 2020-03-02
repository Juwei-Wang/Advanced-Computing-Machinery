#include <stdio.h>

void func(int *n, int *m) {
	*n = 8;
	*m = 9;
}

int main() {
	int n = 0, m = 0;
	func(&n,&m);
	printf("%d, %d", n, m);	
}
