// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

// Read image module inclusion
#include "read_image.c"

// Write image module inclusion
#include "write_image.c"

int unix_usage(int argc){
    // Unix usage check - runs with no arguments
    if (argc == 1){
        printf("Usage: ebfEcho file1 file2");
        return USAGE_REQUEST;
    }

    // return 10 on function success
    else return 10;  // is this return value valid?
}

int main(int argc, char **argv){
    // image struct variable initialization
    image_struct_type image_struct;

    // Unix usage information
    if (unix_usage(argc) == 0)
        return 0;

    // running tests on argument counts
    if (check_arg_count(argv[0], argc) == BAD_ARGUMENT_COUNT)
        return BAD_ARGUMENT_COUNT;

    // open the input file in read mode
    FILE *inputFile = fopen(argv[1], "r");

    if (check_file_opened(argv[1], inputFile) == BAD_FILE)
        return BAD_FILE;

    if (check_magic_number(&image_struct, argv[1], inputFile) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;
    
    if (check_dimensions(&image_struct,  argv[1], inputFile) == BAD_DIMENSION)
        return BAD_DIMENSION;

    if(check_malloc(&image_struct, inputFile) == BAD_MALLOC)
        return BAD_MALLOC;

    if (read_data(&image_struct, argv[1], inputFile) == BAD_DATA)
        return BAD_DATA;

    // finised with input file - close it
    fclose(inputFile);


    FILE *outputFile = fopen(argv[2], "w");
    // open the output file in write mode
    if (check_bad_output(&image_struct, outputFile, argv[2]) == BAD_WRITE_PERMISSIONS)
        return BAD_WRITE_PERMISSIONS;


    // validate that the file has been opened correctly
    // if (check_file_opened(argv[2], outputFile) == BAD_WRITE_PERMISSIONS)
    //     return BAD_WRITE_PERMISSIONS;

    // write output file header and data

    if (write_header(&image_struct, outputFile) == BAD_OUTPUT)
        return BAD_OUTPUT;

    if (write_image_data(&image_struct, outputFile) == BAD_OUTPUT)
        return BAD_OUTPUT;

    // free allocated memory before exit
    free(image_struct.imageData);
    // close the output file before exit
    fclose(outputFile);

    // print final success message and return
    printf("ECHOED\n");
    return SUCCESS;
}