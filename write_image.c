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
#include "generic.h"

// Error checking module inclusion
#include "error_checking.h"

int write_header(image_struct_type *image_struct, FILE *output_file){
    // write the header data in one block
    image_struct->check = fprintf(output_file, "%c%c\n%d %d\n", 
    image_struct->magic_number[0], image_struct->magic_number[1], image_struct->height, image_struct->width);

    // check data is written correctly 
    if (check_data_written(image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;

    else return FUNCTION_SUCCESS;
}

int write_image_data(image_struct_type *image_struct, FILE *output_file){
    // iterating through 2d array and writing
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){

            if (image_struct->width == j + 1){
                image_struct->check = fprintf(output_file, "%u", image_struct->imageData[i][j]);
            }
            else{       
                image_struct->check = fprintf(output_file, "%u ", image_struct->imageData[i][j]);
            }
            // checks fprintf has run succesfully
            if (check_data_written(image_struct, output_file) == BAD_OUTPUT)
                return BAD_OUTPUT;
        }

        if (i + 2 <= image_struct->height){
            image_struct->check = fprintf(output_file, "\n");
        }

        // checks fprintf has run succesfully
        if (check_data_written(image_struct, output_file) == BAD_OUTPUT)
            return BAD_OUTPUT;
    }
    return FUNCTION_SUCCESS;
}
