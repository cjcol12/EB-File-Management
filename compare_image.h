#ifndef comp_image_header
#define comp_image_header

// Function declaration for comparing the magic numbers of two image structs
int comp_magic_number(image_struct_type *image_struct, image_struct_type *image_struct_compare);

// Function declaration for comparing the dimensions of two image structs
int comp_dimensions(image_struct_type *image_struct, image_struct_type *image_struct_compare);

// Function declaration for comparing the image data of two image structs
int comp_image_data(image_struct_type *image_struct, image_struct_type *image_struct_compare);

#endif