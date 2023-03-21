#include <stdio.h>
#include <stdlib.h>
#include<time.h>


#define ROW 10
#define COL 72

void print_binary(int num);


unsigned int *arr[ROW];
unsigned int **array2d = arr;
unsigned int compressed_array[ROW][45] = {{0}};

unsigned int uncompressed_array[ROW][COL] = {{0}};

void fill_array_manual(){

    for(int i = 0; i < ROW; i++) {
        array2d[i] = (unsigned int *)malloc(COL * sizeof(unsigned int));
    }

    srand(time(NULL));

    for(int i = 0 ; i < ROW ; i++ ) {
        for(int j = 0; j < COL; j++)
            array2d[i][j] = rand() % 31;
    }
    
    // array2d[0][0] = 17;
    // array2d[0][1] = 25;
    // array2d[0][2] = 14;
    // array2d[0][3] = 5;
    // array2d[0][4] = 31;
    // array2d[0][5] = 19;
    // array2d[0][6] = 10;
    // array2d[0][7] = 28;
    
    // array2d[0][8] = 17;
    // array2d[0][9] = 25;
    // array2d[0][10] = 14;
    // array2d[0][11] = 5;
    // array2d[0][12] = 31;
    // array2d[0][13] = 19;
    // array2d[0][14] = 10;
    // array2d[0][15] = 28;

    // array2d[0][16] = 17;
    // array2d[0][17] = 25;
    // array2d[0][18] = 14;
    // array2d[0][19] = 5;
    // array2d[0][20] = 31;
    // array2d[0][21] = 19;
    // array2d[0][22] = 10;
    // array2d[0][23] = 28;

    // array2d[0][24] = 17;
    // array2d[0][25] = 25;
    // array2d[0][26] = 14;
    // array2d[0][27] = 5;
    // array2d[0][28] = 31;
    // array2d[0][29] = 19;
    // array2d[0][30] = 10;
    // array2d[0][31] = 28;

    // array2d[0][8] = 0;
    // array2d[0][9] = 0;
    // array2d[0][10] = 1;
    // array2d[0][11] = 15;
    // array2d[0][12] = 17;
    // array2d[0][13] = 12;
    // array2d[0][14] = 25;
    // array2d[0][15] = 4;
    
    // array2d[1][0] = 0;
    // array2d[1][1] = 0;
    // array2d[1][2] = 0;
    // array2d[1][3] = 0;
    // array2d[1][4] = 17;
    // array2d[1][5] = 12;
    // array2d[1][6] = 25;
    // array2d[1][7] = 4;

    // array2d[1][8] = 31;
    // array2d[1][9] = 0;
    // array2d[1][10] = 1;
    // array2d[1][11] = 15;
    // array2d[1][12] = 17;
    // array2d[1][13] = 12;
    // array2d[1][14] = 25;
    // array2d[1][15] = 4;
}

void display_array(){
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++){
            if (j % 8 == 0){
                printf("\t");
            }
            printf("%d ", array2d[i][j]);
        }
        printf("\n");
    }
}

void display_array_uncomp(){
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++){
            if (j % 8 == 0){
                printf("\t");
            }
            printf("%d ", uncompressed_array[i][j]);
        }
        printf("\n");
    }
}

void display_array_comp(){
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < 45; j++){
            if (j % 5 == 0){
                printf("\t");
            }
            printf("%d ", compressed_array[i][j]);
        }
        printf("\n");
    }
}

int compare_array(){
    for(int i = 0 ; i < ROW ; i++ ) {
        for(int j = 0; j < COL; j++){
            if (array2d[i][j] != uncompressed_array[i][j]){
                printf("\nERROR!!!\n");
                return 0;
            }
        }
    }
    printf("NO ERROR!!!\n");
    return 1;
}

void decompress(){
    int count = 0;
    int k = 0;
    int compressed_byte_counter;
    for(int i = 0; i < ROW; i++) {
        count = 0;
        k = 0;
        compressed_byte_counter = 0;
        for(int j = 0; j < COL; j++) {
            
            unsigned char this_element = compressed_array[i][k];
            unsigned char prev_element = (k > 0) ? compressed_array[i][k-1] : 0;
            unsigned char element_2_away = (k > 1) ? compressed_array[i][k-2] : 0;
            unsigned char element_3_away = (k > 2) ? compressed_array[i][k-3] : 0;

            switch(count){
                case(0):
                    this_element >>= 3;
                    uncompressed_array[i][j] = this_element;
                    break;

                case(1):
                    prev_element <<= 5;
                    prev_element >>= 3;

                    this_element >>= 6;
                    this_element |= prev_element;
                    uncompressed_array[i][j] = this_element;
                    break;
                
                case(2):
                    prev_element <<= 2;
                    prev_element >>= 3;
                    uncompressed_array[i][j] = prev_element;
                    break;

                case(3):
                    element_2_away <<= 7;
                    element_2_away >>= 3;

                    prev_element >>= 4;
                    
                    prev_element |= element_2_away;
                    uncompressed_array[i][j] = prev_element;
                    break;
                
                case(4):
                    element_2_away <<= 4;
                    element_2_away >>= 3;

                    prev_element >>= 7;
                    element_2_away |= prev_element;
                    uncompressed_array[i][j] = element_2_away;
                    break;

                case(5):
                    element_2_away <<= 1;
                    element_2_away >>= 3;
                    uncompressed_array[i][j] = element_2_away;
                    break;

                case(6):
                    element_3_away <<= 6;
                    element_3_away >>= 3;

                    element_2_away >>= 5;
                    
                    element_3_away |= element_2_away;
                    uncompressed_array[i][j] = element_3_away;
                    break;

                case(7):
                    element_3_away <<= 3;
                    element_3_away >>= 3;
                    uncompressed_array[i][j] = element_3_away;
                    break;

            }
            count ++;
            k++;

            if (count == 8){
                count = 0;
                k = 5;
                compressed_byte_counter ++;
                k = compressed_byte_counter * 5;
            }



        }
    }
}

void compress(){
    int count; 
    int compressed_byte_counter = 0;
    int k;
    

    for(int i = 0; i < ROW; i++) {
        count = 0;
        k = 0;
        compressed_byte_counter = 0;
        for(int j = 0; j < COL; j++){

            if(count == 0){
                j = compressed_byte_counter * 8;
                compressed_byte_counter ++;
            }

            unsigned int this_element = array2d[i][j];
            unsigned int next_element = (j < COL - 1) ? array2d[i][j+1] : 0;
            unsigned int element_2_away = (j < COL - 2) ? array2d[i][j+2] : 0;
            unsigned int element_3_away = (j < COL - 3) ? array2d[i][j+3] : 0;

            switch(count){
                case 0:
                    this_element <<= 27;
                    this_element >>= 24;

                    next_element <<= 27;
                    next_element >>= 29;

                    this_element |= next_element;
                    compressed_array[i][k] = this_element;

                    break;

                case 1:
                    this_element <<= 30;
                    this_element >>= 24;

                    next_element <<= 25;
                    next_element >>= 24;

                    this_element |= next_element;
        
                    element_2_away <<= 27;
                    element_2_away >>= 31;

                    this_element |= element_2_away;
                    compressed_array[i][k] = this_element;
                    break;

                case 2:
                    //print_binary(next_element);
                    next_element <<= 28;
                    next_element >>= 24;

                    element_2_away <<= 27;
                    element_2_away >>= 28;

                    next_element |= element_2_away;
                    compressed_array[i][k] = next_element;
                    break;

                case 3:
                    next_element <<= 31;
                    next_element >>= 24;

                    element_2_away <<= 26;
                    element_2_away >>= 24;

                    next_element |= element_2_away;
                
                    element_3_away >>= 3;
                    next_element |= element_3_away;
                    compressed_array[i][k] = next_element;
                    break;

                case 4:
                    element_2_away <<= 29;
                    element_2_away >>= 24;

                    element_2_away |= element_3_away;
                    compressed_array[i][k] = element_2_away;
                    break;            
            }
            count ++;
            k++;

            if (count == 5){
                count = 0;
                // compressed_byte_counter ++;
                // j = compressed_byte_counter * 8;
            }
        }


    }
}

void print_binary(int num) {
    int i;
    for (i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}


int main(char **argv, int argc){
    // fill_array();
    fill_array_manual();
    display_array();
    printf("\n");

    compress();
    display_array_comp();
    printf("\n");


    decompress();
    display_array_uncomp();
    
    compare_array();
    return 0;
}