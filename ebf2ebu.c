/*  Function: Read in a .ebf file and echo (copy) its contents to another file
    
    Arguments: Expects 3 arguments: ./ebfEcho input_file, output_file
    
    Returns: 0 on success, different values depending on error - found in 
    definitions.h

    Author: CJ Coleman
*/


// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

// Read image module inclusion
#include "read_image.c"

// Read image module inclusion
#include "write_image.c"

int main(int argc, char **argv){
    // image struct variable initialization
    image_struct_type image_struct;

    // Unix usage information
    // Returns 0 if program is run with no arguments
    if (argc == 1){
        printf("Usage: ebfEcho file1 file2");
        return USAGE_REQUEST;
    }

    // validates number of arguments
    // Parameters: argc - to test
    // Returns 0 on success or 1 on failure
    if (check_arg_count(argc) == BAD_ARGUMENT_COUNT)
        return BAD_ARGUMENT_COUNT;

    // open the input file in read mode
    FILE *input_file = fopen(argv[1], "r");

    // check to see if file opened successfully
    // Parameters: argv[1] - for error statements, input_file - the file to test
    // Returns 0 on success or 2 on failure
    if (check_file_opened(argv[1], input_file) == BAD_FILE)
        return BAD_FILE;

    // checks if the magic number is what we expect
    // Parameters: image_struct, argv[1] - for error statements, input_file - 
    // the file to test
    // Returns: 0 on success, 3 on failure
    if (check_magic_number(&image_struct, argv[1], input_file) == 
        BAD_MAGIC_NUMBER) 
        return BAD_MAGIC_NUMBER;
    
    // checks dimensions are within specified range(MIN_DIMENSION-MAX_DIMENSION)
    // Parameters: image_struct, argv[1] - for error statements, input_file - 
    // the file to test
    // Returns 0 on success, 4 on failure
    if (check_dimensions(&image_struct,  argv[1], input_file) == BAD_DIMENSION)
        return BAD_DIMENSION;

    // checks memory has been allocated properly for 2d array
    // Parameters: image_struct, input_file - the file to test
    // Returns 0 on success, 5 on failure
    if(check_malloc(&image_struct, input_file) == BAD_MALLOC)
        return BAD_MALLOC;

    // reads data into 2d array and checks data is valid
    // e.g within MIN_GRAY - MAX_GRAY and correct amounts of data read
    // Parameters image_struct, argv[1] - for error statements, input_file - 
    // the file to test 
    // Returns 0 on success, 6 on failure
    if (read_data(&image_struct, argv[1], input_file) == BAD_DATA)
        return BAD_DATA;


    // open the output file in write mode
    FILE *output_file = fopen(argv[2], "wb");
    image_struct.magic_number[1] = 'u';

    // checks we can write to output_file
    // Parameters: Parameters image_struct, argv[1] - for error statements, 
    // output_file - the file to test 
    // Return: returns 0 on success returns 8 on failure
    if (check_bad_output(&image_struct, output_file, argv[2]) == 
    BAD_WRITE_PERMISSIONS)
        return BAD_WRITE_PERMISSIONS;

    // Writes the header of the output file
    // Parameters: image_struct, output_file - the file to write to 
    // Return: returns 0 on success returns 8 on failure
    if (write_header(&image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;


    printf("CONVERTED\n");
    return SUCCESS;
}