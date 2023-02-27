/*  Function: Read in a .ebf file and echo (copy) its contents to another file
    
    Arguments: Expects 3 arguments: ./ebfEcho input_file, output_file
    
    Returns: 0 on success, different values depending on error - found in 
    definitions.h

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


int write_binary_data(image_struct_type *image_struct, FILE *output_file){
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){
            // printf("%d ", image_struct->imageData[i][j]);
            // fwrite(&image_struct->imageData[i][j], sizeof(int), image_struct->width * image_struct->height, output_file);

            unsigned int value = image_struct->imageData[i][j];
            unsigned char binary_value = (unsigned char)(value & 0x1F);

            fwrite(&binary_value, sizeof(unsigned char), 1, output_file);

        }
    }
    return 0;
}