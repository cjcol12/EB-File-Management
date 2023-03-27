/*  Function: Read in two .ebu files and compare their contents to see if theyre
    idetical.

    Arguments: Expects 3 arguments: ./ebuComp inputFile, comparison_file

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

// Write image module inclusion
#include "compare_image.c"

// Binary function inclusion
#include "binary_management.c"

int main(int argc, char **argv)
{
    // image struct variable initialization
    image_struct_type image_struct;

    // Unix usage information
    // Returns 0 if program is run with no arguments
    if (argc == 1)
    {
        printf("Usage: ebuComp file1 file2");
        return USAGE_REQUEST;
    }

    // validates number of arguments
    if (check_arg_count(argc) == BAD_ARGUMENT_COUNT)
        return BAD_ARGUMENT_COUNT;

    // open the input file in read mode
    FILE *input_file = fopen(argv[1], "r");

    // check to see if file opened successfully
    if (check_file_opened(argv[1], input_file) == BAD_FILE)
        return BAD_FILE;

    // checks if the magic number is what we expect
    if (check_magic_number(&image_struct, argv[1], input_file) ==
        BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;

    // checks dimensions are within specified range(MIN_DIMENSION-MAX_DIMENSION)
    if (check_dimensions(&image_struct, argv[1], input_file) == BAD_DIMENSION)
        return BAD_DIMENSION;

    // checks memory has been allocated properly for 2d array
    if (check_malloc(&image_struct, input_file) == BAD_MALLOC)
        return BAD_MALLOC;

    // reads data into 2d array and checks data is valid
    // e.g within MIN_GRAY - MAX_GRAY and correct amounts of data read
    if (read_binary_data(&image_struct, argv[1], input_file) == BAD_DATA)
        return BAD_DATA;

    // Image struct variable initialization (comparison)
    image_struct_type image_struct_compare;

    // Open input file 2 in read mode
    FILE *comparison_File = fopen(argv[2], "r");

    // check to see if file opened successfully
    if (check_file_opened(argv[2], comparison_File) == BAD_FILE)
        return BAD_FILE;

    // checks if the magic number is what we expect
    if (check_magic_number(&image_struct_compare, argv[2], comparison_File) ==
        BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;

    // checks dimensions are within specified range(MIN_DIMENSION-MAX_DIMENSION)
    if (check_dimensions(&image_struct_compare, argv[2], comparison_File) ==
        BAD_DIMENSION)
        return BAD_DIMENSION;

    // checks memory has been allocated properly for 2d array
    if (check_malloc(&image_struct_compare, comparison_File) == BAD_MALLOC)
        return BAD_MALLOC;

    // reads data into 2d array and checks data is valid
    // e.g within MIN_GRAY - MAX_GRAY and correct amounts of data read
    if (read_binary_data(&image_struct_compare, argv[2], comparison_File) == BAD_DATA)
        return BAD_DATA;

    // File comparison functions
    // checks if the magic numbers are identical
    if (comp_magic_number(&image_struct, &image_struct_compare) ==
        FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS_DIFFERENT;

    // checks if the dimensions are identical
    if (comp_dimensions(&image_struct, &image_struct_compare) ==
        FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS_DIFFERENT;

    // checks if the data is identical
    if (comp_image_data(&image_struct, &image_struct_compare) ==
        FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS_DIFFERENT;

    // frees malloc'd memory and closes the output file
    destructor_no_file(&image_struct);
    destructor_no_file(&image_struct_compare);

    // If we have not exited on different data, must be identical
    printf("IDENTICAL\n");
    return SUCCESS;
}
