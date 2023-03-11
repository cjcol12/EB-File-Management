/*  Function: Module used in Echo and Comp modules to read in image_data.
    
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
#include "read_image.h"

// Generic function file inclusion
#include "generic.c"

// Error checking module inclusion
#include "error_checking.c"

int check_magic_number(
    image_struct_type *image_struct, char *input_file_name, FILE *input_file){

    // stores first two characters of file into character array
    image_struct->magic_number[0] = getc(input_file);
    image_struct->magic_number[1] = getc(input_file);

    // cast to short for endianness
    image_struct->magic_number_value = 
    (unsigned short *)image_struct->magic_number;
    
    if (check_mn_valid(image_struct, input_file, input_file_name) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;

    // return 0 on function success
    return FUNCTION_SUCCESS;
}

int check_dimensions(
    image_struct_type *image_struct, char *input_file_name, FILE *input_file){

    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    image_struct->check = 
    fscanf(input_file, "%d %d", &image_struct->height, &image_struct->width);

    if (check_dimensions_valid(image_struct, input_file, input_file_name) == BAD_DIMENSION)
        return BAD_DIMENSION;

    // return 0 on function success
    return FUNCTION_SUCCESS;
}


int check_malloc(image_struct_type *image_struct, FILE *input_file){
    // setting numBytes
    image_struct->numBytes = image_struct->width * image_struct->height;
    
    // allocating memory for pointers to each row 
    image_struct->imageData = 
    (unsigned int **) malloc(image_struct->height * sizeof(unsigned int *));

    // testing return value of malloc
    if (check_image_data_malloc(image_struct, input_file) == BAD_MALLOC)
        return BAD_MALLOC;

    // allocating memory for all data at once
    image_struct->data_block = 
    (unsigned int *) malloc(image_struct->numBytes * sizeof(unsigned int));

    // testing return value of malloc
    if (check_data_block_malloc(image_struct, input_file) == BAD_MALLOC)
        return BAD_MALLOC;

    // loop set up pointers to each row - saves multiple costly malloc calls
    for (int row = 0; row < image_struct->height; row++){
        image_struct->imageData[row] = image_struct->data_block + row * image_struct->width; 
    }

    // return 0 on function success
    return FUNCTION_SUCCESS;
}


int read_data(
    image_struct_type *image_struct, char *input_file_name, FILE *input_file){

    // iterating through image
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){
            unsigned int data;
            // reading in data
            image_struct->check = fscanf(input_file, "%u", &data);

            // check data is captured correctly
            if (check_data_captured(image_struct, input_file, input_file_name) == BAD_DATA)
                return BAD_DATA;

            // check data is within bounds (0 - 31)
            if (check_data_values(data, input_file, input_file_name, image_struct) == BAD_DATA)
                return BAD_DATA;

            // set correct data to imageData
            image_struct->imageData[i][j] = data;
        }
    }
 
    // repeat fscanf to check if there is any data we haven't read in
    image_struct->check = fscanf(input_file, "%u", *image_struct->imageData);

    // if there is more data, fscanf returns 1
    // if thats the case, we have too much data ( > numBytes)
    if (check_extra_data(image_struct, input_file, input_file_name) == BAD_DATA)
        return BAD_DATA;
    
    // Finished with input file
    fclose(input_file);
    return FUNCTION_SUCCESS;
}

