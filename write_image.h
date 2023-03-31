#ifndef write_image_header
#define write_image_header

int write_header(image_struct_type *image_struct, FILE *output_file);
    
int write_image_data(image_struct_type *image_struct, FILE *output_file);

#endif