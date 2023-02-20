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

int main(int argc, char **argv)
{

    // Unix usage information
    if (unix_usage(argc) == 0)
        return 0;

    // running tests on argument counts
    if (check_arg_count(argv[0], argc) == BAD_ARGUMENT_COUNT)
        return BAD_ARGUMENT_COUNT;

    // open the input file in read mode
    FILE *inputFile = fopen(argv[1], "r");

    // check file opened successfully
    if (check_file_opened(argv[1], inputFile) == BAD_FILE)
        return BAD_FILE;

    if (check_magic_number(argv[1], inputFile) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;
    
    if (check_dimensions(argv[1], inputFile) == BAD_DIMENSION)
        return BAD_DIMENSION;

    if(check_malloc(inputFile) == BAD_MALLOC)
        return BAD_MALLOC;

    if (read_data(argv[1], inputFile) == BAD_DATA)
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