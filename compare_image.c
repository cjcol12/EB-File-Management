// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

int comp_magic_number(image_struct_type *image_struct, image_struct_type *image_struct_compare){
    if (*(image_struct->magicNumberValue) != *(image_struct_compare->magicNumberValue)){ // free and exit
        free(image_struct->imageData);
        free(image_struct->imageData);
        printf("DIFFERENT\n");
        return FUNCTION_SUCCESS_DIFFERENT;
    }
    else return FUNCTION_SUCCESS;
}

int comp_dimensions(image_struct_type *image_struct, image_struct_type *image_struct_compare){
    if ((image_struct->height != image_struct_compare->height) || 
        (image_struct->width != image_struct_compare->width)){
            free(image_struct->imageData);
            free(image_struct_compare->imageData);
            printf("DIFFERENT\n");
            return FUNCTION_SUCCESS_DIFFERENT;
            } // free and exit
    else return FUNCTION_SUCCESS;
}

int comp_image_data(image_struct_type *image_struct, image_struct_type *image_struct_compare){
    // and check the pixel values
    for (int n=0; n<image_struct->numBytes; n++)
        {
        if(image_struct->imageData[n]!=image_struct_compare->imageData[n])
            { // free and exit
            free(image_struct->imageData);
            free(image_struct_compare->imageData);
            printf("DIFFERENT\n");
            return FUNCTION_SUCCESS_DIFFERENT;
            } // free and exit
        }
    return FUNCTION_SUCCESS;

}