// Standard I/O header file inclusion
#include <stdio.h>

// Standard library header file inclusion
#include <stdlib.h>

// Definition header file inclusion
#include "definitions.h"

// Image structure header file inclusion
#include "image_structs.h"

// Error checking header file inclusion
#include "error_checking.h"

// Generic function file inclusion
#include "generic.h"

/**
 * @brief Check the number of command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @return 0 on success, BAD_ARGUMENT_COUNT on failure.
 */
int check_arg_count(int argc) {
    // Validate that the user has entered 2 arguments
    if (argc != 3) {
        printf("ERROR: Bad Arguments\n");
        return BAD_ARGUMENT_COUNT;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the input file was opened successfully.
 *
 * @param input_file_name The name of the input file.
 * @param input_file Pointer to the input file.
 * @return 0 on success, BAD_FILE on failure.
 */
int check_file_opened(char *input_file_name, FILE *input_file) {
    // Check if the file was opened correctly
    if (!input_file) {
        printf("ERROR: Bad File Name (%s)\n", input_file_name);
        return BAD_FILE;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the magic number in the image structure is valid.
 *
 * @param image_struct Pointer to the image structure.
 * @param input_file Pointer to the input file.
 * @param input_file_name The name of the input file.
 * @return 0 on success, BAD_MAGIC_NUMBER on failure.
 */
int check_mn_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name) {
    // Checking magic number against the casted value due to endianness.
    // Change to check file type (.ebf /.ebu)?
    if (*(image_struct->magic_number_value) != MAGIC_NUMBER_EB &&
        *(image_struct->magic_number_value) != MAGIC_NUMBER_EU &&
        *(image_struct->magic_number_value) != MAGIC_NUMBER_EC) {
        printf("ERROR: Bad Magic Number (%s)\n", input_file_name);
        fclose(input_file);
        return BAD_MAGIC_NUMBER;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the dimensions in the image structure are valid.
 *
 * @param image_struct Pointer to the image structure.
 * @param input_file Pointer to the input file.
 * @param input_file_name The name of the input file.
 * @return 0 on success, BAD_DIMENSION on failure.
 */
int check_dimensions_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name) {
    // Check if we captured two return values
    // Check if dimensions are valid
    if (image_struct->check != 2 ||
        image_struct->height < MIN_DIMENSION ||
        image_struct->width < MIN_DIMENSION ||
        image_struct->height > MAX_DIMENSION ||
        image_struct->width > MAX_DIMENSION) {
        fclose(input_file);
        printf("ERROR: Bad Dimensions (%s)\n", input_file_name);
        return BAD_DIMENSION;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if memory allocation for image data was successful.
 *
 * @param image_struct Pointer to the image structure.
 * @param input_file Pointer to the input file.
 * @return 0 on success, BAD_MALLOC on failure.
 */
int check_image_data_malloc(image_struct_type *image_struct, FILE *input_file) {
    if (image_struct->imageData == NULL) {
        printf("ERROR: Image Malloc Failed\n");
        destructor(image_struct, input_file);
        return BAD_MALLOC;
    }
    return FUNCTION_SUCCESS;
}

/**
 * @brief Check if memory allocation for data block was successful.
 *
 * @param image_struct Pointer to the image structure.
 * @param input_file Pointer to the input file.
 * @return 0 on success, BAD_MALLOC on failure.
 */
int check_data_block_malloc(image_struct_type *image_struct, FILE *input_file) {
    if (image_struct->data_block == NULL) {
        printf("ERROR: Image Malloc Failed\n");
        destructor(image_struct, input_file);
        return BAD_MALLOC;
    }
    return FUNCTION_SUCCESS;
}

/**
 * @brief Check if there is extra data in the image structure.
 *
 * @param image_struct Pointer to the image structure.
 * @param input_file Pointer to the input file.
 * @param input_file_name The name of the input file.
 * @return 0 on success, BAD_DATA on failure.
 */
int check_extra_data(image_struct_type *image_struct, FILE *input_file, char *input_file_name) {
    if (image_struct->check == 1) {
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the data value is within valid range.
 *
 * @param data The data value to check.
 * @param input_file Pointer to the input file.
 * @param input_file_name The name of the input file.
 * @param image_struct Pointer to the image structure.
 * @return 0 on success, BAD_DATA on failure.
 */
int check_data_values(int data, FILE *input_file, char *input_file_name, image_struct_type *image_struct) {
    if (data > MAX_GRAY || data < MIN_GRAY) {
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the compressed data value is within valid range.
 *
 * @param data The compressed data value to check.
 * @param input_file Pointer to the input file.
 * @param input_file_name The name of the input file.
 * @param image_struct Pointer to the image structure.
 * @return 0 on success, BAD_DATA on failure.
 */
int check_data_values_compressed(int data, FILE *input_file, char *input_file_name, image_struct_type *image_struct) {
    if (data > MAX_GRAY_COMPRESSED || data < MIN_GRAY) {
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the data was properly captured.
 *
 * @param image_struct Pointer to the image structure.
 * @param input_file Pointer to the input file.
 * @param input_file_name The name of the input file.
 * @return 0 on success, BAD_DATA on failure.
 */
int check_data_captured(image_struct_type *image_struct, FILE *input_file, char *input_file_name) {
    if (image_struct->check != 1) {
        printf("ERROR: Bad Data (%s)\n", input_file_name);
        destructor(image_struct, input_file);
        return BAD_DATA;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the output file was opened successfully.
 *
 * @param image_struct Pointer to the image structure.
 * @param output_file Pointer to the output file.
 * @param input_file_name The name of the input file.
 * @return 0 on success, BAD_WRITE_PERMISSIONS on failure.
 */
int check_bad_output(image_struct_type *image_struct, FILE *output_file, char *input_file_name) {
    // Check if the output file was opened correctly
    if (output_file == NULL) {
        printf("ERROR: Bad Output(%s)", input_file_name);
        return BAD_WRITE_PERMISSIONS;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the data was written to the output file successfully.
 *
 * @param image_struct Pointer to the image structure.
 * @param output_file Pointer to the output file.
 * @return 0 on success, BAD_OUTPUT on failure.
 */
int check_data_written(image_struct_type *image_struct, FILE *output_file) {
    if (image_struct->check == 0) {
        printf("ERROR: Bad Output\n");
        destructor(image_struct, output_file);
        return BAD_OUTPUT;
    } else {
        return FUNCTION_SUCCESS;
    }
}

/**
 * @brief Check if the binary data was written to the output file successfully.
 *
 * @param image_struct Pointer to the image structure.
 * @param output_file Pointer to the output file.
 * @return 0 on success, BAD_OUTPUT on failure.
 */
int check_binary_written(image_struct_type *image_struct, FILE *output_file) {
    if (image_struct->check != 1) {
        destructor(image_struct, output_file);
        printf("ERROR: Bad Output\n");
        return BAD_OUTPUT;
    } else {
        return FUNCTION_SUCCESS;
    }
}
