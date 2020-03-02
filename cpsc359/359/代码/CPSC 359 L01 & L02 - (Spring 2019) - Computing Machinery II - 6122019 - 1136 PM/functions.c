#include <stdio.h>
 
void main () {
	int isEven(long int n) {
		return !(n % 2);
	}
	
	int isOdd(long int n) {
		return (n % 2);
	}
	
	int max(long int n1, long int n2) {
				
		if (n1 > n2)
      		return n1;
   		 
   		return n2; 
	}
}