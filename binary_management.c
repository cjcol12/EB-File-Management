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
    
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){
            unsigned char binary_value;
            unsigned int value;

            image_struct->check = fread(&binary_value, sizeof(unsigned char), 1, input_file);

            if(image_struct->check != 1){
                printf("ERROR: Bad Data (%s)\n", input_file_name);
                destructor(image_struct, input_file);
                return BAD_DATA;
            }
            value = (unsigned int) binary_value;

            image_struct->imageData[i][j] = value;

            if (image_struct->imageData[i][j] > MAX_GRAY
            ||image_struct->imageData[i][j] < MIN_GRAY){
                printf("ERROR: Bad Data (%s)\n", input_file_name);
                destructor(image_struct, input_file);
                return BAD_DATA;
            }
        }
    }
    return 0;
}

// add error checking
int write_binary_data(image_struct_type *image_struct, FILE *output_file){
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){

            unsigned int value = image_struct->imageData[i][j];
            unsigned char binary_value = (unsigned char)(value & 0x1F); 
            // do we need a unit seperator to limit size to 1F - & 0x1F

            fwrite(&binary_value, sizeof(unsigned char), 1, output_file);
        } 
    }
    return 0;
}