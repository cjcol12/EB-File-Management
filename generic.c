/**
 * @file destructor.c
 * @brief Module used generically to free allocated memory and close files.
 * @details This module provides functions to release allocated memory and close files. The `destructor` function is used when both memory deallocation and file closure are required, while the `destructor_no_file` function is used for memory deallocation only.
 * @author CJ Coleman
 */

// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Image structure header file inclusion
#include "image_structs.h"

/**
 * @brief Frees memory and closes a file.
 * @param image_struct Pointer to the image_struct_type struct.
 * @param file Pointer to the FILE object representing the file.
 * @return void
 */
void destructor(image_struct_type *image_struct, FILE *file)
{
    // Free the data block if it is not NULL
    if (image_struct->data_block != NULL)
    {
        free(image_struct->data_block);
    }

    // Free the image data if it is not NULL
    if (image_struct->imageData != NULL)
    {
        free(image_struct->imageData);
    }

    // Close the file if it is not NULL
    if (file != NULL)
    {
        fclose(file);
    }
}

/**
 * @brief Frees memory without closing a file.
 * @param image_struct Pointer to the image_struct_type struct.
 * @return void
 */
void destructor_no_file(image_struct_type *image_struct)
{
    // Free the data block if it is not NULL
    if (image_struct->data_block != NULL)
    {
        free(image_struct->data_block);
    }

    // Free the image data if it is not NULL
    if (image_struct->imageData != NULL)
    {
        free(image_struct->imageData);
    }
}
