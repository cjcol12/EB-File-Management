#ifndef ERROR_CHECKING_MODULE
#define ERROR_CHECKING_MODULE

int check_arg_count(int argc);

int check_file_opened(char *input_file_name, FILE *input_file);

int check_mn_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name);

int check_dimensions_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name);

int check_extra_data(image_struct_type *image_struct, FILE *input_file, char *input_file_name);

int check_data_values(int data, FILE *input_file, char *input_file_name, image_struct_type *image_struct);

int check_data_values_compressed(int data, FILE *input_file, char *input_file_name, image_struct_type *image_struct);

int check_data_captured(image_struct_type *image_struct, FILE *input_file, char *input_file_name);

int check_image_data_malloc(image_struct_type *image_struct, FILE *input_file);

int check_data_block_malloc(image_struct_type *image_struct, FILE *input_file);

int check_bad_output(image_struct_type *image_struct, FILE *output_file, char *input_file_name);

int check_data_written(image_struct_type *image_struct, FILE  *output_file);

int check_binary_written(image_struct_type *image_struct, FILE *output_file);

#endif