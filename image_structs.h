typedef struct{
    // create a char array to hold magic number
    unsigned char magicNumber[2];
    unsigned short *magicNumberValue;

    // create and initialise variables used within code
    int width; 
    int height;
    int check;

    // image data 
    unsigned int *imageData;
    long numBytes;
} image_struct_type;