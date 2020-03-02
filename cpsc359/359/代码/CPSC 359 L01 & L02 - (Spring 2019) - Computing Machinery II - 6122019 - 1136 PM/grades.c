#include <stdio.h>
 
int main () {
	int m;
	printf("Enter your mark:");
	scanf("%d",&m);
	if ( m > 100 || m < 0) { printf("Incorrect mark."); return 1;}
	else if (m >= 95) printf("A+");
	else if (m >= 90) printf("A");
	else if (m >= 85) printf("A-");
	else if (m >= 80) printf("B+");
	else if (m >= 75) printf("B");
	else if (m >= 70) printf("B-");
	else if (m >= 65) printf("C+");
	else if (m >= 60) printf("C");
	else if (m >= 55) printf("C-");
	else if (m >= 50) printf("D+");
	else printf("F");
	
	return 0;
}