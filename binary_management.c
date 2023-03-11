/*  Function: Module to read and write binary data to and from files

    No main function

    Author: CJ Coleman
*/


// Standard I/O header file inclusion
#include <stdio.h>

// Image structure header file inclusion
#include "image_structs.h"

// Definition header file inclusion
#include "definitions.h"

// Generic function file inclusion
// #include "generic.c"

// Generic function file inclusion
#include "binary_management.h"

// add error checking
int read_binary_data(
    image_struct_type *image_struct, char *input_file_name, FILE *input_file){

    unsigned char test;
    fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    // fread starts on the wrong line - manually incremenet it 
    
    // iterate through 2d array
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){
            unsigned char binary_value;
            unsigned int value;

            // read in binary values
            image_struct->check = fread(&binary_value, sizeof(unsigned char), 1, input_file);

            // check fread has read exactly one item
            if(image_struct->check != 1){
                printf("ERROR: Bad Data (%s)\n", input_file_name);
                destructor(image_struct, input_file);
                return BAD_DATA;
            }

            // cast to int and store back in 2d array
            value = (unsigned int) binary_value;
            image_struct->imageData[i][j] = value;

            // check imageData is within bounds
            if (image_struct->imageData[i][j] > MAX_GRAY
            ||image_struct->imageData[i][j] < MIN_GRAY){
                printf("ERROR: Bad Data (%s)\n", input_file_name);
                destructor(image_struct, input_file);
                return BAD_DATA;
            }
        }
    }
    return FUNCTION_SUCCESS;
}

// add error checking
int write_binary_data(image_struct_type *image_struct, FILE *output_file){
    // iterate through 2d array
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){

            unsigned int value = image_struct->imageData[i][j];
            unsigned char binary_value = (unsigned char)(value & 0x1F); 
            // do we need a unit seperator to limit size to 1F - & 0x1F

            // write binary values
            image_struct->check = fwrite(&binary_value, sizeof(unsigned char), 1, output_file);
                        
            // check fwrite has written exactly one item
            if (image_struct->check != 1){
                destructor(image_struct, output_file);
                printf("ERROR: Bad Output\n");
                return BAD_OUTPUT;
            }
        } 
    }
    return FUNCTION_SUCCESS;
}