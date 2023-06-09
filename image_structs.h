#ifndef image_struct_h
#define image_struct_h

typedef struct
{
    // create a char array to hold magic number
    unsigned char magic_number[2];
    unsigned short *magic_number_value;

    // create and initialise variables used within code
    int width;
    int compressed_width;
    int height;
    int check;

    // image data
    unsigned int **imageData;
    unsigned int* array;
    unsigned int *data_block;

    // total image size data
    long numBytes;
    long compressed_numBytes;

} image_struct_type;

#endif