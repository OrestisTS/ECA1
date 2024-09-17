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
	// for (int y = 0; y < size-1; y++)
	// 	for (int x = 0; x < size-1; x++)
	// 		for (int ky = 0; ky < 2; ky++)
	// 			for (int kx = 0; kx < 2; kx++)
	// 				output[y][x] += matrix[y + ky][x + kx] * kernel[ky][kx];

    /* Trying to describe the (y,x) coordinates of the 2D output array based only on the 1D iterator value of the 2D output array.
    Meaning, on a 12x12, the iterator of a corresponding 1D array iterator goes from 0-143 because 12*12 = 144.
    Given this iterator value, we can describe the corresponding 2D coordinates on a 2D array.
    For example, element [3][2] of a 2D array is element 38.
    Remember, on a 2D array, the 1st value of the y value, and the 2nd is the x value.
    
    The y(row) of the array can be described as the current iterator value div the size of the array.
    It describes how many times "size" can fit into the current iterator value. 
    Example for size = 12: 
    For the 1st row, meaning from 0-11, iterator div 12 gives us 0.
    On the 2nd row, from 12-23, iterator div 12 gives us 1, etc.
        Finally, row = iterator / size
    
    The x(column) of the array can be described as the current iterator value reduced by the current row*size of the array.
    It takes the iterator value and reduces it by a specific amount so that the "x" value always sits between 0-size.
    Example for size = 12: For the 1st row, meaning from 0-11, iterator - (row * size) will always clamp the 1st row values from 0-11.
    Example for value 27 of a 12x12 array:
    27 - (row * size) =>
    27 - (iterator / size * size) =>
    27 - (27 / 12 * 12) => 
    27 - (2 * 12) => 
    27-24 = 3
    Indeed, the value 27 on a 2D array has the coordinates of [2][3]
    In the "iterator/size * size" the '/' and the '*' do not cancel each other out.
        Finally, column = iterator - (row * size) =>
        column = iterator - (iterator / size * size)
    */

    // Create vars that will store the values of the current (y,x) coords of the 2D array so that we wont have to recalculate it each time
    int output_y, output_x;

    // Calculating the convolution array by using only the 1D iterator of the 2D array.
    for(int iter=0; iter<(size-1)*(size-1); iter++)
	{
        output_y = iter/(size-1);
        output_x = iter - (iter/(size-1) * (size-1));

	    output[output_y][output_x] = 

            // matrix[y][x]*kernel[0][0] => matrix[y][x]*15 => matrix[y][x]* 2^4 - matrix[y][x]
            matrix[output_y][output_x] * 15 +

            // matrix[y][x]*kernel[0][1] => matrix[y][x]*2 => //matrix[y][x]*2^1
            ( matrix[output_y][output_x + 1] << 1 ) +

            // matrix[y][x]*kernel[1][0] => matrix[y][x]*3 => //matrix[y][x]*2^1 + matrix[y][x]
            matrix[output_y + 1][output_x] * 3 +

            // matrix[y][x]*kernel[1][1] => matrix[y][x]*5 => //matrix[y][x]*2^2 + matrix[y][x]
            matrix[output_y + 1][output_x + 1] * 5;
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

