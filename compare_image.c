/*  Function: Module used in ebfComp to compare images.
    
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
#include "compare_image.h"

// compares magic_number
int comp_magic_number(
    image_struct_type *image_struct, image_struct_type *image_struct_compare){

    // compares magic number of two structs
    if (*(image_struct->magic_number_value) != 
    *(image_struct_compare->magic_number_value)){

        printf("DIFFERENT\n");
        destructor_no_file(image_struct);
        destructor_no_file(image_struct_compare);
        return FUNCTION_SUCCESS_DIFFERENT;
    }
    else return FUNCTION_SUCCESS;
}

int comp_dimensions(
    image_struct_type *image_struct, image_struct_type *image_struct_compare){

    // compares if height and width are the same in both images
    if ((image_struct->height != image_struct_compare->height) || 
        (image_struct->width != image_struct_compare->width)){

            printf("DIFFERENT\n");
            destructor_no_file(image_struct);
            destructor_no_file(image_struct_compare);
            return FUNCTION_SUCCESS_DIFFERENT;
            }
    else return FUNCTION_SUCCESS;
}

int comp_image_data(
    image_struct_type *image_struct, image_struct_type *image_struct_compare){
    // iterate through 2d array
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){

        // compare pixel values
        if(image_struct->imageData[i][j] != 
        image_struct_compare->imageData[i][j]){
            
            printf("DIFFERENT\n");
            destructor_no_file(image_struct);
            destructor_no_file(image_struct_compare);
            return FUNCTION_SUCCESS_DIFFERENT;
            }
        }
    }
    return FUNCTION_SUCCESS;
}