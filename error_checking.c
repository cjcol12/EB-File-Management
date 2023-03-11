// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

// Image structure header file inclusion
#include "image_structs.h"

// Error checking header file inclusion
#include "error_checking.h"

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

int check_mn_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name){
    // checking magic number against the casted value due to endienness.
    // change to check file type (.ebf /.ebu)?
    if (*(image_struct->magic_number_value) != MAGIC_NUMBER_EB &&
        *(image_struct->magic_number_value) != MAGIC_NUMBER_EU){
        printf("ERROR: Bad Magic Number (%s)\n", input_file_name);
        fclose(input_file);
        return BAD_MAGIC_NUMBER;
    }
    else return FUNCTION_SUCCESS;
}

int check_dimensions_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name){
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
    else return FUNCTION_SUCCESS;
}

int check_image_data_malloc(image_struct_type *image_struct, FILE *input_file){
    if (image_struct->imageData == NULL){
        printf("ERROR: Image Malloc Failed\n");
        destructor(image_struct, input_file);
        return BAD_MALLOC;
    }
    return FUNCTION_SUCCESS;
}

int check_data_block_malloc(image_struct_type *image_struct, FILE *input_file){
    if (image_struct->data_block == NULL){
        printf("ERROR: Image Malloc Failed\n");    
        destructor(image_struct, input_file);    
        return BAD_MALLOC;
    }
    return FUNCTION_SUCCESS;
}

int check_extra_data(image_struct_type *image_struct, FILE *input_file, char *input_file_name){
    if (image_struct->check == 1){
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
    }
    else return FUNCTION_SUCCESS;
}

int check_data_values(int data, FILE *input_file, char *input_file_name, image_struct_type *image_struct){
    if (data > MAX_GRAY || data < MIN_GRAY){
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
        }

    else return FUNCTION_SUCCESS;
}

int check_data_captured(image_struct_type *image_struct, FILE *input_file, char *input_file_name){
    if (image_struct->check != 1){
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
    }
    else return FUNCTION_SUCCESS;
}