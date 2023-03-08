/*  Function: Module used in ebfEcho and ebfComp to read in image_data.
    
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

int check_arg_count(int argc){
    // validate that user has entered 2 arguments
    if (argc != 3){
        printf("ERROR: Bad Arguments\n"); 
        return BAD_ARGUMENT_COUNT; 
    }
    
    // return 0 on function success
    else return FUNCTION_SUCCESS;
}

int check_file_opened(char *input_file_name, FILE *input_file){
    // check file opened correctly
    if (!input_file){ 
        printf("ERROR: Bad File Name (%s)\n", input_file_name);
        return BAD_FILE;
    }

    // return 0 on function success
    else return FUNCTION_SUCCESS;
}

int check_magic_number(
    image_struct_type *image_struct, char *input_file_name, FILE *input_file){

    // stores first two characters of file into character array
    image_struct->magic_number[0] = getc(input_file);
    image_struct->magic_number[1] = getc(input_file);

    // cast to short for endianness
    image_struct->magic_number_value = 
    (unsigned short *)image_struct->magic_number;
    
    // checking magic number against the casted value due to endienness.
    // change to check file type (.ebf /.ebu)?
    if (*(image_struct->magic_number_value) != MAGIC_NUMBER_EB &&
        *(image_struct->magic_number_value) != MAGIC_NUMBER_EU){
        printf("ERROR: Bad Magic Number (%s)\n", input_file_name);
        fclose(input_file);
        return BAD_MAGIC_NUMBER;
    }

    // return 0 on function success
    else return FUNCTION_SUCCESS;
}

int check_dimensions(
    image_struct_type *image_struct, char *input_file_name, FILE *input_file){

    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    image_struct->check = 
    fscanf(input_file, "%d %d", &image_struct->height, &image_struct->width);

    // checks we captured two return values
    // checks dimensions are valid
    if (image_struct->check != 2 || 
        image_struct->height < MIN_DIMENSION || 
        image_struct->width < MIN_DIMENSION || 
        image_struct->height > MAX_DIMENSION || 
        image_struct->width > MAX_DIMENSION){
        
        fclose(input_file);
        printf("ERROR: Bad Dimensions (%s)\n", input_file_name);
        return BAD_DIMENSION;
    }

    // return 0 on function success
    else return FUNCTION_SUCCESS;
}
int check_malloc(image_struct_type *image_struct, FILE *input_file){
    // setting numBytes
    image_struct->numBytes = image_struct->width * image_struct->height;
    
    // allocating memory for pointers to each row 
    image_struct->imageData = 
    (unsigned int **) malloc(image_struct->height * sizeof(unsigned int *));

    // testing return value of malloc
    if (image_struct->imageData == NULL){
        printf("ERROR: Image Malloc Failed\n");
        destructor(image_struct, input_file);
        return BAD_MALLOC;
    }
    
    // allocating memory for all data at once
    image_struct->data_block = 
    (unsigned int *) malloc(image_struct->numBytes * sizeof(unsigned int));

    // testing return value of malloc
    if (image_struct->data_block == NULL){
        printf("ERROR: Image Malloc Failed\n");    
        destructor(image_struct, input_file);    
        return BAD_MALLOC;
    }

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

            // testing one value captured
            if (image_struct->check != 1){
                printf("ERROR: Bad Data (%s)\n", input_file_name);
                destructor(image_struct, input_file);
                return BAD_DATA;
            }

            // setting imageData to data
            image_struct->imageData[i][j] = data;

            // check data is within bounds (0 - 31)
            if (image_struct->imageData[i][j] > MAX_GRAY
            ||image_struct->imageData[i][j] < MIN_GRAY){
                printf("ERROR: Bad Data (%s)\n", input_file_name);
                destructor(image_struct, input_file);
                return BAD_DATA;
            }
        }
    }
 
    // repeat fscanf to check if there is any data we haven't read in
    image_struct->check = fscanf(input_file, "%u", *image_struct->imageData);

    // if there is more data, fscanf returns 1
    // if thats the case, we have too much data ( > numBytes)
    if (image_struct->check == 1){
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
    }

    // Finished with input file - close it
    fclose(input_file);
    return FUNCTION_SUCCESS;
}

// // add error checking
// int read_binary_data(
//     image_struct_type *image_struct, char *input_file_name, FILE *input_file){

//     unsigned char test;
//     fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    
//     for(int i = 0; i < image_struct->height; i++){
//         for(int j = 0; j < image_struct->width; j++){
//             unsigned char binary_value;
//             unsigned int value;

//             image_struct->check = fread(&binary_value, sizeof(unsigned char), 1, input_file);

//             if(image_struct->check != 1){
//                 printf("ERROR: Bad Data (%s)\n", input_file_name);
//                 destructor(image_struct, input_file);
//                 return BAD_DATA;
//             }
//             value = (unsigned int) binary_value;

//             image_struct->imageData[i][j] = value;

//             if (image_struct->imageData[i][j] > MAX_GRAY
//             ||image_struct->imageData[i][j] < MIN_GRAY){
//                 printf("ERROR: Bad Data (%s)\n", input_file_name);
//                 destructor(image_struct, input_file);
//                 return BAD_DATA;
//             }
//         }
//     }

//     return 0;
// }