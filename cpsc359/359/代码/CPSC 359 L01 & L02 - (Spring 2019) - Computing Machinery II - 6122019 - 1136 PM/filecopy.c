#include <stdio.h>
 
int main(argc, argv) 
	int argc;
	char *argv[];
{
	FILE *inp, *outp;
	char ch;
	
	if (argc < 3) {
		printf("Usage filecopy \"sourceFile\" \"destinationFile\"\n");
		return 1;
	}
	
	if ((outp = fopen(argv[2], "r")) != NULL) {
		fclose(outp);
		printf("Destination file \"%s\" already exists.\n", argv[2]);
		printf("Copy aborted.\n");
		return 1;
	}
	
	if ((inp = fopen(argv[1], "r")) == NULL) {
		printf("Unable to open input file \"%s\".\n", argv[1]);
		return 1;
	}
	
	if ((outp = fopen(argv[2], "w")) == NULL) {
		printf("Unable to open output file.\n");
		return 1;
	}
	
	while ((ch = fgetc(inp)) != EOF) {
		fputc (ch,outp);	
	}
	fputc(EOF,outp);
	fclose(outp);
	fclose(inp);
	printf("File \"%s\" has been successfuly copied to \"%s\".\n", argv[1], argv[2]);
	return 0;
	
}
