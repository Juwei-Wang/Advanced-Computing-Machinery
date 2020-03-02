#include <stdio.h>
 
int main(argc, argv) 
	int argc;
	char *argv[];
{
	int n = 0;
	if (argc == 1) return 1;
	while (argv[1][n++] != '\0')	;
	printf("\"%s\" has %d letters",argv[1],n);
}
