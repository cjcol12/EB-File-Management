#ifndef binary_MGMT_header
#define binary_MGMT_header

int read_binary_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file);

int write_binary_data(image_struct_type *image_struct, FILE *output_file);

void comp(unsigned char buffer[], unsigned char comp_buffer[], FILE *output_file);

int compress_data_to_file(image_struct_type *image_struct, FILE *output_file);

void round_up(image_struct_type *image_struct, image_struct_type *image_struct_compressed);

int round_up_return(image_struct_type *image_struct);

int read_compressed_data(image_struct_type *image_struct, char *input_file_name, FILE *input_file);

void decomp(unsigned char buffer[], unsigned char uncomp_buffer[], image_struct_type *image_struct);

int decompress(image_struct_type *image_struct, image_struct_type *image_struct_compressed);

#endif