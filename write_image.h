#ifndef write_image_header
#define write_image_header

int check_bad_output(
    image_struct_type *image_struct, FILE *output_file, char *input_file_name);

int write_header(
    image_struct_type *image_struct, FILE *output_file);
    
int write_image_data(
    image_struct_type *image_struct, FILE *output_file);

#endif