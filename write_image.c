// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

#include "image_structs.h"

int check_bad_output(image_struct_type *image_struct, FILE *output_file, char *executable_name){
    if (output_file == NULL){
        printf("ERROR: Bad Output(%s)", executable_name);
        return BAD_WRITE_PERMISSIONS;
    }
    else return FUNCTION_SUCCESS;
}

int write_header(image_struct_type *image_struct, FILE *output_file){
    // write the header data in one block
    // change to write with magicNumberValue
    image_struct->check = fprintf(output_file, "%s\n%d %d\n", image_struct->magic_number, image_struct->height, image_struct->width);
    // and use the return from fprintf to check that we wrote.
    if (image_struct->check == 0) 
        { // check write
        fclose(output_file);
        free(image_struct->imageData);
        printf("ERROR: Bad Output\n");
        return BAD_OUTPUT;
        } // check write
    else return FUNCTION_SUCCESS;
}

int write_image_data(image_struct_type *image_struct, FILE *output_file){
    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){
            // iterating through 2d array and writing
            // if fprintf fails, return 0 for error checking
            image_struct->check = fprintf(output_file, "%u ", image_struct->imageData[i][j]);

            if (image_struct->check == 0){
                fclose(output_file);

                // iterate through imageData to free 2nd dimension arrays
                for(int i = 0; i < image_struct->height; i++){
                    free(image_struct->imageData[i]);
                }
                free(image_struct->imageData);

                printf("ERROR: Bad Output\n");
                return BAD_OUTPUT;
            }
        }
        fprintf(output_file, "\n");
    }

    // free allocated memory before exit
    for(int i = 0; i < image_struct->height; i++){
        free(image_struct->imageData[i]);
    }
    free(image_struct->imageData);
    fclose(output_file);

    return FUNCTION_SUCCESS;
}