#include <stdio.h>
void main() {
	long int n, r, sum=0;
		
	void numToLetter(long int n){
		switch (n) {
			case 9: printf("Nine"); break;
			case 8: printf("Eight"); break;
			case 7: printf("Seven"); break;
			case 6: printf("Six"); break;
			case 5: printf("Five"); break;
			case 4: printf("Four"); break;
			case 3: printf("Three"); break;
			case 2: printf("Two"); break;
			case 1: printf("One"); break;
			case 0: printf("Zero"); break;
		}
	}
	
	
	printf("Enter a non-negative number:");
	scanf("%ld", &n);
	if (n < 0) {printf("Number must be non-negative!"); return;}
	   
    
    while(n > 0) {    
    	r = n % 10;    
    	sum = sum * 10 + r;    
    	n = n / 10;    
    }   
	
    n = sum;    
    while(n > 0) {    
    	r = n % 10;    
    	numToLetter(r);
		printf(" ");
        n = n / 10;    
    }    
    		
}