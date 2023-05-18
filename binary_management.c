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


/**
 * @brief Reads binary data from a file and stores it in a 2D array.
 *
 * This function reads binary values from the specified file and stores them in a 2D array
 * within the provided `image_struct`. It assumes that the file is already opened for reading.
 *
 * @param image_struct Pointer to the structure holding image data.
 * @param input_file_name Name of the input file.
 * @param input_file Pointer to the opened input file.
 * @return Returns `FUNCTION_SUCCESS` if the operation is successful, or `BAD_DATA` if an error occurs.
 */
int read_binary_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file)
{
    unsigned char test;
    fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    // fread starts on the wrong line - manually increment it

    // iterate through 2D array
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

            // cast to int and store back in 2D array
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


/**
 * @brief Writes binary data from a 2D array to a file.
 *
 * This function writes binary values from the `image_struct`'s 2D array to the specified output file.
 * It assumes that the file is already opened for writing.
 *
 * @param image_struct Pointer to the structure holding image data.
 * @param output_file Pointer to the opened output file.
 * @return Returns `FUNCTION_SUCCESS` if the operation is successful, or `BAD_OUTPUT` if an error occurs.
 */
int write_binary_data(image_struct_type *image_struct, FILE *output_file)
{
    // iterate through 2D array
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


/**
 * @brief Compresses the buffer and writes the compressed data to an output file.
 *
 * This function compresses the data in the `buffer` by performing bitwise operations
 * and stores the compressed data in the `comp_buffer`. It then writes the compressed
 * data to the specified `output_file`.
 *
 * @param buffer Array of unsigned characters containing the data to be compressed.
 * @param comp_buffer Array of unsigned characters to hold the compressed data.
 * @param output_file Pointer to the opened output file.
 */
void comp(unsigned char buffer[], unsigned char comp_buffer[], FILE *output_file)
{
    // Initialize variables to hold elements in the buffer
    unsigned char this_element, next_element, element_2_away, element_3_away;

    // Loop through each byte in the buffer
    for (int count = 0; count < 5; count++)
    {
        // Assign the values for this_element and next_element
        this_element = buffer[count];
        next_element = buffer[count + 1];
        element_2_away = buffer[count + 2];
        element_3_away = buffer[count + 3];

        // bad compression algorithm
        switch (count)
        {
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

    // write comp_buffer to output file
    if (fwrite(comp_buffer, 1, 5, output_file) != 5)
    {
        printf("Error writing to the output file.\n");
        return;
    }
}


/**
 * @brief Compresses the image data and writes it to the output file.
 *
 * This function compresses the image data from the `image_struct` by dividing it into 8-byte chunks,
 * filling the `buffer` with uncompressed data or zeros, compressing the buffer using the `comp` function,
 * and writing the compressed data to the specified `output_file`.
 *
 * @param image_struct Pointer to the structure holding image data.
 * @param output_file Pointer to the opened output file.
 * @return Returns `FUNCTION_SUCCESS` if the operation is successful.
 */
int compress_data_to_file(image_struct_type *image_struct, FILE *output_file)
{
    unsigned char comp_buffer[5];
    unsigned char buffer[8];

    int total_bytes = image_struct->height * image_struct->width;

    // Iterate over the image data in 8-byte chunks
    for (int byte_index = 0; byte_index < total_bytes; byte_index += 8)
    {
        // Fill the buffer with 8 bytes of uncompressed data or 0's
        for (int b = 0; b < 8; b++)
        {
            if (byte_index + b < total_bytes)
            {
                // Calculate the row index by dividing the sum of byte_index and b
                // by the width of the image
                int row = (byte_index + b) / image_struct->width;

                // Calculate the column index by taking the modulus of the sum of byte_index and b
                // with the width of the image
                int col = (byte_index + b) % image_struct->width;

                // Copy the pixel value from the image structure to the buffer
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
    return FUNCTION_SUCCESS;
}


/**
 * @brief Rounds up the width of the image for compression.
 *
 * This function rounds up the width of the original image to obtain the compressed image width
 * based on a compression ratio of 1.6. The result is stored in the `image_struct_compressed`.
 *
 * @param image_struct Pointer to the structure holding the original image data.
 * @param image_struct_compressed Pointer to the structure holding the compressed image data.
 */
void round_up(image_struct_type *image_struct, image_struct_type *image_struct_compressed)
{
    // Calculate the compressed width based on the original width divided by 1.6
    image_struct_compressed->width = (int)(image_struct->width / 1.6);

    // Check if rounding up is necessary
    if (image_struct->width > image_struct_compressed->width * 1.6)
    {
        image_struct_compressed->width += 1; // Round up by increasing the compressed width by 1
    }
}


/**
 * @brief Rounds up the width of the image and returns the result.
 *
 * This function calculates and rounds up the width of the image based on a compression ratio of 1.6.
 * The rounded-up width is returned as the result.
 *
 * @param image_struct Pointer to the structure holding the image data.
 * @return The rounded-up width of the image.
 */
int round_up_return(image_struct_type *image_struct)
{
    int tmp = (int)(image_struct->width / 1.6);

    // Check if rounding up is necessary
    if (image_struct->width > tmp * 1.6)
    {
        tmp += 1; // Round up by increasing the width by 1
    }
    return tmp;
}


/**
 * @brief Reads compressed data from a file and stores it in a 2D array.
 *
 * This function reads compressed binary values from the specified file and stores them in a 2D array
 * within the provided `image_struct`. It assumes that the file is already opened for reading.
 *
 * @param image_struct Pointer to the structure holding image data.
 * @param input_file_name Name of the input file.
 * @param input_file Pointer to the opened input file.
 * @return Returns `FUNCTION_SUCCESS` if the operation is successful, or an appropriate error code.
 */
int read_compressed_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file)
{
    unsigned char test;
    fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    // fread starts on the wrong line - manually increment it

    unsigned char binary_value;

    // Iterate through the 2D array
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {
            unsigned int value;

            // Read in binary values
            image_struct->check = fread(&binary_value, sizeof(unsigned char), 1, input_file);

            if (image_struct->check == 0)
            {
                return FUNCTION_SUCCESS;
            }

            // Cast to int and store in 2D array
            value = (unsigned int)binary_value;
            image_struct->imageData[i][j] = value;
        }
    }
    fclose(input_file);
    return FUNCTION_SUCCESS;
}


/**
 * @brief Decompresses the buffer and stores the decompressed data in an array.
 *
 * This function decompresses the data in the `buffer` by performing bitwise operations and stores
 * the decompressed data in the `uncomp_buffer`. The decompression is performed based on the specified
 * rules and using the provided `image_struct`.
 *
 * @param buffer Array of unsigned characters containing the compressed data.
 * @param uncomp_buffer Array of unsigned characters to hold the decompressed data.
 * @param image_struct Pointer to the structure holding image data.
 */
void decomp(unsigned char buffer[], unsigned char uncomp_buffer[], image_struct_type *image_struct){
    int local_k = 0;
    int count = 0;

    for (int l = 0; l < 8; l++){

        unsigned char this_element = buffer[count];
        unsigned char prev_element = (count > 0) ? buffer[count - 1] : 0;
        unsigned char element_2_away = (count > 1) ? buffer[count - 2] : 0;
        unsigned char element_3_away = (count > 2) ? buffer[count - 3] : 0;

        switch (count)
        {
            case (0):
                this_element >>= 3;
                uncomp_buffer[local_k] = this_element;
                break;

            case (1):
                prev_element <<= 5;
                prev_element >>= 3;

                this_element >>= 6;
                this_element |= prev_element;
                uncomp_buffer[local_k] = this_element;
                break;

            case (2):
                prev_element <<= 2;
                prev_element >>= 3;
                uncomp_buffer[local_k] = prev_element;
                break;

            case (3):
                element_2_away <<= 7;
                element_2_away >>= 3;

                prev_element >>= 4;
                prev_element |= element_2_away;
                uncomp_buffer[local_k] = prev_element;            
                break;

            case (4):
                element_2_away <<= 4;
                element_2_away >>= 3;

                prev_element >>= 7;
                element_2_away |= prev_element;
                uncomp_buffer[local_k] = element_2_away;           
                break;

            case (5):
                element_2_away <<= 1;
                element_2_away >>= 3;
                uncomp_buffer[local_k] = element_2_away;
                break;

            case (6):
                element_3_away <<= 6;
                element_3_away >>= 3;

                element_2_away >>= 5;
                element_3_away |= element_2_away;
                uncomp_buffer[local_k] = element_3_away;
                break;

            case (7):
                element_3_away <<= 3;
                element_3_away >>= 3;
                uncomp_buffer[local_k] = element_3_away;
                break;
        }

        count++;
        local_k++;

        if (count == 8)
        {
            count = 0;
        }
    }
}


/**
 * @brief Decompresses the data from a file and stores it in the image structure.
 *
 * This function reads compressed data from the specified file, decompresses it using the `decomp` function,
 * and stores the decompressed data in the `image_struct` array. It assumes that the file is already opened for reading.
 *
 * @param image_struct Pointer to the structure holding image data.
 * @param input_file Pointer to the opened input file.
 * @param input_file_name Name of the input file.
 * @return Returns `FUNCTION_SUCCESS` if the operation is successful, or `BAD_DATA` if an error occurs.
 */
int decompress_and_store(image_struct_type *image_struct, FILE *input_file, char *input_file_name)
{
    unsigned char test;
    fread(&test, sizeof(unsigned char), 1, input_file); // not sure why needed
    // fread starts on the wrong line - manually increment it

    int k = 0;

    // Read the image data
    unsigned char buffer[5];
    unsigned char uncomp_buffer[8];

    for (int i = 0; i < image_struct->compressed_numBytes; i += 5, k += 3)
    {
        image_struct->check = fread(buffer, 1, 5, input_file);

        decomp(buffer, uncomp_buffer, image_struct);

        for (int z = 0; z < 8; z++)
        {
            if (i + z + k < image_struct->numBytes)
            {
                image_struct->array[i + z + k] = uncomp_buffer[z];
            }
        }
    }

    image_struct->check = fread(buffer, 1, 1, input_file);

    if (image_struct->check != 0)
    {
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        return BAD_DATA;
    }

    fclose(input_file);
    return FUNCTION_SUCCESS;
}

/**
 * @brief Converts a one-dimensional array to a two-dimensional array.
 *
 * This function converts the one-dimensional array `array` in the `image_struct` to a two-dimensional
 * array `imageData` in the same structure. The conversion is based on the specified `height` and `width`
 * of the image.
 *
 * @param image_struct Pointer to the structure holding image data.
 */
void one_dim_two_dim(image_struct_type *image_struct)
{
    // Iterate through the 1D array and copy elements into 2D array
    for (int i = 0; i < image_struct->height; i++)
    {
        for (int j = 0; j < image_struct->width; j++)
        {
            // Work out the 1D index corresponding to 2D indices
            int one_d_index = i * image_struct->width + j;

            // Copy the element from the 1D array to the 2D array
            image_struct->imageData[i][j] = image_struct->array[one_d_index];
        }
    }
}
