#include <stdio.h>
#include <stdint.h>
#define size 13

// The input array. This is not assumed to be static. Size and/or contents are subject to change, thus we add "volatile".
// For simplicity, we assume the size is "static" by declaring a size above.
volatile uint32_t matrix[size][size] =
{
	{56318, 55435, 39177, 22213, 58312, 22620, 25135, 63007, 19718, 25969, 7824, 34034, 54937},
	{50697, 40447, 36839, 32714, 57451, 64484, 19561, 12492, 7940, 5808, 40023, 53039, 45524},
	{17339, 46509, 41720, 17743, 43777, 18454, 53883, 35867, 9173, 15225, 8334, 16205, 9098},
	{36283, 223, 53009, 32364, 43146, 814, 27450, 58986, 4452, 15319, 10053, 16022, 3122},
	{40009, 3993, 21945, 61121, 42736, 65338, 13274, 36133, 59577, 60451, 41909, 28608, 31519},
	{12851, 53332, 49060, 58231, 18026, 3843, 6614, 61522, 33426, 41991, 8824, 46200, 35444},
	{47163, 22304, 56786, 48662, 10487, 6428, 42075, 43076, 7003, 21968, 7323, 29736, 21388},
	{57285, 59295, 35671, 61019, 6141, 43184, 41962, 41288, 14951, 27409, 43637, 60882, 30348},
	{26628, 23618, 41793, 27902, 24210, 46694, 33393, 40528, 47553, 48951, 11936, 27547, 1171},
	{15159, 46182, 5166, 38079, 31541, 35958, 57666, 12310, 29341, 62563, 24859, 7780, 36857},
	{28781, 63208, 6635, 47065, 55871, 65144, 5827, 53107, 49837, 31788, 57265, 15869, 62462},
	{43173, 40021, 18362, 27579, 43204, 60972, 55150, 21328, 22400, 24888, 17593, 36659, 10939},
	{63257, 48374, 58052, 24604, 39581, 4516, 5788, 64307, 41008, 53389, 56660, 64997, 35516},
};

// The convolution kernel. This is assumed to be always static
const uint32_t kernel[2][2] =
{
	{15, 2},
	{3, 5}
};

// The output array. Its size is always smaller than the input array, and the size value depends on the kernel size.
// The formula for calculating the (y,x) size of the output is:
// [ (W-F+2P)/S ] + 1, with:
// W the y or x size of the array,
// F the kernel size,
// P the padding around the main array,
// S the step with which we slide the kernel over the main array.
uint32_t output[size-1][size-1] = {0};


int main()
{

    /*
    Based on the code of the medium architecture, a more in depth unrolling will happen for the performance architecture.
    Instead of "size*size" repetitions for the loop, we will try to make it complete all calculations in "size" repetitions.
    */
    
    // Calculating the convolution array by "unrolling" output calculation into its 12 constituent parts
    for(int iter_y=0; iter_y<size-1; iter_y++)
    {
        // Each "block" here represents the calculations that will happen per column
        for(int iter_x=0; iter_x<size-1; iter_x++)
        {
            //output_y = iter/(size-1);
            //output_x = iter - (iter/(size-1) * (size-1));

            // calculates all the output values for: output[0][iter]
            output[iter_y][iter_x] = 
                matrix[iter_y    ][iter_x    ] * 15 +
               (matrix[iter_y    ][iter_x + 1]<< 1) +
                matrix[iter_y + 1][iter_x    ] * 3  +
                matrix[iter_y + 1][iter_x + 1] * 5;
        }
    }

    // Checking if the output is the same as the one with the original method of calculation
    // for(int iter=0; iter<(size-1)*(size-1); iter++)
    //     printf("%d\n", output[iter/(size-1)][iter - (iter/(size-1) * (size-1))]-output2[iter/(size-1)][iter - (iter/(size-1) * (size-1))]);
    for (int y = 0; y < 12; y++)
    {
        for (int x = 0; x < 12; x++)
            printf("%d ", output[y][x]);
        printf("\n");
    }




return 0;
}

