#include <stdio.h>

int output[12][12] = {0};
int counter = 0;

int main()
{
    for (int y = 0; y < 12; y++)
    {
        printf("%d: ", y);
 		for (int x = 0; x < 12; x++)
 	    {
 		    output[y][x] = counter;
 		    counter ++;
 		    printf("%d  ", output[y][x]);
 	    }
 	    printf("\n");
    }

    return 0;
}