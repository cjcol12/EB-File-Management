#ifndef read_image_header
#define read_image_header

int check_arg_count(int argc);

int check_file_opened(char *input_file_name, FILE *input_file);

int check_magic_number(image_struct_type *image_struct, char *input_file_name, FILE *inputFile);


int check_dimensions(image_struct_type *image_struct, char *input_file_name, FILE *input_file);
    
int check_malloc(image_struct_type *image_struct, FILE *input_file);

int read_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file);

int check_mn_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name);

int check_dimensions_valid(image_struct_type *image_struct, FILE *input_file, char *input_file_name);

#endif