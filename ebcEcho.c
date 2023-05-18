/* 
    @file ebfEcho.c
    @brief Program to read a .ebf file and echo (copy) its contents to another file.

    This program reads a .ebf file and echoes (copies) its contents to another file.

    Usage: ./ebfEcho input_file output_file

    Author: CJ Coleman
*/

#include <stdio.h>      // Standard I/O header file inclusion
#include <stdlib.h>     // Standard library header file inclusion
#include "definitions.h"   // Definition header file inclusion
#include "read_image.c"     // Read image module inclusion
#include "write_image.c"    // Write image module inclusion
#include "binary_management.c"  // Binary function inclusion

int main(int argc, char **argv)
{
    // image struct variable initialization
    image_struct_type image_struct;

    // Unix usage information
    // Returns 0 if program is run with no arguments
    if (argc == 1)
    {
        printf("Usage: ebcEcho file1 file2");
        return USAGE_REQUEST;
    }

    // validates number of arguments
    if (check_arg_count(argc) == BAD_ARGUMENT_COUNT)
        return BAD_ARGUMENT_COUNT;

    // open the input file in read mode
    FILE *input_file = fopen(argv[1], "rb");

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

    // open the output file in write mode
    FILE *output_file = fopen(argv[2], "wb");

    // checks we can write to output_file
    if (check_bad_output(&image_struct, output_file, argv[2]) == BAD_WRITE_PERMISSIONS)
        return BAD_WRITE_PERMISSIONS;

    // Writes the header of the output file
    if (write_header(&image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;

    compress_data_to_file(&image_struct, output_file);

    // frees malloc'd memory and closes the output file
    destructor(&image_struct, output_file);
    free(image_struct.array);

    // Print final success message and return 0 on success
    printf("ECHOED\n");
    return SUCCESS;
}
