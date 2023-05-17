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

/**
 * @brief Compares the magic numbers of two image structures.
 * 
 * This function compares the magic numbers of two provided image structures.
 * If the magic numbers are different, it destructs the structures without
 * saving to a file and returns a predefined success code. If the magic numbers 
 * are the same, it simply returns a predefined success code.
 * 
 * @param image_struct The first image structure for the comparison.
 * @param image_struct_compare The second image structure for the comparison.
 * 
 * @return Returns FUNCTION_SUCCESS_DIFFERENT if the magic numbers are different,
 *         and FUNCTION_SUCCESS if the magic numbers are the same.
 */
int comp_magic_number(image_struct_type *image_struct, image_struct_type *image_struct_compare)
{

    // Compare the magic numbers of two image structs
    if (*(image_struct->magic_number_value) !=
        *(image_struct_compare->magic_number_value))
    {

        // If the magic numbers are not equal, print "DIFFERENT" and destruct the image structs
        printf("DIFFERENT\n");
        destructor_no_file(image_struct);
        destructor_no_file(image_struct_compare);

        // Return 0
        return FUNCTION_SUCCESS_DIFFERENT;
    }
    else

        // If the magic numbers are equal, return 0
        return FUNCTION_SUCCESS;
}

/**
 * @brief Compares the dimensions of two image structures.
 * 
 * This function compares the height and width of two provided image structures.
 * If the dimensions are not the same, it destructs the structures without
 * saving to a file and returns a predefined success code. If the dimensions 
 * are the same, it simply returns a predefined success code.
 * 
 * @param image_struct The first image structure for the comparison.
 * @param image_struct_compare The second image structure for the comparison.
 * 
 * @return Returns FUNCTION_SUCCESS_DIFFERENT if the dimensions are different,
 *         and FUNCTION_SUCCESS if the dimensions are the same.
 */
int comp_dimensions(image_struct_type *image_struct, image_struct_type *image_struct_compare)
{

    // Compare the height and width of two image structs
    if ((image_struct->height != image_struct_compare->height) ||
        (image_struct->width != image_struct_compare->width))
    {

        // If the dimensions are not the same, print "DIFFERENT" and destruct the image structs
        printf("DIFFERENT\n");
        destructor_no_file(image_struct);
        destructor_no_file(image_struct_compare);

        // Return a constant indicating success, but with different dimensions
        return FUNCTION_SUCCESS_DIFFERENT;
    }
    else

        // If the dimensions are the same, return a constant indicating success
        return FUNCTION_SUCCESS;
}


/**
 * @brief Compares the image data of two image structures.
 * 
 * This function iterates through the 2D arrays of image data in two provided image structures.
 * If any pixel values are not the same, it destructs the structures without
 * saving to a file and returns a predefined success code. If all pixel values 
 * are the same, it simply returns a predefined success code.
 * 
 * @param image_struct The first image structure for the comparison.
 * @param image_struct_compare The second image structure for the comparison.
 * 
 * @return Returns FUNCTION_SUCCESS_DIFFERENT if any pixel values are different,
 *         and FUNCTION_SUCCESS if all pixel values are the same.
 */
int comp_image_data(image_struct_type *image_struct, image_struct_type *image_struct_compare)
{
    // Loop through the pixels of the image arrays
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {

            // Compare the pixel values of the two image structs
            if (image_struct->imageData[i][j] !=
                image_struct_compare->imageData[i][j])
            {

                // If any pixel is different, destruct the image structs, and return 0
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