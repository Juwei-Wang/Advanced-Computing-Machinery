#include <stdio.h>
void main() {
	int n;
	char fname[20], lname[20];
	float cost;
	printf("Enter your name:");
	scanf("%s %s",fname,lname);
	printf("How many items are there?");
	scanf("%d",&n);
	printf("What is the unit cost?");
	scanf("%f",&cost);
	printf("\nHey %s %s, \nThere are %d items at $%4.2f\n", fname, lname, n, cost);
}