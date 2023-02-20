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


    



    // compare the data from the two files:
    
    //start with magic number values
    if (*magicNumberValue1 != *magicNumberValue2)
        { // free and exit
        free(imageData1);
        free(imageData2);
        printf("DIFFERENT\n");
        return SUCCESS;
        } // free and exit

    // check dimensions
    if ((height1 != height2) || (width1 != width2))
        { // free and exit
        free(imageData1);
        free(imageData2);
        printf("DIFFERENT\n");
        return SUCCESS;
        } // free and exit

    // and check the pixel values
    for (int n=0; n<numBytes1; n++)
        {
        if(imageData1[n]!=imageData2[n])
            { // free and exit
            free(imageData1);
            free(imageData2);
            printf("DIFFERENT\n");
            return SUCCESS;
            } // free and exit
        }


    // free allocated memory before exit
    free(imageData1);
    free(imageData2);

    // if we have not exited on different data, must be identical
    printf("IDENTICAL\n");
    return SUCCESS;
    } // main()
