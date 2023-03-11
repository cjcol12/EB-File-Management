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
#include "generic.h"

// Generic function file inclusion
#include "binary_management.h"


// Error checking module inclusion
#include "error_checking.h"

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
            if (check_data_captured(image_struct, input_file, input_file_name) == BAD_DATA)
                return BAD_DATA;

            // cast to int and store back in 2d array
            value = (unsigned int) binary_value;
            image_struct->imageData[i][j] = value;

            // check imageData is within bounds
            if (check_data_values(value, input_file, input_file_name, image_struct) == BAD_DATA)
                return BAD_DATA;
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
            if (check_binary_written(image_struct, output_file) == BAD_OUTPUT)
                return BAD_OUTPUT;

        } 
    }
    return FUNCTION_SUCCESS;
}

//int compress_data(image_struct_type *image_struct, FILE *output_file){


    // int count = 0;
    // unsigned char buffer;
    // unsigned char right_buffer;
    // for (int i = 0; i < image_struct->height; i++){
    //     for (int j = 0; j < image_struct->width; j++){

    //         if (i < image_struct->height - 1){
    //             right_buffer = image_struct->imageData[i + 1][j];
    //         }
    //         if (j < image_struct->width - 1){
    //             printf("image %d\n", image_struct->imageData[i][j]);
    //             right_buffer = image_struct->imageData[i][j + 1];
    //         }

    //         switch(count){

    //             case(0):
    //                 buffer = image_struct->imageData[i][j] << 3;
    //                 right_buffer = right_buffer >> 3;
    //                 image_struct->imageData[i][j] = buffer + right_buffer;
    //                 printf("imageData %d\n", image_struct->imageData[i][j]);
    //                 break;
    //             case(1):
    //                 buffer = image_struct->imageData[i][j] << 3;
    //                 right_buffer = right_buffer >> 3;
    //                 image_struct->imageData[i][j] = buffer + right_buffer;
    //                 printf("imageData %d\n", right_buffer);
    //                 break;
    //         }
            
    //         count++;
    //     }
    // }
    // return FUNCTION_SUCCESS;
//}