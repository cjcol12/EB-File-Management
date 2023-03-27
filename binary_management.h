#ifndef binary_MGMT_header
#define binary_MGMT_header

int read_binary_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file);

int write_binary_data(image_struct_type *image_struct, FILE *output_file);

int compress_data(image_struct_type *image_struct, image_struct_type *image_struct_compressed);

void round_up(image_struct_type *image_struct, image_struct_type *image_struct_compressed);

int round_up_return(image_struct_type *image_struct);

int read_compressed_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file);

int decompress(image_struct_type *image_struct, image_struct_type *image_struct_compressed);

#endif