#include <stdio.h>
#include <stdlib.h>

#define ROW 2
#define COL 13

void print_binary(int num);


unsigned int *arr[ROW];
unsigned int **array2d = arr;
unsigned int compressed_array[2][11] = {{0}};

unsigned int uncompressed_array[2][11] = {{0}};

void fill_array_manual(){

    for(int i = 0; i < ROW; i++) {
        array2d[i] = (unsigned int *)malloc(COL * sizeof(unsigned int));
    }


    
    array2d[0][0] = 14;
    array2d[0][1] = 15;
    array2d[0][2] = 31;
    array2d[0][3] = 20;
    array2d[0][4] = 9;
    array2d[0][5] = 4;
    array2d[0][6] = 29;
    array2d[0][7] = 18;

    array2d[0][8] = 0;
    array2d[0][9] = 0;
    // array2d[0][10] = 1;
    // array2d[0][11] = 15;
    // array2d[0][12] = 17;
    // array2d[0][13] = 12;
    // array2d[0][14] = 25;
    // array2d[0][15] = 4;
    
    array2d[1][0] = 0;
    array2d[1][1] = 0;
    array2d[1][2] = 0;
    array2d[1][3] = 0;
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
            printf("%d ", array2d[i][j]);
        }
        printf("\n");
    }
}

void display_array_uncomp(){
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 15; j++){
            printf("%d ", uncompressed_array[i][j]);
        }
        printf("\n");
    }
}

void display_array_comp(){
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 15; j++){
            printf("%d ", compressed_array[i][j]);
        }
        printf("\n");
    }
}

void decompress(){
    int count = 0;
    int k = 0;
    for(int i = 0; i < ROW; i++) {
        count = 0;
        k = 0;
        for(int j = 0; j < COL; j++) {
            
            unsigned char this_element = compressed_array[i][j];
            unsigned char prev_element = (j > 0) ? compressed_array[i][j-1] : 0;
            unsigned char element_2_away = (j > 1) ? compressed_array[i][j-2] : 0;
            unsigned char element_3_away = (j > 2) ? compressed_array[i][j-3] : 0;

            switch(count){
                case(0):
                    this_element >>= 3;
                    uncompressed_array[i][k] = this_element;
                    print_binary(this_element);
                    break;

                case(1):
                    prev_element <<= 5;
                    prev_element >>= 3;

                    this_element >>= 6;
                    this_element |= prev_element;
                    uncompressed_array[i][k] = this_element;
                    break;
                
                case(2):
                    prev_element <<= 2;
                    prev_element >>= 3;
                    uncompressed_array[i][k] = prev_element;
                    break;

                case(3):
                    element_2_away <<= 7;
                    element_2_away >>= 3;

                    prev_element >>= 4;
                    
                    prev_element |= element_2_away;
                    uncompressed_array[i][k] = prev_element;
                    break;
                
                case(4):
                    element_2_away <<= 4;
                    element_2_away >>= 3;

                    prev_element >>= 7;
                    element_2_away |= prev_element;
                    uncompressed_array[i][k] = element_2_away;
                    break;

                case(5):
                    element_2_away <<= 1;
                    element_2_away >>= 3;
                    uncompressed_array[i][k] = element_2_away;
                    break;

                case(6):
                    element_3_away <<= 6;
                    element_3_away >>= 3;

                    element_2_away >>= 5;
                    
                    element_3_away |= element_2_away;
                    uncompressed_array[i][k] = element_3_away;
                    break;

                case(7):
                    element_3_away <<= 3;
                    element_3_away >>= 3;
                    uncompressed_array[i][k] = element_3_away;


            }
            count ++;
            k++;

            // if (count == 8){
            //     decompressed_byte ++
            //     j = compressed_byte_counter * 7;

            // }


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
                compressed_byte_counter ++;
                j = compressed_byte_counter * 7;
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
    printf("\n\n");

    compress();
    display_array_comp();
    printf("\n\n");


    decompress();
    display_array_uncomp();
    
    // print_binary(array2d[0][1]);
}