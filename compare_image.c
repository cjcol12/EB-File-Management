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

int comp_magic_number(
    image_struct_type *image_struct, image_struct_type *image_struct_compare){
    if (*(image_struct->magic_number_value) != 
    *(image_struct_compare->magic_number_value)){

        free(image_struct->imageData);
        printf("DIFFERENT\n");
        return FUNCTION_SUCCESS_DIFFERENT;
    }
    else return FUNCTION_SUCCESS;
}

int comp_dimensions(
    image_struct_type *image_struct, image_struct_type *image_struct_compare){
    if ((image_struct->height != image_struct_compare->height) || 
        (image_struct->width != image_struct_compare->width)){

            free(image_struct->imageData);
            free(image_struct_compare->imageData);
            printf("DIFFERENT\n");
            return FUNCTION_SUCCESS_DIFFERENT;
            } // free and exit
    else return FUNCTION_SUCCESS;
}

int comp_image_data(
    image_struct_type *image_struct, image_struct_type *image_struct_compare){
    // and check the pixel values
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){

        if(image_struct->imageData[i][j] != 
        image_struct_compare->imageData[i][j]){
            
            free(image_struct->imageData);
            free(image_struct_compare->imageData);
            printf("DIFFERENT\n");
            return FUNCTION_SUCCESS_DIFFERENT;
            }
        }
    }
    return FUNCTION_SUCCESS;
}