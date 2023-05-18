/* 
    @file ebcComp.c
    @brief Program to read two .ebu files and compare their contents for identity.

    This program reads two .ebu files and compares their contents to determine if they are identical.

    Usage: ./ebuComp inputFile comparison_file

    Author: CJ Coleman
*/

#include <stdio.h>      // Standard I/O header file inclusion
#include <stdlib.h>     // Standard library header file inclusion
#include "definitions.h"   // Definition header file inclusion
#include "read_image.c"     // Read image module inclusion
#include "compare_image.c"    // Compare image module inclusion
#include "binary_management.c"  // Binary function inclusion

int main(int argc, char **argv)
{
    // image struct variable initialization
    image_struct_type image_struct;

    // Unix usage information
    // Returns 0 if program is run with no arguments
    if (argc == 1)
    {
        printf("Usage: ebcComp file1 file2");
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
    if (check_magic_number(&image_struct, argv[1], input_file) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;

    // checks dimensions are within specified range (MIN_DIMENSION-MAX_DIMENSION)
    if (check_dimensions(&image_struct, argv[1], input_file) == BAD_DIMENSION)
        return BAD_DIMENSION;

    // checks memory has been allocated properly for 2d array
    if (check_malloc(&image_struct, input_file) == BAD_MALLOC)
        return BAD_MALLOC;

    image_struct.compressed_width = round_up_return(&image_struct);
    image_struct.numBytes = image_struct.height * image_struct.width;
    image_struct.compressed_numBytes = image_struct.height * image_struct.compressed_width;

    check_1d_malloc(&image_struct, input_file);

    if (decompress_and_store(&image_struct, input_file, argv[1]) == BAD_DATA){
        return BAD_DATA;
    }

    one_dim_two_dim(&image_struct);

    // Image struct variable initialization (comparison)
    image_struct_type image_struct_compare;

    // Open input file 2 in read mode
    FILE *comparison_File = fopen(argv[2], "r");

    // check to see if file opened successfully
    if (check_file_opened(argv[2], comparison_File) == BAD_FILE)
        return BAD_FILE;

    // checks if the magic number is what we expect
    if (check_magic_number(&image_struct_compare, argv[2], comparison_File) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;

    // checks dimensions are within specified range (MIN_DIMENSION-MAX_DIMENSION)
    if (check_dimensions(&image_struct_compare, argv[2], comparison_File) == BAD_DIMENSION)
        return BAD_DIMENSION;

    // checks memory has been allocated properly for 2d array
    if (check_malloc(&image_struct_compare, comparison_File) == BAD_MALLOC)
        return BAD_MALLOC;

    image_struct_compare.compressed_width = round_up_return(&image_struct_compare);
    image_struct_compare.numBytes = image_struct_compare.height * image_struct_compare.width;
    image_struct_compare.compressed_numBytes = image_struct_compare.height * image_struct_compare.compressed_width;

    check_1d_malloc(&image_struct_compare, input_file);

    if (decompress_and_store(&image_struct_compare, input_file, argv[1]) == BAD_DATA){
        return BAD_DATA;
    }

    one_dim_two_dim(&image_struct_compare);

    // File comparison functions

    // checks if the magic numbers are identical
    if (comp_magic_number(&image_struct, &image_struct_compare) == FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS_DIFFERENT;

    // checks if the dimensions are identical
    if (comp_dimensions(&image_struct, &image_struct_compare) == FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS_DIFFERENT;

    // checks if the image data is identical
    if (comp_image_data(&image_struct, &image_struct_compare) == FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS_DIFFERENT;

    // free(image_struct.data_block);  // freeing data_block
    // image_struct.data_block = NULL;
    // free(image_struct.imageData);  // freeing imageData array of pointers
    // image_struct.imageData = NULL;
    // free(image_struct.array);  // freeing array
    // image_struct.array = NULL;

    // free(image_struct.array);
    // fclose(input_file);
    // fclose(comparison_File);

    // destructor(&image_struct, input_file);
    

    // If we have not exited on different data, they must be identical
    printf("IDENTICAL\n");
    return SUCCESS;
}
