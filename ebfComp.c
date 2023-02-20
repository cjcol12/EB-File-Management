// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

// Read image module inclusion
#include "read_image.c"

#include "compare_image.c"

int unix_usage(int argc){
    // Unix usage check - runs with no arguments
    if (argc == 1){
        printf("Usage: ebfComp file1 file2");
        return USAGE_REQUEST;
    }

    // return 10 on function success
    else return 10;  // is this return value valid?
}
int main(int argc, char **argv)
    { // main

    image_struct_type image_struct;

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

    if (check_magic_number(&image_struct, argv[1], inputFile) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;
    
    // printf("\n\nmagic number is %hn\n\n", image_struct.magicNumberValue);
    if (check_dimensions(&image_struct,  argv[1], inputFile) == BAD_DIMENSION)
        return BAD_DIMENSION;

    if(check_malloc(&image_struct, inputFile) == BAD_MALLOC)
        return BAD_MALLOC;

    if (read_data(&image_struct, argv[1], inputFile) == BAD_DATA)
        return BAD_DATA;

    // now we have finished using the inputFile we should close it
    fclose(inputFile);





    image_struct_type image_struct_compare;

    // open the input file in read mode
    FILE *comparison_File = fopen(argv[2], "r");

    // check file opened successfully
    if (check_file_opened(argv[2], comparison_File) == BAD_FILE)
        return BAD_FILE;

    if (check_magic_number(&image_struct_compare, argv[2], comparison_File) == BAD_MAGIC_NUMBER)
        return BAD_MAGIC_NUMBER;
    
    // printf("\n\nmagic number is %hn\n\n", image_struct.magicNumberValue);
    if (check_dimensions(&image_struct_compare,  argv[2], comparison_File) == BAD_DIMENSION)
        return BAD_DIMENSION;

    if(check_malloc(&image_struct_compare, comparison_File) == BAD_MALLOC)
        return BAD_MALLOC;

    if (read_data(&image_struct_compare, argv[2], comparison_File) == BAD_DATA)
        return BAD_DATA;

    // now we have finished using the inputFile we should close it
    fclose(inputFile);


    if (comp_magic_number(&image_struct, &image_struct_compare) == FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS;
    if (comp_dimensions(&image_struct, &image_struct_compare) == FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS;
    if (comp_image_data(&image_struct, &image_struct_compare) == FUNCTION_SUCCESS_DIFFERENT)
        return SUCCESS;
        

    



    // free allocated memory before exit
    free(image_struct.imageData);
    free(image_struct_compare.imageData);

    // if we have not exited on different data, must be identical
    printf("IDENTICAL\n");
    return SUCCESS;
    } // main()
