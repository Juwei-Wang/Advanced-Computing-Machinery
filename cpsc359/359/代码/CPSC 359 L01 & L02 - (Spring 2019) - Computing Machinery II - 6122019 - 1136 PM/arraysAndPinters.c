    #include <stdio.h>

    void main() {
        static int a[10] = {1,2,3,4,5,6,7,8,9,10};
		int *ptr, i;
        
		for (i = 0; i < 10; i++)
            printf("%d ", *(a+i));
		
		printf("\n");
        ptr = a;
		for (i = 0; i < 10; i++)
            printf("%d ", *(ptr+i));
    }