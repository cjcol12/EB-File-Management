// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Image struct header file inclusion
#include "image_structs.h"

// Definition header file inclusion
#include "definitions.h"

// image struct initialization
// TODO Remove from global
image_struct_type image_struct;

int unix_usage(char *executable_name, int argc){
    // Unix usage check
    if (argc == 1){
        printf("Usage: %s file1 file2", executable_name);
        // printf("Usage: ebfEcho file1 file2"); // static test still failing
        return USAGE_REQUEST;
    }
    else return 10;
}

int check_arg_count(char *executable_name, int argc){

    // validate that user has entered 2 arguments
    if (argc != 3)
        {
        printf("ERROR: Bad Arguments\n"); // runs twice in main
        return BAD_ARGUMENT_COUNT; 
        }
    else return 0;
}

int check_file_opened(char *executable_name, FILE *inputFile){
    if (!inputFile)
    { // check file pointer
    printf("ERROR: Bad File Name (%s)\n", executable_name);
    // printf("ERROR: Bad File Name (1)\n");

    return BAD_FILE;
    }
    else return 0;
}

int check_magic_number(char *executable_name, FILE *inputFile){
    image_struct.magicNumber[0] = getc(inputFile);
    image_struct.magicNumber[1] = getc(inputFile);

    // cast to short for endianness
    image_struct.magicNumberValue = (unsigned short *)image_struct.magicNumber;
    
    // checking against the casted value due to endienness.
    if (*(image_struct.magicNumberValue) != MAGIC_NUMBER){
        // check magic number
        printf("ERROR: Bad Magic Number (%s)\n", executable_name);
        return BAD_MAGIC_NUMBER;
    }
    else return 0;

}

int check_dimensions(char *executable_name, FILE *inputFile){
    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    image_struct.check = fscanf(inputFile, "%d %d", &image_struct.height, &image_struct.width);

    if (image_struct.check != 2 || 
        image_struct.height < MIN_DIMENSION || 
        image_struct.width < MIN_DIMENSION || 
        image_struct.height > MAX_DIMENSION || 
        image_struct.width > MAX_DIMENSION){
        
        // checks dimensions
        // close the file as soon as an error is found
        fclose(inputFile);
        // print appropriate error message and return
        printf("ERROR: Bad Dimensions (%s)\n", executable_name);
        return BAD_DIMENSION;
    } // check dimensions5
    else return 0;
}

int check_malloc(FILE *inputFile){
    // caclulate total size and allocate memory for array
    image_struct.numBytes = image_struct.height * image_struct.width;
    image_struct.imageData = (unsigned int *)malloc(image_struct.numBytes * sizeof(unsigned int));

    // if malloc is unsuccessful, it will return a null pointer
    if (image_struct.imageData == NULL){
        // check malloc
        fclose(inputFile);
        printf("ERROR: Image Malloc Failed\n");
        return BAD_MALLOC;
    }
    else return 0;
}

int read_data(char *executable_name, FILE *inputFile){
    // read in each grey value from the file
    for (int current = 0; current < image_struct.numBytes; current++)
        { // reading in
        image_struct.check = fscanf(inputFile, "%u", &image_struct.imageData[current]);
        // validate that we have captured 1 pixel value
        if (image_struct.check != 1)
            { // check inputted data
            // ensure that allocated data is freed before exit.
            free(image_struct.imageData);
            fclose(inputFile);
            printf("ERROR: Bad Data\n");
            return BAD_DATA;
            } // check inputted data
        if (*(image_struct.imageData) > MAX_GRAY){
            free(image_struct.imageData);
            fclose(inputFile);
            printf("ERROR: BAD Data\n");
            return BAD_DATA;
        }
        }
    return 0;
}

int write_header(FILE *outputFile){
    // write the header data in one block
    image_struct.check = fprintf(outputFile, "eb\n%d %d\n", image_struct.height, image_struct.width);
    // and use the return from fprintf to check that we wrote.
    if (image_struct.check == 0) 
        { // check write
        fclose(outputFile);
        free(image_struct.imageData);
        printf("ERROR: Bad Output\n");
        return BAD_OUTPUT;
        } // check write
    else return 0;
}

int write_image_data(FILE *outputFile){
    // iterate though the array and print out pixel values
    for (int current = 0; current < image_struct.numBytes; current++)
        { // writing out
        // if we are at the end of a row ((current+1)%width == 0) then write a newline, otherwise a space.
        image_struct.check = fprintf(outputFile, "%u%c", image_struct.imageData[current], ((current + 1) % image_struct.width) ? ' ' : '\n');
        if (image_struct.check == 0)
            { // check write
            fclose(outputFile);
            free(image_struct.imageData);
            printf("ERROR: Bad Output\n");
            return BAD_OUTPUT;
            } // check write
        } // writing out
    return 0;
}
int main(int argc, char **argv)
    { // main

    // Unix usage information
    if (unix_usage(argv[0], argc) == 0)
        return 0;
    

    // running tests on argument counts
    if (check_arg_count(argv[0], argc) == BAD_ARGUMENT_COUNT)
        return BAD_ARGUMENT_COUNT;


    // open the input file in read mode
    FILE *inputFile = fopen(argv[1], "r");

    // check file opened successfully
    if (check_file_opened(argv[0], inputFile) == BAD_FILE)
        return BAD_FILE;

    if (check_magic_number(argv[0], inputFile) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;
    
    if (check_dimensions(argv[0], inputFile) == BAD_DIMENSION)
        return BAD_DIMENSION;

    if(check_malloc(inputFile) == BAD_MALLOC)
        return BAD_MALLOC;

    if (read_data(argv[0], inputFile) == BAD_DATA)
        return BAD_DATA;

    // now we have finished using the inputFile we should close it
    fclose(inputFile);

    // open the output file in write mode
    FILE *outputFile = fopen(argv[2], "w");
    // validate that the file has been opened correctly
    if (check_file_opened(argv[0], outputFile) == BAD_WRITE_PERMISSIONS)
        return BAD_WRITE_PERMISSIONS;

    // write output file header and data


    write_header(outputFile);
    write_image_data(outputFile);

    // free allocated memory before exit
    free(image_struct.imageData);
    // close the output file before exit
    fclose(outputFile);

    // print final success message and return
    printf("ECHOED\n");
    return SUCCESS;
    } // main()