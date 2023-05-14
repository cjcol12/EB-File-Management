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
            // printf("%d ", image_struct->imageData[i][j]);
            unsigned char binary_value = (unsigned char)(value);

            // write binary values
            image_struct->check = fwrite(&binary_value, sizeof(unsigned char), 1, output_file);

            // check fwrite has written exactly one item
            if (check_binary_written(image_struct, output_file) == BAD_OUTPUT)
                return BAD_OUTPUT;
        }
        // printf("\n");
    }
    return FUNCTION_SUCCESS;
}

void comp(unsigned char buffer[], unsigned char comp_buffer[], FILE *output_file) {
    // printf("\n\n\nbuffer is ");
    // for (int b = 0; b < 8; b++){
    //     printf("%d ", buffer[b]);
    // }
 
    // Initialize variables to hold elements in the buffer
    unsigned char this_element, next_element, element_2_away, element_3_away;

    // Loop through each byte in the buffer
    for(int count = 0; count < 5; count++) {
        // Assign the values for this_element and next_element
        this_element = buffer[count];
        next_element = buffer[count + 1];
        element_2_away = buffer[count + 2];
        element_3_away = buffer[count + 3];

        // Compression logic using switch-case
        switch (count) {
            case 0:
                this_element <<= 3;
                next_element <<= 3;
                next_element >>= 5;
                this_element |= next_element;
                comp_buffer[count] = this_element;
                break;

            case 1:
                this_element <<= 6;
                next_element <<= 3;
                next_element >>= 2;
                this_element |= next_element;
                element_2_away <<= 3;
                element_2_away >>= 7;
                this_element |= element_2_away;
                comp_buffer[count] = this_element;
                break;

            case 2:
                next_element <<= 4;
                element_2_away <<= 3;
                element_2_away >>= 4;
                next_element |= element_2_away;
                comp_buffer[count] = next_element;
                break;

            case 3:
                next_element <<= 7;
                element_2_away <<= 3;
                element_2_away >>= 1;
                next_element |= element_2_away;
                element_3_away <<= 3;
                element_3_away >>= 6;
                next_element |= element_3_away;
                comp_buffer[count] = next_element;
                break;

            case 4:
                element_2_away <<= 5;
                element_3_away <<= 3;
                element_3_away >>= 3;
                element_2_away |= element_3_away;
                comp_buffer[count] = element_2_away;
                break;
        }
    }

    if (fwrite(comp_buffer, 1, 5, output_file) != 5) {
        printf("Error writing to the output file.\n");
        return;
    }

}

void compress_data_to_file(image_struct_type *image_struct, FILE *output_file)
{
    unsigned char comp_buffer[5];
    unsigned char buffer[8];

    int total_bytes = image_struct->height * image_struct->width;
    
    for(int byte_index = 0; byte_index < total_bytes; byte_index += 8)
    {
        // Fill the buffer with 8 bytes of uncompressed data or 0's
        for (int b = 0; b < 8; b++)
        {
            if (byte_index + b < total_bytes)
            {
                int row = (byte_index + b) / image_struct->width;
                int col = (byte_index + b) % image_struct->width;
                buffer[b] = image_struct->imageData[row][col];
            }
            else
            {
                buffer[b] = 0;
            }
        }

        // Compress the buffer and write to output file
        comp(buffer, comp_buffer, output_file);
    }
}
 
// void compress_data_to_file(image_struct_type *image_struct, FILE *output_file){

//     unsigned char comp_buffer[5];
//     unsigned char buffer[8];
//     // int k = 0;
    
//     for(int i = 0; i < image_struct->height; i++){
//         for(int j = 0; j < image_struct->width; j += 8){

//             // Fill the buffer with 8 bytes of uncompressed data or 0's
//             for (int b = 0; b < 8; b++)
//             {
//                 if (j + b < image_struct->width){
//                     buffer[b] = image_struct->imageData[i][j + b];
//                 }
//                 else{
//                     buffer[b] = 0;
//                 }
//             }

            
//             comp(buffer, comp_buffer, output_file);
//         }
//     }
// }

// int compress_data_to_files(image_struct_type *image_struct, FILE *output_file)
// {
//     // printf("compressed data\n");
//     int count;
//     int k;
//     unsigned char comp_buffer[5];
//     unsigned char buffer[8];


//     for(int i = 0; i < image_struct->height; i++)
//     {
//         count = 0;
//         k = 0;
//         for(int j = 0; j < image_struct->width; j++)
//         {
//             unsigned char this_element = image_struct->imageData[i][j];
//             unsigned char next_element = (j < image_struct->width - 1) ? image_struct->imageData[i][j + 1] : 0;
//             unsigned char element_2_away = (j < image_struct->width - 2) ? image_struct->imageData[i][j + 2] : 0;
//             unsigned char element_3_away = (j < image_struct->width - 3) ? image_struct->imageData[i][j + 3] : 0;

//             switch (count)
//             {
//             case 0:
           
//                 this_element <<= 3;

//                 next_element <<= 3;
//                 next_element >>= 5;

//                 this_element |= next_element;

//                 comp_buffer[k] = this_element;

//                 break;

//             case 1:
//                 this_element <<= 6;

//                 next_element <<= 3;
//                 next_element >>= 2;

//                 this_element |= next_element;

//                 element_2_away <<= 3;
//                 element_2_away >>= 7;

//                 this_element |= element_2_away;

//                 comp_buffer[k] = this_element;
//                 break;

//             case 2:
//                 next_element <<= 4;

//                 element_2_away <<= 3;
//                 element_2_away >>= 4;

//                 next_element |= element_2_away;
//                 comp_buffer[k] = next_element;
//                 break;

//             case 3:
//                 next_element <<= 7;

//                 element_2_away <<= 3;
//                 element_2_away >>= 1;

//                 next_element |= element_2_away;

//                 element_3_away <<= 3;
//                 element_3_away >>= 6;
//                 next_element |= element_3_away;
//                 comp_buffer[k] = next_element;
//                 break;

//             case 4:
//                 element_2_away <<= 5;

//                 element_3_away <<= 3;
//                 element_3_away >>= 3;

//                 element_2_away |= element_3_away;
//                 comp_buffer[k] = element_2_away;
//                 break;
//             }
//             count++;
//             k++;


//             if (count == 5)
//             {
            
//                 // for (int z = 0; z < 5; z++){
//                 //     printf("%d ", buffer[z]);
//                 // }
                
//                 // Write the compressed data to the output file
//                 if (fwrite(buffer, 1, k, output_file) != k)
//                 {
//                     printf("Error writing to the output file.\n");
//                     return FUNCTION_SUCCESS;
//                 }
//                 k = 0;
//                 count = 0;
//                 j += 3; // Advance the index to account for the next 3 elements that were already processed
//             }
//         }
//     }

//     // Close the output file
//     return FUNCTION_SUCCESS;
// }

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

            

            if (image_struct->check == 0){
                return FUNCTION_SUCCESS;
            }
            // cast to int and store back in 2d array
            value = (unsigned int)binary_value;
            image_struct->imageData[i][j] = value;
        }

    }
    fclose(input_file);
    return FUNCTION_SUCCESS;
}


void decomp(unsigned char buffer[], image_struct_type *image_struct, int count, int i, int k){
    for (int l = 0; l < 8; l++){

        unsigned char this_element = buffer[count];
        unsigned char prev_element = (count > 0) ? buffer[count - 1] : 0;
        unsigned char element_2_away = (count > 1) ? buffer[count - 2] : 0;
        unsigned char element_3_away = (count > 2) ? buffer[count - 3] : 0;

        switch (count)
        {
            case (0):
                this_element >>= 3;
                image_struct->imageData[i][k] = this_element;
                break;

            case (1):
                prev_element <<= 5;
                prev_element >>= 3;

                this_element >>= 6;
                this_element |= prev_element;
                image_struct->imageData[i][k] = this_element;
                break;

            case (2):
                prev_element <<= 2;
                prev_element >>= 3;
                image_struct->imageData[i][k] = prev_element;
                break;

            case (3):
                element_2_away <<= 7;
                element_2_away >>= 3;

                prev_element >>= 4;

                prev_element |= element_2_away;
                image_struct->imageData[i][k] = prev_element;
                break;

            case (4):
                element_2_away <<= 4;
                element_2_away >>= 3;

                prev_element >>= 7;
                element_2_away |= prev_element;
                image_struct->imageData[i][k] = element_2_away;
                break;

            case (5):
                element_2_away <<= 1;
                element_2_away >>= 3;
                image_struct->imageData[i][k] = element_2_away;
                break;

            case (6):
                element_3_away <<= 6;
                element_3_away >>= 3;

                element_2_away >>= 5;

                element_3_away |= element_2_away;
                image_struct->imageData[i][k] = element_3_away;
                break;

            case (7):
                element_3_away <<= 3;
                element_3_away >>= 3;
                image_struct->imageData[i][k] = element_3_away;
                break;
            }
            count++;
            k++;

        if (count == 8)
        {
            count = 0;
        }
    }
}


int decompress_and_store(image_struct_type *image_struct, FILE *input_file)
{

    unsigned char test;
    fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    // fread starts on the wrong line - manually incremenet it

    int count = 0;
    int k = 0;

    // Read the image data
    unsigned char buffer[5];

    for (int i = 0; i < image_struct->height; i++)
    {
        k = 0;
        count = 0;
        for (int j = 0; j < image_struct->compressed_width; j += 5) // width / block size
        {
            image_struct->check = fread(buffer, 1, 5, input_file);
            decomp(buffer, image_struct, count, i, k);
            k += 8;
        }
    }

    fclose(input_file);
    return FUNCTION_SUCCESS;
}

int decompress(image_struct_type *image_struct_uncompressed, image_struct_type *image_struct)
{
    int count = 0;
    int k = 0;
    int compressed_byte_counter;

    // for (int i = 0; i < image_struct->height; i++){
    //     printf("%d\t", i + 1);
    //     for (int j = 0; j < image_struct->width; j++){
    //         printf("%d ", image_struct->imageData[i][j]);
    //     }
    //     printf("\n\n");
    // }

    for (int i = 0; i < image_struct->height; i++)
    {
        count = 0;
        k = 0;
        compressed_byte_counter = 0;
        for (int j = 0; j < image_struct->width; j++)
        {

            unsigned char this_element = image_struct->imageData[i][k];
            unsigned char prev_element = (k > 0) ? image_struct->imageData[i][k - 1] : 0;
            unsigned char element_2_away = (k > 1) ? image_struct->imageData[i][k - 2] : 0;
            unsigned char element_3_away = (k > 2) ? image_struct->imageData[i][k - 3] : 0;

            switch (count)
            {
            case (0):
                this_element >>= 3;
                image_struct_uncompressed->imageData[i][j] = this_element;
                break;

            case (1):
                prev_element <<= 5;
                prev_element >>= 3;

                this_element >>= 6;
                this_element |= prev_element;
                image_struct_uncompressed->imageData[i][j] = this_element;
                break;

            case (2):
                prev_element <<= 2;
                prev_element >>= 3;
                image_struct_uncompressed->imageData[i][j] = prev_element;
                break;

            case (3):
                element_2_away <<= 7;
                element_2_away >>= 3;

                prev_element >>= 4;

                prev_element |= element_2_away;
                image_struct_uncompressed->imageData[i][j] = prev_element;
                break;

            case (4):
                element_2_away <<= 4;
                element_2_away >>= 3;

                prev_element >>= 7;
                element_2_away |= prev_element;
                image_struct_uncompressed->imageData[i][j] = element_2_away;
                break;

            case (5):
                element_2_away <<= 1;
                element_2_away >>= 3;
                image_struct_uncompressed->imageData[i][j] = element_2_away;
                break;

            case (6):
                element_3_away <<= 6;
                element_3_away >>= 3;

                element_2_away >>= 5;

                element_3_away |= element_2_away;
                image_struct_uncompressed->imageData[i][j] = element_3_away;
                break;

            case (7):
                element_3_away <<= 3;
                element_3_away >>= 3;
                image_struct_uncompressed->imageData[i][j] = element_3_away;
                break;
            }
            count++;
            k++;

            if (count == 8)
            {
                count = 0;
                compressed_byte_counter++;
                k = compressed_byte_counter * 5;
            }
        }
    }
    return FUNCTION_SUCCESS;
}