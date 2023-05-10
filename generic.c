/*  Function: Module used generically to free allocated memory and close files

    No main function

    Author: CJ Coleman
*/

// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Image structure header file inclusion
#include "image_structs.h"

// frees memory and closes file
void destructor(image_struct_type *image_struct, FILE *file)
{
    if (image_struct->data_block != NULL)
    {
        free(image_struct->data_block);
    }

    if (image_struct->imageData != NULL)
    {
        free(image_struct->imageData);
    }

    if (file != NULL)
    {
        fclose(file);
    }
}

// frees memory
void destructor_no_file(image_struct_type *image_struct)
{
    if (image_struct->data_block != NULL)
    {
        free(image_struct->data_block);
    }

    if (image_struct->imageData != NULL)
    {
        free(image_struct->imageData);
    }
}