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

// void construct_image(image_struct_type *image_struct){
//     image_struct->magic_number[0] = NULL;
//     image_struct->magic_number_value = NULL;

//     // create and initialise variables used within code
//     image_struct->width = NULL;
//     image_struct->height = NULL;
//     image_struct->check = NULL;

//     // image data 
//     image_struct->imageData = NULL;
//     image_struct->numBytes = NULL;
// }


// void destruct_image(image_struct_type *image_struct){
//     if ((image_struct->magic_number[0]) != NULL)
//         image_struct->magic_number[0] = NULL;
    
//     if (image_struct->magic_number[1] != NULL)
//         image_struct->magic_number[1] = NULL;

//     if (image_struct->magic_number_value != NULL)
//         image_struct->magic_number_value = NULL;

//     if (image_struct->width != NULL)
//         image_struct->width = NULL;

//     if (image_struct->height != NULL)
//         image_struct->height = NULL;

//     if (image_struct->check != NULL)
//         image_struct->check = NULL;

//     if (image_struct->imageData != NULL)
//         image_struct->imageData = NULL;
    
//     if (image_struct->numBytes != NULL)
//         image_struct->numBytes = NULL;

//     if (input_file != NULL)
//         input_file = NULL;
    
//     if (output_file != NULL)
//         output_file = NULL;
// }

int check_arg_count(int argc){
    // validate that user has entered 2 arguments
    if (argc != 3){
        printf("ERROR: Bad Arguments\n"); 
        return BAD_ARGUMENT_COUNT; 
    }
    else return FUNCTION_SUCCESS;
}

int check_file_opened(char *input_file_name, FILE *input_file){
    if (!input_file){ // check file pointer opened correctly
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
    
    // checking against the casted value due to endienness.
    if (*(image_struct->magic_number_value) != MAGIC_NUMBER_EB &&
        *(image_struct->magic_number_value) != MAGIC_NUMBER_EU){
        printf("ERROR: Bad Magic Number (%s)\n", input_file_name);
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
    image_struct->numBytes = image_struct->width * image_struct->height;
    // malloc for 2d array
    image_struct->imageData = (unsigned int **) malloc(image_struct->height * sizeof(unsigned int *));

    // testing return value of malloc
    if (image_struct->imageData == NULL){
        fclose(input_file);
        printf("ERROR: Image Malloc Failed\n");        
        return BAD_MALLOC;
    }
    

    unsigned int *data_block = (unsigned int *) malloc(image_struct->numBytes * sizeof(unsigned int));

    if (data_block == NULL){
        fclose(input_file);
        printf("ERROR: Image Malloc Failed\n");        
        return BAD_MALLOC;
    }

    for (int row = 0; row < image_struct->height; row++){
        image_struct->imageData[row] = data_block + row * image_struct->width; 
    }







    // for(int i = 0; i < image_struct->height; i++){
    //     image_struct->imageData[i] = 
    //     (unsigned int *) malloc(image_struct->width * sizeof(unsigned int));

    //     if (image_struct->imageData[i] == NULL){
    //         fclose(input_file);
    //         printf("ERROR: Image Malloc Failed\n");

    //         for(int j = 0; j < i; j++){
    //             free(image_struct->imageData[j]);
    //         }
    //         free(image_struct->imageData);
            
    //         return BAD_MALLOC;
    //     }
    // }
    // if malloc is unsuccessful, it will return a null pointer

    // return 0 on function success
    return FUNCTION_SUCCESS;
}

int read_data(
    image_struct_type *image_struct, char *input_file_name, FILE *input_file){

    for(int i = 0; i < 360; i++){
        for(int j = 0; j < image_struct->width; j++){
            unsigned int data;
            image_struct->check = fscanf(input_file, "%u", &data);

            if (image_struct->check != 1){
                fclose(input_file);
                printf("ERROR: Bad Data (%s)\n", input_file_name);
                return BAD_DATA;
            }
            
            image_struct->imageData[i][j] = data;


            
            // check data is within bounds (0 - 31)
            if (image_struct->imageData[i][j] > MAX_GRAY
            ||image_struct->imageData[i][j] < MIN_GRAY){
                
                // iterate through imageData to free 2nd dimension arrays
                // for(int i = 0; i < image_struct->height; i++){
                //     free(image_struct->imageData[i]);
                // }
                // free(image_struct->imageData);

                printf("ERROR: Bad Data (%s)\n", input_file_name);
                return BAD_DATA;
            }

            //printf("i is %d\n j is %d\n", i, j);
        }
    }
 
    
    // repeat fscanf to check if there is any data we haven't read in
    image_struct->check = fscanf(input_file, "%u", *image_struct->imageData);
    // image_struct.check = 
    //fscanf(input_file, "%u", &image_struct.imageData[current]); 

    // if there is more data, fscanf returns 1
    // if thats the case, we have too much data ( > numBytes)
    if (image_struct->check == 1){
        
        // iterate through imageData to free 2nd dimension arrays
        // for(int i = 0; i < image_struct->height; i++){
        //     free(image_struct->imageData[i]);
        // }
        // free(image_struct->imageData);

        fclose(input_file);
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        return BAD_DATA;
    }

    // Finished with input file - close it
    fclose(input_file);
    return FUNCTION_SUCCESS;
}

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
                return BAD_DATA;
            }
            value = (unsigned int) binary_value;

            image_struct->imageData[i][j] = value;

            if (image_struct->imageData[i][j] > MAX_GRAY
            ||image_struct->imageData[i][j] < MIN_GRAY){
                
                // iterate through imageData to free 2nd dimension arrays
                // for(int i = 0; i < image_struct->height; i++){
                //     free(image_struct->imageData[i]);
                // }
                // free(image_struct->imageData);

                printf("ERROR: Bad Data (%s)\n", input_file_name);
                return BAD_DATA;
            }
        }
    }

    return 0;
}




// non functional
int close_file_free_mem(
    image_struct_type *image_struct, char *executable_name, FILE *input_file){
    // iterate through imageData to free 2nd dimension arrays
    // for(int i = 0; i < image_struct->height; i++){
    //     free(image_struct->imageData[i]);
    // }
    // free(image_struct->imageData);

    fclose(input_file);
    printf("ERROR: Bad Data (%s)\n", executable_name);
    return BAD_DATA;
}