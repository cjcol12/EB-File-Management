/*  Function: Module to read and write binary data to and from files

    No main function

    Author: CJ Coleman
*/

// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Image structure header file inclusion
#include "image_structs.h"

// Definition header file inclusion
#include "definitions.h"

// Generic function file inclusion
#include "generic.h"

// Generic function file inclusion
#include "binary_management.h"

// Error checking module inclusion
#include "error_checking.h"

int read_binary_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file)
{

    unsigned char test;
    fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    // fread starts on the wrong line - manually incremenet it

    // iterate through 2d array
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {
            unsigned char binary_value;
            unsigned int value;

            // read in binary values
            image_struct->check = fread(&binary_value, sizeof(unsigned char), 1, input_file);

            // check fread has read exactly one item
            if (check_data_captured(image_struct, input_file, input_file_name) == BAD_DATA)
                return BAD_DATA;

            // cast to int and store back in 2d array
            value = (unsigned int)binary_value;
            image_struct->imageData[i][j] = value;

            // check imageData is within bounds
            if (check_data_values(value, input_file, input_file_name, image_struct) == BAD_DATA)
                return BAD_DATA;
        }
    }
    fclose(input_file);
    return FUNCTION_SUCCESS;
}

int write_binary_data(image_struct_type *image_struct, FILE *output_file)
{
    // iterate through 2d array
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {

            // casting imageData to char to be written to output file using fwrite
            unsigned int value = image_struct->imageData[i][j];
            unsigned char binary_value = (unsigned char)(value);

            // write binary values
            image_struct->check = fwrite(&binary_value, sizeof(unsigned char), 1, output_file);

            // check fwrite has written exactly one item
            if (check_binary_written(image_struct, output_file) == BAD_OUTPUT)
                return BAD_OUTPUT;
        }
    }
    return FUNCTION_SUCCESS;
}

int compress_data(image_struct_type *image_struct, image_struct_type *image_struct_compressed)
{
    int count;
    int compressed_byte_counter = 0;
    int k;

    for (int i = 0; i < image_struct_compressed->height; i++)
    {
        count = 0;
        k = 0;
        compressed_byte_counter = 0;
        for (int j = 0; j < image_struct->width; j++)
        {

            unsigned char this_element = image_struct->imageData[i][j];
            unsigned char next_element = (j < image_struct->width - 1) ? image_struct->imageData[i][j + 1] : 0;
            unsigned char element_2_away = (j < image_struct->width - 2) ? image_struct->imageData[i][j + 2] : 0;
            unsigned char element_3_away = (j < image_struct->width - 3) ? image_struct->imageData[i][j + 3] : 0;

            switch (count)
            {
            case 0:
                this_element <<= 3;

                next_element <<= 3;
                next_element >>= 5;

                this_element |= next_element;
                image_struct_compressed->imageData[i][k] = this_element;

                break;

            case 1:
                this_element <<= 6;

                next_element <<= 3;
                next_element >>= 2;

                this_element |= next_element;

                element_2_away <<= 3;
                element_2_away >>= 7;

                this_element |= element_2_away;

                image_struct_compressed->imageData[i][k] = this_element;
                break;

            case 2:
                next_element <<= 4;

                element_2_away <<= 3;
                element_2_away >>= 4;

                next_element |= element_2_away;
                image_struct_compressed->imageData[i][k] = next_element;
                break;

            case 3:
                next_element <<= 7;

                element_2_away <<= 3;
                element_2_away >>= 1;

                next_element |= element_2_away;

                element_3_away <<= 3;
                element_3_away >>= 6;
                next_element |= element_3_away;
                image_struct_compressed->imageData[i][k] = next_element;
                break;

            case 4:
                element_2_away <<= 5;

                element_3_away <<= 3;
                element_3_away >>= 3;

                element_2_away |= element_3_away;
                image_struct_compressed->imageData[i][k] = element_2_away;
                break;
            }
            count++;
            k++;

            if (count == 5)
            {
                count = 0;
                compressed_byte_counter++;
                j = compressed_byte_counter * 7;
            }
        }
    }
    return FUNCTION_SUCCESS;
}

void round_up(image_struct_type *image_struct, image_struct_type *image_struct_compressed)
{
    image_struct_compressed->width = (int)(image_struct->width / 1.6);
    if (image_struct->width > image_struct_compressed->width * 1.6)
    {
        image_struct_compressed->width += 1;
    }
}

int round_up_return(image_struct_type *image_struct)
{
    int tmp = (int)(image_struct->width / 1.6);
    if (image_struct->width > tmp * 1.6)
    {
        tmp += 1;
    }
    return tmp;
}

int read_compressed_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file)
{
    unsigned char test;
    fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    // fread starts on the wrong line - manually incremenet it

    unsigned char binary_value;
    // iterate through 2d array
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {
            unsigned int value;

            // read in binary values
            image_struct->check = fread(&binary_value, sizeof(unsigned char), 1, input_file);

            // cast to int and store back in 2d array
            value = (unsigned int)binary_value;
            image_struct->imageData[i][j] = value;
        }
    }
    fclose(input_file);
    return FUNCTION_SUCCESS;
}

int decompress(image_struct_type *image_struct, image_struct_type *image_struct_compressed)
{
    int count = 0;
    int k = 0;
    int compressed_byte_counter;
    for (int i = 0; i < image_struct->height; i++)
    {
        count = 0;
        k = 0;
        compressed_byte_counter = 0;
        for (int j = 0; j < image_struct->width; j++)
        {

            unsigned char this_element = image_struct_compressed->imageData[i][k];
            unsigned char prev_element = (k > 0) ? image_struct_compressed->imageData[i][k - 1] : 0;
            unsigned char element_2_away = (k > 1) ? image_struct_compressed->imageData[i][k - 2] : 0;
            unsigned char element_3_away = (k > 2) ? image_struct_compressed->imageData[i][k - 3] : 0;

            switch (count)
            {
            case (0):
                this_element >>= 3;
                image_struct->imageData[i][j] = this_element;
                break;

            case (1):
                prev_element <<= 5;
                prev_element >>= 3;

                this_element >>= 6;
                this_element |= prev_element;
                image_struct->imageData[i][j] = this_element;
                break;

            case (2):
                prev_element <<= 2;
                prev_element >>= 3;
                image_struct->imageData[i][j] = prev_element;
                break;

            case (3):
                element_2_away <<= 7;
                element_2_away >>= 3;

                prev_element >>= 4;

                prev_element |= element_2_away;
                image_struct->imageData[i][j] = prev_element;
                break;

            case (4):
                element_2_away <<= 4;
                element_2_away >>= 3;

                prev_element >>= 7;
                element_2_away |= prev_element;
                image_struct->imageData[i][j] = element_2_away;
                break;

            case (5):
                element_2_away <<= 1;
                element_2_away >>= 3;
                image_struct->imageData[i][j] = element_2_away;
                break;

            case (6):
                element_3_away <<= 6;
                element_3_away >>= 3;

                element_2_away >>= 5;

                element_3_away |= element_2_away;
                image_struct->imageData[i][j] = element_3_away;
                break;

            case (7):
                element_3_away <<= 3;
                element_3_away >>= 3;
                image_struct->imageData[i][j] = element_3_away;
                break;
            }
            count++;
            k++;

            if (count == 8)
            {
                count = 0;
                k = 5;
                compressed_byte_counter++;
                k = compressed_byte_counter * 5;
            }
        }
    }
    return FUNCTION_SUCCESS;
}
