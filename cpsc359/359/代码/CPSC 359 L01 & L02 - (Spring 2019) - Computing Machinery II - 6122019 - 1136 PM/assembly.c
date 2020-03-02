#include <stdio.h>

int main() {

    int n1, n2, add, sub, mul, quo, rem ;

    printf( "Enter two integers:" );
    scanf( "%d%d", &n1, &n2 );

    __asm__ ( "addl %%ebx, %%eax;" : "=a" (add) : "a" (n1) , "b" (n2) );
	 printf( "%d + %d = %d\n", n1, n2, add );
	 
    __asm__ ( "subl %%ebx, %%eax;" : "=a" (sub) : "a" (n1) , "b" (n2) );
	 printf( "%d - %d = %d\n", n1, n2, sub );
	 
    __asm__ ( "imull %%ebx, %%eax;" : "=a" (mul) : "a" (n1) , "b" (n2) );
	printf( "%d * %d = %d\n", n1, n2, mul );

    return 0 ;
}