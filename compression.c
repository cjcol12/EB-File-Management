#include <stdio.h>
#include <stdlib.h>

#define ROW 1
#define COL 16

unsigned int *arr[ROW];
unsigned int **array2d = arr;


void fill_array(){

    for(int i = 0; i <  ROW; i++) {
        array2d[i] = (unsigned int *)malloc(COL * sizeof(unsigned int));
    }
    
    int count = 0;
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            array2d[i][j] = count;
            count++;
        }
    }
}

void fill_array_manual(){

    for(int i = 0; i < ROW; i++) {
        array2d[i] = (unsigned int *)malloc(COL * sizeof(unsigned int));
    }
    
    array2d[0][0] = 31;
    array2d[0][1] = 0;
    array2d[0][2] = 1;
    array2d[0][3] = 15;
    array2d[0][4] = 17;
    array2d[0][5] = 12;
    array2d[0][6] = 25;
    array2d[0][7] = 4;

    array2d[0][8] = 31;
    array2d[0][9] = 0;
    array2d[0][10] = 1;
    array2d[0][11] = 15;
    array2d[0][12] = 17;
    array2d[0][13] = 12;
    array2d[0][14] = 25;
    array2d[0][15] = 4;
    
    // array2d[1][0] = 31;
    // array2d[1][1] = 0;
    // array2d[1][2] = 1;
    // array2d[1][3] = 15;
    // array2d[1][4] = 17;
    // array2d[1][5] = 12;
    // array2d[1][6] = 25;
    // array2d[1][7] = 4;
    
}

void display_array(){
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++){
            printf("%d ", array2d[i][j]);
        }
        printf("\n");
    }
}

void compress(){
    int count; 
    int compressed_byte_counter = 0;
    

    for(int i = 0; i < ROW; i++) {
        count = 0;
        for(int j = 0; j < COL; j++){

            unsigned int next_element = (j < COL - 1) ? array2d[i][j+1] : 0;
            unsigned int element_2_away = (j < COL - 2) ? array2d[i][j+2] : 0;
            unsigned int element_3_away = (j < COL - 3) ? array2d[i][j+3] : 0;
            unsigned int element_4_away = (j < COL - 4) ? array2d[i][j+4] : 0;


            // printf("next element is %d\nelement 2 away is %d\n\n", next_element, element_2_away);

            switch(count){
                case 0:
                    array2d[i][j] <<= 27;
                    array2d[i][j] >>= 24;

                    next_element <<= 27;
                    next_element >>= 24;

                    next_element >>= 5;
                    array2d[i][j] |= next_element;
                    break;

                case 1:
                    array2d[i][j] <<= 30;
                    array2d[i][j] >>= 24;

                    next_element <<= 25;
                    next_element >>= 24;

                    array2d[i][j] |= next_element;
        
                    element_2_away <<= 27;
                    element_2_away >>= 31;

                    array2d[i][j] |= element_2_away;
                    break;

                case 2:
                    next_element <<= 28;
                    next_element >>= 24;

                    element_2_away <<= 27;
                    element_2_away >>= 28;

                    next_element |= element_2_away;
                    array2d[i][j] = next_element;
                    break;

                case 3:
                    next_element <<= 31;
                    next_element >>= 24;

                    element_2_away <<= 26;
                    element_2_away >>= 24;

                    next_element |= element_2_away;
                
                    element_3_away >>= 3;
                    next_element |= element_3_away;
                    array2d[i][j] = next_element;
                    break;

                case 4:
                    element_2_away <<= 29;
                    element_2_away >>= 24;

                    element_2_away |= element_3_away;
                    array2d[i][4] = element_2_away;
            
            }
            count ++;

            // if (count == 5){
            //     printf(" j is before %d\n", j);
            //     compressed_byte_counter ++;
            //     j = compressed_byte_counter * 8;
            //     printf(" j is after %d\n", j);
            //     count = 0;
            // }
        }

    }
}

void next_element(){
    int current_row = 0;
    int current_col = 0;
    int next_row = current_row;
    int next_col = current_col + 1;

    if (next_col >= COL) {
    next_row++;
    next_col = 0;
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
    compress();
    display_array();
    
    print_binary(array2d[0][1]);
}