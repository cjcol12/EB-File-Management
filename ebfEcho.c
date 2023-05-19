/* 
    @file ebfEcho.c
    @brief Program to read a .ebf file and echo (copy) its contents to another file.

    This program reads a .ebf file, copies its contents, and writes them to another file.

    Usage: ./ebfEcho input_file output_file

    Author: CJ Coleman
*/

#include <stdio.h>      // Standard I/O header file inclusion
#include <stdlib.h>     // Standard library header file inclusion
#include "definitions.h"   // Definition header file inclusion
#include "read_image.c"     // Read image module inclusion
#include "write_image.c"    // Write image module inclusion
#include "error_checking.h" // Error checking module inclusion

<<<<<<< HEAD
int main(int argc, char **argv)
{
=======
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

int main(int argc, char **argv){
>>>>>>> 0d7f47695274b2a53e0b80fc1762bf5f30e2ac10
    // image struct variable initialization
    image_struct_type image_struct;

    // Unix usage information
    // Returns 0 if program is run with no arguments
    if (argc == 1)
    {
        printf("Usage: ebfEcho file1 file2");
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
<<<<<<< HEAD

    // checks dimensions are within specified range (MIN_DIMENSION-MAX_DIMENSION)
    if (check_dimensions(&image_struct, argv[1], input_file) == BAD_DIMENSION)
=======
    }
    
    // checks dimensions are within specified range(MIN_DIMENSION-MAX_DIMENSION)
    // Parameters: image_struct, argv[1] - for error statements, input_file - 
    // the file to test
    // Returns 0 on success, 4 on failure
    if (check_dimensions(&image_struct,  argv[1], input_file) == BAD_DIMENSION){
>>>>>>> 0d7f47695274b2a53e0b80fc1762bf5f30e2ac10
        return BAD_DIMENSION;

    // checks memory has been allocated properly for 2d array
    if (check_malloc(&image_struct, input_file) == BAD_MALLOC)
        return BAD_MALLOC;

<<<<<<< HEAD
    // reads in image data from input file
    if (read_data(&image_struct, argv[1], input_file) == BAD_DATA)
=======
    // reads data into 2d array and checks data is valid
    // e.g within MIN_GRAY - MAX_GRAY and correct amounts of data read
    // Parameters image_struct, argv[1] - for error statements, input_file - 
    // the file to test 
    // Returns 0 on success, 6 on failure
    if (read_data(&image_struct, argv[1], input_file) == BAD_DATA){
>>>>>>> 0d7f47695274b2a53e0b80fc1762bf5f30e2ac10
        return BAD_DATA;

    // open the output file in write mode
    FILE *output_file = fopen(argv[2], "w");

    // checks we can write to output_file
    if (check_bad_output(&image_struct, output_file, argv[2]) == BAD_WRITE_PERMISSIONS)
        return BAD_WRITE_PERMISSIONS;

    // Writes the header of the output file
    if (write_header(&image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;
<<<<<<< HEAD
=======
    }

    // Writes main image data to output file
    // Parameters: image_struct, output_file - the file to write to
    // Return: returns 0 on success returns 7 on failure
    if (write_image_data(&image_struct, output_file) == BAD_OUTPUT){
        return BAD_OUTPUT;
    }
>>>>>>> 0d7f47695274b2a53e0b80fc1762bf5f30e2ac10

    // writes image data to output file
    if (write_image_data(&image_struct, output_file) == BAD_OUTPUT)
        return BAD_OUTPUT;

    // frees malloc'd memory and closes the output file
    destructor(&image_struct, output_file);

    // Print final success message and return 0 on success
    printf("ECHOED\n");
    return SUCCESS;
}
