
#ifndef image_struct_h
#define image_struct_h

typedef struct{
    // create a char array to hold magic number
    unsigned char magic_number[2];
    unsigned short *magic_number_value;

    // create and initialise variables used within code
    int width; 
    int height;
    int check;

    // image data 
    unsigned int **imageData;
    long numBytes;

} image_struct_type;

#endif