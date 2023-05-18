/**
 * @file ebuEcho.c
 * @brief Read in a .ebu file and echo (copy) its contents to another file.
 *
 * This program reads in a .ebu file, validates its format, and copies its contents to another file.
 *
 * Arguments: Expects 3 arguments: ./ebuEcho input_file, output_file
 * Returns: 0 on success, different values depending on error - found in definitions.h
 * Author: CJ Coleman
 */

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "read_image.c"
#include "write_image.c"
#include "binary_management.c"

/**
 * @brief Main function of the ebuEcho program.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on success, different values depending on error.
 */
int main(int argc, char **argv)
{
    // image struct variable initialization
    image_struct_type image_struct;

    // Unix usage information
    // Returns 0 if program is run with no arguments
    if (argc == 1)
    {
        printf("Usage: ebuEcho file1 file2");
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

    // open the output file in write mode
    FILE *output_file = fopen(argv[2], "wb");

    // checks we can write to output_file
    if (check_bad_output(&image_struct, output_file, argv[2]) ==
        BAD_WRITE_PERMISSIONS)
        return BAD_WRITE_PERMISSIONS;

    // Writes the header of the output file
    if (write_header(&image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;

    // Writes the data to the output file
    if (write_binary_data(&image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;

    // frees malloc'd memory and closes the output file
    destructor(&image_struct, output_file);

    printf("ECHOED\n");
    return SUCCESS;
}
