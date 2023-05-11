#include <stdio.h>
#include <stdint.h>

int main() {
    const char *filename = "sample_images/output.ebc";
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    // PGM header
    fprintf(file, "ec\n");
    fprintf(file, "2 16\n");


    // Pixel values
    uint8_t pixels[2][10] = {
        {204, 73, 230, 64, 147, 72, 160, 54, 45, 27},
        {248, 2, 248, 179, 36, 72, 160, 54, 45, 27}
        // {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        // {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}
    };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 10; j++) {
            fwrite(&pixels[i][j], sizeof(uint8_t), 1, file);
        }
    }

    fclose(file);
    printf("PGM file created: %s\n", filename);
    return 0;
}
