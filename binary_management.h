#ifndef binary_MGMT_header
#define binary_MGMT_header

int read_binary_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file);

int write_binary_data(image_struct_type *image_struct, FILE *output_file);

int decompress(image_struct_type *image_struct, image_struct_type *image_struct_compressed);

void print_binary(int num);

#endif