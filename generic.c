// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Image structure header file inclusion
#include "image_structs.h"


void destructor(image_struct_type *image_struct, FILE *file){
    if(image_struct->data_block != NULL){
        free(image_struct->data_block);
    }

    if(image_struct->imageData != NULL){
        free(image_struct->imageData);
    }

    if (file != NULL){
        fclose(file);
    }
}