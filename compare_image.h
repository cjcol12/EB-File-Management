#ifndef comp_image_header
#define comp_image_header

int comp_magic_number(image_struct_type *image_struct, image_struct_type *image_struct_compare);

int comp_dimensions(image_struct_type *image_struct, image_struct_type *image_struct_compare);

int comp_image_data(image_struct_type *image_struct, image_struct_type *image_struct_compare);

#endif