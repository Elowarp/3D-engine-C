/*
 *  Name : Elowan
 *  Creation : 29-04-2024 19:32:51
 *  Last modified : 04-05-2024 20:26:05
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"

/*
Swaps the i-th and j-th elements of `array` of any types but 
that where its elements has the size `elmt_size`
*/
void swap(void* array, size_t elmt_size, int i, int j){
    void* tmp = malloc(elmt_size);
    void* a = malloc(elmt_size);
    void* b = malloc(elmt_size);

    a = array + elmt_size*i;
    b = array + elmt_size*j;

    memcpy(tmp, a, elmt_size);
    memcpy(a, b, elmt_size);
    memcpy(b, tmp, elmt_size);

    free(tmp);
}

/*
Sorts descending any array `keys` by its value-corresponding array `values`
of size `length`, elements of `keys`should have the size `elmt_size` 
*/
void sortArrayDesc(void* keys, double* values, int length, size_t elmt_size){
    // Insertion sorting
    for(int i = 1; i < length; i++){
        int j = i;
        while(j > 0 && values[j-1] < values[j]){
            swap(values, sizeof(double), j, j-1);
            swap(keys, elmt_size, j, j-1);
            j--;
        }
    }
}