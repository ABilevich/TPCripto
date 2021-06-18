#include "include/interpolation.h"
#include "include/galois_8.h"
#include <stdlib.h>

// values es una matriz donde cada elemento es de la forma [x,y] --> [[1,2],[2,3],[4,5]]
// amount tiene la cantidad de pares [x,y] dentro de values
uint8_t * gauss_interpolation(uint8_t** values,uint8_t amount){
    uint8_t terms = amount + 1;
    uint8_t ** system = (uint8_t *) malloc(amount);

    // Formar la matriz
    for(uint8_t i = 0; i < amount; i++){
        uint8_t x = values[i][0];
        uint8_t y = values[i][1];
        system[i] = (uint8_t *) malloc(terms * sizeof(uint8_t));
        system[i][0] = 1;
        for(uint8_t j = 1; j < amount; j++){
            system[i][j] = mult(x,system[i][j-1]);
        }
        system[i][terms-1] = y;
    }
    

}