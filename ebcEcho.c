/*  Function: Read in a .ebf file and echos (copy) its contents to another file

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

// Write image module inclusion
#include "write_image.c"

#include "binary_management.c"

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
    {
        return BAD_FILE;
    }

    // checks if the magic number is what we expect
    if (check_magic_number(&image_struct, argv[1], input_file) ==
        BAD_MAGIC_NUMBER)
    {
        return BAD_MAGIC_NUMBER;
    }

    // checks dimensions are within specified range(MIN_DIMENSION-MAX_DIMENSION)
    if (check_dimensions(&image_struct, argv[1], input_file) == BAD_DIMENSION)
    {
        return BAD_DIMENSION;
    }


    // removes redundant block at EOF
    // image_struct.height -= 1;
    // checks memory has been allocated properly for 2d array
    if (check_malloc(&image_struct, input_file) == BAD_MALLOC)
    {
        return BAD_MALLOC;
    }


    // reverts to correct size for header writing
    // image_struct.height += 1;


    // int temp_width = image_struct.width;

    // find the size of width of compressed file ~ 0.625 original
    // image_struct.width = round_up_return(&image_struct);

    // reads data into 2d array and checks data is valid
    // e.g within MIN_GRAY - MAX_GRAY and correct amounts of data read
    // if (read_compressed_data(&image_struct, argv[1], input_file) == BAD_DATA)
    // {
    //     return BAD_DATA;
    // }

    image_struct.compressed_width = round_up_return(&image_struct);
    
    decompress_and_store(&image_struct, input_file);

    



    // open the output file in write mode
    FILE *output_file = fopen(argv[2], "w");

    // checks we can write to output_file
    if (check_bad_output(&image_struct, output_file, argv[2]) ==
        BAD_WRITE_PERMISSIONS)
    {
        return BAD_WRITE_PERMISSIONS;
    }

    // image_struct.width = temp_width;

    // Writes the header of the output file
    if (write_header(&image_struct, output_file) == BAD_OUTPUT)
    {
        return BAD_OUTPUT;
    }

    // image_struct.height -= 1;

    // find the size of width of compressed file ~ 0.625 original
    // image_struct.width = round_up_return(&image_struct);

    // write_binary_data(&image_struct, output_file);
    //image_struct.width = temp_width;

    // printf("uncompressed data\n");
    // for (int i = 0; i < image_struct.height; i++){
    //     // printf("%d\t", i + 1);
    //     for (int j = 0; j < image_struct.width; j++){
    //         printf("%d  ", image_struct.imageData[i][j]);
    //     }
    //     printf("\n\n");
    // }
    compress_data_to_file(&image_struct, output_file);
    // frees malloc'd memory and closes the output file
    destructor(&image_struct, output_file);

    // Print final success message and return 0 on success
    printf("ECHOED\n");
    return SUCCESS;
}