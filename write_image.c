/*  Function: Module used in ebfEcho to write image_data to output_file.
    
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
#include "write_image.h"

// Generic function file inclusion
// #include "generic.c"

// Error checking module inclusion
#include "error_checking.h"

/**
 * Function: write_header
 * ----------------------
 * Writes the header data of the image to the output file.
 *
 * image_struct: Pointer to the image_struct_type that holds image data and information.
 * output_file: Pointer to the output file.
 *
 * returns: 0 on success, BAD_OUTPUT on error.
 */
int write_header(image_struct_type *image_struct, FILE *output_file)
{
    // write the header data in one block
<<<<<<< HEAD
    image_struct->check = fprintf(output_file, "%c%c\n%d %d\n",
                                  image_struct->magic_number[0], image_struct->magic_number[1], image_struct->height, image_struct->width);
=======
    // change to write with magicNumberValue
    image_struct->check = fprintf(output_file, "%s\n%d %d\n", 
    image_struct->magic_number, image_struct->height, image_struct->width);
>>>>>>> 0d7f47695274b2a53e0b80fc1762bf5f30e2ac10

    // check if data is written correctly
    if (check_data_written(image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;
    else
        return FUNCTION_SUCCESS;
}

/**
 * Function: write_image_data
 * --------------------------
 * Writes the image data to the output file.
 *
 * image_struct: Pointer to the image_struct_type that holds image data and information.
 * output_file: Pointer to the output file.
 *
 * returns: 0 on success, BAD_OUTPUT on error.
 */
int write_image_data(image_struct_type *image_struct, FILE *output_file)
{
    // iterating through 2d array and writing
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {
            if (image_struct->width == j + 1)
            {
                image_struct->check = fprintf(output_file, "%u", image_struct->imageData[i][j]);
            }
            else
            {
                image_struct->check = fprintf(output_file, "%u ", image_struct->imageData[i][j]);
            }

            // checks if fprintf has run successfully
            if (check_data_written(image_struct, output_file) == BAD_OUTPUT)
                return BAD_OUTPUT;
        }

        if (i + 2 <= image_struct->height)
        {
            image_struct->check = fprintf(output_file, "\n");
        }

        // checks if fprintf has run successfully
        if (check_data_written(image_struct, output_file) == BAD_OUTPUT)
            return BAD_OUTPUT;
    }

    return FUNCTION_SUCCESS;
}
