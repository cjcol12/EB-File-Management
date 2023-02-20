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
    image_struct->numBytes = image_struct->height * image_struct->width;
    image_struct->imageData = (unsigned int *)malloc(image_struct->numBytes * sizeof(unsigned int));

    // if malloc is unsuccessful, it will return a null pointer
    if (image_struct->imageData == NULL){
        fclose(inputFile);
        printf("ERROR: Image Malloc Failed\n");
        return BAD_MALLOC;
    }

    // return 0 on function success
    else return FUNCTION_SUCCESS;
}

int read_data(image_struct_type *image_struct, char *executable_name, FILE *inputFile){
    // read in each grey value from the file
    int current; // declaration moved outside of for loop so it can be used after
    for (current = 0; current < image_struct->numBytes; current++){
        image_struct->check = fscanf(inputFile, "%u", &image_struct->imageData[current]);

        // validate that we have captured exactly 1 pixel value
        if (image_struct->check != 1){
            // ensure that allocated data is freed before exit->
            free(image_struct->imageData);
            fclose(inputFile);
            printf("ERROR: Bad Data (%s)\n", executable_name);
            return BAD_DATA;
        }

        // check data is within bounds (0 - 31)
        if (image_struct->imageData[current] > MAX_GRAY
          ||image_struct->imageData[current] < MIN_GRAY){ // not needed as rolls around
            free(image_struct->imageData);
            fclose(inputFile);
            printf("ERROR: Bad Data (%s)\n", executable_name);
            return BAD_DATA;
        }
    }
        
    // repeat fscanf to check if there is any data we haven't read in
    image_struct->check = fscanf(inputFile, "%u", &image_struct->imageData[current]);
    // image_struct.check = fscanf(inputFile, "%u", &image_struct.imageData[current]); // uncomment if empty line expected at EOF

    // if there is more data, fscanf returns 1
    // if thats the case, we have too much data ( > numBytes)
    if (image_struct->check == 1){
        free(image_struct->imageData);
        fclose(inputFile);
        printf("ERROR: Bad Data (%s)\n", executable_name);
        return BAD_DATA;
    }

    // return 0 on function success
    return FUNCTION_SUCCESS;
}