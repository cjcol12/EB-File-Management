/*  Function: Module used in ebfComp to compare images.

    No main function

    Author: CJ Coleman
*/

// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

// Image structure header file inclusion
#include "image_structs.h"

// Function prototype header file inclusion
#include "compare_image.h"


// compares magic_number
/**
 * @brief Compares the magic number of two image structures.
 *
 * This function compares the magic number of two image structures, `image_struct` and `image_struct_compare`.
 * If the magic numbers are different, it prints a message, performs cleanup, and returns `FUNCTION_SUCCESS_DIFFERENT`.
 * Otherwise, it returns `FUNCTION_SUCCESS`.
 *
 * @param image_struct Pointer to the first image structure.
 * @param image_struct_compare Pointer to the second image structure.
 * @return Returns `FUNCTION_SUCCESS_DIFFERENT` if the magic numbers are different, or `FUNCTION_SUCCESS` if they are the same.
 */
int comp_magic_number(image_struct_type *image_struct, image_struct_type *image_struct_compare)
{
    // Compares the magic number of two structs
    // prints different and returns if different
    // also destructs structures
    if (*(image_struct->magic_number_value) != *(image_struct_compare->magic_number_value))
    {
        printf("DIFFERENT\n");
        destructor_no_file(image_struct);
        destructor_no_file(image_struct_compare);

        // Return 0
        return FUNCTION_SUCCESS_DIFFERENT;
    }
    else
    {
        return FUNCTION_SUCCESS;
    }
}


/**
 * @brief Compares the dimensions of two image structures.
 *
 * This function compares the height and width of two image structures, `image_struct` and `image_struct_compare`.
 * If the dimensions are different, it prints a message, performs cleanup, and returns `FUNCTION_SUCCESS_DIFFERENT`.
 * Otherwise, it returns `FUNCTION_SUCCESS`.
 *
 * @param image_struct Pointer to the first image structure.
 * @param image_struct_compare Pointer to the second image structure.
 * @return Returns `FUNCTION_SUCCESS_DIFFERENT` if the dimensions are different, or `FUNCTION_SUCCESS` if they are the same.
 */
int comp_dimensions(image_struct_type *image_struct, image_struct_type *image_struct_compare)
{
    // Compares if height and width are the same in both images
    // prints different and returns if different
    // also destructs structures
    if ((image_struct->height != image_struct_compare->height) ||
        (image_struct->width != image_struct_compare->width))
    {
        printf("DIFFERENT\n");
        destructor_no_file(image_struct);
        destructor_no_file(image_struct_compare);

        // Return a constant indicating success, but with different dimensions
        return FUNCTION_SUCCESS_DIFFERENT;
    }
    else
    {
        return FUNCTION_SUCCESS;
    }
}


/**
 * @brief Compares the image data of two image structures.
 *
 * This function compares the pixel values of two image structures, `image_struct` and `image_struct_compare`,
 * by iterating through their 2D arrays. If any pixel values differ, it prints a message, performs cleanup,
 * and returns `FUNCTION_SUCCESS_DIFFERENT`. Otherwise, it returns `FUNCTION_SUCCESS`.
 *
 * @param image_struct Pointer to the first image structure.
 * @param image_struct_compare Pointer to the second image structure.
 * @return Returns `FUNCTION_SUCCESS_DIFFERENT` if the image data is different, or `FUNCTION_SUCCESS` if it is the same.
 */
int comp_image_data(image_struct_type *image_struct, image_struct_type *image_struct_compare)
{
    // Iterate through 2D array
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {
            // Compare pixel values
            // prints different and returns if different
            // also destructs structures
            if (image_struct->imageData[i][j] != image_struct_compare->imageData[i][j])
            {
                printf("DIFFERENT\n");
                destructor_no_file(image_struct);
                destructor_no_file(image_struct_compare);
                return FUNCTION_SUCCESS_DIFFERENT;
            }
        }
    }
    // If all pixels are the same, return a success constant
    return FUNCTION_SUCCESS;
}
