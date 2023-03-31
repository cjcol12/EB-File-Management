#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WIDTH 29
#define HEIGHT 6
#define BLOCK_SIZE 3
#define COMPRESSED_HEIGHT 10
#define COMPRESSED_WIDTH 10

void fill_array();
void print_array();
void print_uncompressed();
void print_compressed();
void ebcBlock();
void ebcUnblock();

int image[HEIGHT][WIDTH];
int compressed[COMPRESSED_HEIGHT][COMPRESSED_WIDTH];
int uncompressed[HEIGHT][WIDTH];
int block_height = ceil((double)HEIGHT / BLOCK_SIZE);
int block_width = ceil((double)WIDTH / BLOCK_SIZE);


int main() {

    fill_array();

    ebcBlock();

    print_array();
    printf("\n");
    print_compressed();

    ebcUnblock();
    print_uncompressed();



    return 0;
}

void fill_array(){
    srand(time(NULL));

    for(int i = 0 ; i < HEIGHT ; i++ ) {
        for(int j = 0; j < WIDTH; j++)
            image[i][j] = rand() % 31;
    }
}

void print_array(){
    // Print the original 2D array
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            printf("%2d ", image[i][j]);
            if ((j + 1) % BLOCK_SIZE == 0 && j != WIDTH - 1) {
                printf("    ");
            }
        }
        printf("\n");
        if ((i + 1) % BLOCK_SIZE == 0 && i != HEIGHT - 1) {
            printf("\n");

        }
    }
}

void print_uncompressed(){
    // Print the original 2D array
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            printf("%2d ", uncompressed[i][j]);
            if ((j + 1) % BLOCK_SIZE == 0 && j != WIDTH - 1) {
                printf("    ");
            }
        }
        printf("\n");
        if ((i + 1) % BLOCK_SIZE == 0 && i != HEIGHT - 1) {
            printf("\n");

        }
    }
}

void print_compressed(){
    // Print the compressed 2D array
    for (int i = 0; i < block_height; ++i) {
        for (int j = 0; j < block_width; ++j) {
            printf("%2d           ", compressed[i][j]);
        }
        printf("\n");
    }
}

void ebcBlock() {
    int sums[WIDTH] = {0};
    int counts[WIDTH] = {0};

    for (int row = 0; row < HEIGHT; ++row) {
        int colSum = 0;
        int colCount = 0;

        for (int col = 0; col < WIDTH; ++col) {
            colSum += image[row][col];
            colCount++;

            if (colCount == BLOCK_SIZE || col == WIDTH - 1) {
                sums[col / BLOCK_SIZE] += colSum;
                counts[col / BLOCK_SIZE] += colCount;
                colSum = 0;
                colCount = 0;
            }
        }

        if ((row + 1) % BLOCK_SIZE == 0 || row == HEIGHT - 1) {
            for (int blockCol = 0; blockCol < block_width; ++blockCol) {
                compressed[row / BLOCK_SIZE][blockCol] = round((double)sums[blockCol] / counts[blockCol]);
                sums[blockCol] = 0;
                counts[blockCol] = 0;
            }
        }
    }
}







// refactor 
void ebcUnblock() {
    int row, col, comp_i, comp_j;
    for (row = 0; row < HEIGHT; row++) {
        comp_i = row / BLOCK_SIZE;
        for (col = 0; col < WIDTH; col++) {
            comp_j = col / BLOCK_SIZE;
            uncompressed[row][col] = compressed[comp_i][comp_j];
        }
    }
}