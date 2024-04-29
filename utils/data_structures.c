/*
 *  Name : Elowan
 *  Creation : 29-04-2024 19:32:51
 *  Last modified : 29-04-2024 20:05:22
 */
#include <stdio.h>
#include <stdlib.h>

#include "data_structures.h"


/*
Sorts descending any array `keys` by its value-corresponding array `values`
of size `n` 
*/
void sortArrayDesc(void** keys, double* values, int n){
    // Insertion sorting
    for(int i =0; i < n-1; i++){
        void* k = keys[i];
        double v = values[i];
        int j = i;
        while(j > 0 && values[j-1] < v){
            values[j] = values[j-1];
            keys[j] = keys[j-1];
            j--;
        }
        values[j] = v;
        keys[j] = k;
    }
}