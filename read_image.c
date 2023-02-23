// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

#include "image_structs.h"

int check_arg_count(char *executable_name, int argc){
    // validate that user has entered 2 arguments
    if (argc != 3){
        printf("ERROR: Bad Arguments\n"); 
        return BAD_ARGUMENT_COUNT; 
    }
    else return FUNCTION_SUCCESS;
}

int check_file_opened(char *executable_name, FILE *inputFile){
    if (!inputFile){ // check file pointer opened correctly
        printf("ERROR: Bad File Name (%s)\n", executable_name);
        return BAD_FILE;
    }

    // return 0 on function success
    else return FUNCTION_SUCCESS;
}

int check_magic_number(image_struct_type *image_struct, char *executable_name, FILE *inputFile){
    // stores first two characters of file into character array
    image_struct->magicNumber[0] = getc(inputFile);
    image_struct->magicNumber[1] = getc(inputFile);

    // cast to short for endianness
    image_struct->magicNumberValue = (unsigned short *)image_struct->magicNumber;
    
    // checking against the casted value due to endienness.
    if (*(image_struct->magicNumberValue) != MAGIC_NUMBER){
        printf("ERROR: Bad Magic Number (%s)\n", executable_name);
        return BAD_MAGIC_NUMBER;
    }

    // return 0 on function success
    else return FUNCTION_SUCCESS;
}

int check_dimensions(image_struct_type *image_struct, char *executable_name, FILE *inputFile){
    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    image_struct->check = fscanf(inputFile, "%d %d", &image_struct->height, &image_struct->width);

    // checks we captured two return values
    // checks dimensions are valid
    if (image_struct->check != 2 || 
        image_struct->height < MIN_DIMENSION || 
        image_struct->width < MIN_DIMENSION || 
        image_struct->height > MAX_DIMENSION || 
        image_struct->width > MAX_DIMENSION){
        
        fclose(inputFile);
        printf("ERROR: Bad Dimensions (%s)\n", executable_name);
        return BAD_DIMENSION;
    }

    // return 0 on function success
    else return FUNCTION_SUCCESS;
}

int check_malloc(image_struct_type *image_struct, FILE *inputFile){
    // caclulate total size and allocate memory for array
    // image_struct->numBytes = image_struct->height * image_struct->width;
    
    // malloc for 2d array
    image_struct->imageData = (unsigned int **) malloc(image_struct->height * sizeof(unsigned int *));

    if (image_struct->imageData == NULL){
        fclose(inputFile);
        // free(image_struct->imageData);
        printf("ERROR: Image Malloc Failed\n");
        return BAD_MALLOC;
    }


    for(int i = 0; i < image_struct->height; i++){
        image_struct->imageData[i] = (unsigned int *) malloc(image_struct->width * sizeof(unsigned int));

        if (image_struct->imageData[i] == NULL){
            fclose(inputFile);
            printf("ERROR: Image Malloc Failed\n");

            for(int j = 0; j < i; j++){
                free(image_struct->imageData[j]);
            }
            
            free(image_struct->imageData);
            return BAD_MALLOC;
        }
    }
    // if malloc is unsuccessful, it will return a null pointer

    // return 0 on function success
    return FUNCTION_SUCCESS;
}

int read_data(image_struct_type *image_struct, char *executable_name, FILE *inputFile){

    for(int i = 0; i < image_struct->height; i++){
        for(int j = 0; j < image_struct->width; j++){
            unsigned int data;
            image_struct->check = fscanf(inputFile, "%u", &data);
            image_struct->imageData[i][j] = data;

            if (image_struct->check != 1){

                // iterate through imageData to free 2nd dimension arrays
                for(int i = 0; i < image_struct->height; i++){
                    free(image_struct->imageData[i]);
                }
                free(image_struct->imageData);

                fclose(inputFile);
                printf("ERROR: Bad Data (%s)\n", executable_name);
                return BAD_DATA;
            }
            // check data is within bounds (0 - 31)
            if (image_struct->imageData[i][j] > MAX_GRAY
            ||image_struct->imageData[i][j] < MIN_GRAY){ // not needed as rolls around
                
                // iterate through imageData to free 2nd dimension arrays
                for(int i = 0; i < image_struct->height; i++){
                    free(image_struct->imageData[i]);
                }
                free(image_struct->imageData);

                printf("ERROR: Bad Data (%s)\n", executable_name);
                return BAD_DATA;
            }
        }
    }

    // repeat fscanf to check if there is any data we haven't read in
    image_struct->check = fscanf(inputFile, "%u", *image_struct->imageData);
    // image_struct.check = fscanf(inputFile, "%u", &image_struct.imageData[current]); // uncomment if empty line expected at EOF

    // if there is more data, fscanf returns 1
    // if thats the case, we have too much data ( > numBytes)
    if (image_struct->check == 1){
        
        // iterate through imageData to free 2nd dimension arrays
        for(int i = 0; i < image_struct->height; i++){
            free(image_struct->imageData[i]);
        }
        free(image_struct->imageData);

        fclose(inputFile);
        printf("ERROR: Bad Data (%s)\n", executable_name);
        return BAD_DATA;
    }
    return FUNCTION_SUCCESS;
}

int close_file_free_mem(image_struct_type *image_struct, char *executable_name, FILE *inputFile){
    // iterate through imageData to free 2nd dimension arrays
    for(int i = 0; i < image_struct->height; i++){
        free(image_struct->imageData[i]);
    }
    free(image_struct->imageData);

    fclose(inputFile);
    printf("ERROR: Bad Data (%s)\n", executable_name);
    return BAD_DATA;
}