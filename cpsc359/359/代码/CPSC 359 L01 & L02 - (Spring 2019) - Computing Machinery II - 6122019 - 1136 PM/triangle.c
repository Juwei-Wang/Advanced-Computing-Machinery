#include <stdio.h>
#include <stdlib.h>
 
int main()
{
    int i, k, levels, space;
    printf("Enter the number of levels in the triangle:");
    scanf("%d",&levels);
 
    space = levels;
 
    for ( i = 1 ; i <= levels ; i++ )
    {
        for ( k = 1 ; k < space ; k++ )
            printf(" ");
        space--;
 
        for ( k = 1 ; k <= 2*i - 1 ; k++ )
            printf("*");
        printf("\n");
    }
    return 0;
}
