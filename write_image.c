// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

#include "image_structs.h"

int write_header(image_struct_type *image_struct, FILE *outputFile){
    // write the header data in one block
    // change to write with magicNumberValue
    image_struct->check = fprintf(outputFile, "%c%c\n%d %d\n",
    image_struct->magicNumber[0], image_struct->magicNumber[1], image_struct->height, image_struct->width);
    // and use the return from fprintf to check that we wrote.
    if (image_struct->check == 0) 
        { // check write
        fclose(outputFile);
        free(image_struct->imageData);
        printf("ERROR: Bad Output\n");
        return BAD_OUTPUT;
        } // check write
    else return FUNCTION_SUCCESS;
}

int write_image_data(image_struct_type *image_struct, FILE *outputFile){
    // iterate though the array and print out pixel values
    for (int current = 0; current < image_struct->numBytes; current++)
        { // writing out
        // if we are at the end of a row ((current+1)%width == 0) then write a newline, otherwise a space.
        image_struct->check = fprintf(outputFile, "%u%c", image_struct->imageData[current], ((current + 1) % image_struct->width) ? 's' : '\n');
        if (image_struct->check == 0)
            { // check write
            fclose(outputFile);
            free(image_struct->imageData);
            printf("ERROR: Bad Output\n");
            return BAD_OUTPUT;
            } // check write
        } // writing out
    return FUNCTION_SUCCESS;
}