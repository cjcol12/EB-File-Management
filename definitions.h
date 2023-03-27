#ifndef definition_header
#define definition_header

// success definitions
#define SUCCESS 0
#define SUCCESS_DIFFERENT 0
#define USAGE_REQUEST 0
#define FUNCTION_SUCCESS 0
#define FUNCTION_SUCCESS_DIFFERENT 100

// error definitions
#define BAD_ARGUMENT_COUNT 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIMENSION 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define BAD_WRITE_PERMISSIONS 8

// constant definitions
#define MAGIC_NUMBER_EB 0x6265
#define MAGIC_NUMBER_EU 0x7565
#define MAGIC_NUMBER_EC 0x6365

#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1
#define MAX_GRAY_COMPRESSED 255
#define MAX_GRAY 31
#define MIN_GRAY 0

#endif