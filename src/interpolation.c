#include "include/interpolation.h"
#include "include/galois_8.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// values es una matriz donde cada elemento es de la forma [x,y] --> [[1,2],[2,3],[4,5]]
// amount tiene la cantidad de pares [x,y] dentro de values
// uint8_t * gauss_interpolation(uint8_t** values,uint8_t amount){
//     uint8_t terms = amount + 1;
//     uint8_t ** system = (uint8_t *) malloc(amount);

//     // Formar la matriz
//     for(uint8_t i = 0; i < amount; i++){
//         uint8_t x = values[i][0];
//         uint8_t y = values[i][1];
//         system[i] = (uint8_t *) malloc(terms * sizeof(uint8_t));
//         system[i][0] = 1;
//         for(uint8_t j = 1; j < amount; j++){
//             system[i][j] = mult(x,system[i][j-1]);
//         }
//         system[i][terms-1] = y;
//     }
    
// }


// void lagrange_interpolation(uint8_t * oc_x, uint8_t * oc_y, int size, uint8_t * coefs){
//     uint8_t x[size];
// 	uint8_t y[size];

//     for(int p=0; p<size; p++){
//         x[p] = oc_x[p];
//         y[p] = oc_y[p];
//     }

//     for(int h=0; h<size; h++){
//         if(x[h] == 0){
//             uint8_t aux_x = x[0];
//             uint8_t aux_y = y[0];
//             x[0] = x[h];
//             y[0] = y[h];
//             x[h] = aux_x;
//             y[h] = aux_y;
//             break;
//         }
//     }
//     for(int j=0; j<size-1; j++){
//         uint8_t L0[size];
//         for(int i=j; i<size; i++){
//             uint8_t prod = 0;
//             for(int k=j; k<size; k++){
//                 if(i != k){
//                     uint8_t upper = g_sub(0, x[k]);
//                     uint8_t bot = g_sub(x[i], x[k]);
//                     uint8_t aux = g_div(upper, bot);
//                     if(prod == 0){
//                         prod = aux;
//                     }else{
//                         prod = g_mult(prod, aux);
//                     }
//                 }
//             }
//             L0[i] = prod;
//         }
//         int acumEmpty = 1;
//         uint8_t acum = 0;
//         for(int i=j; i<size; i++){
//             uint8_t aux = g_mult(L0[i], y[i]);
//             if(acumEmpty){
//                 acum = aux;
//                 acumEmpty = 0;
//             }else{
//                 acum = g_sum(acum, aux);
//             }
//         }
//         coefs[j] = acum;
        
//         for(int i=j+1; i<size; i++){
//             uint8_t upper = g_sub(y[i], coefs[j]);
//             uint8_t bot = x[i];
//             uint8_t aux = g_div(upper, bot);
//             y[i] = aux;
//         }
        
//     }
//     coefs[size-1] = y[size-1];
// }


int lagrange_interpolation(uint8_t * og_x, uint8_t * og_y, int k, uint8_t * coefs) {

    uint8_t L0[k];
    uint8_t s[k];

    uint8_t numerator;
    uint8_t denominator;
    uint8_t aux;

    uint8_t x[k];
    memcpy(x, og_x, k*sizeof(x[0]));
    uint8_t y[k];
    memcpy(y, og_y, k*sizeof(y[0]));

    memset((void *) L0, 0, k*sizeof(uint8_t));
    memset((void *) s, 0, k*sizeof(uint8_t));

    int acum_is_empty;
    uint8_t acum;
    uint8_t aux_x, aux_y;

    for (int l = 0; l < k; l++) {
        if (x[l] == 0) {
            aux_x = x[0];
            aux_y = y[0];
            x[0] = x[l];
            y[0] = y[l];
            x[l] = aux_x;
            y[l] = aux_y;
            break;
        }
    }

    for(int j=0; j<k-1; j++) {

        // L(0)
        for(int i =j; i < k; i++){
            acum_is_empty = 1;
            acum = 0;

            for(int h = j; h < k; h++){
                if (h != i) {
                    numerator = g_sub(0, x[h]);
                    denominator = g_sub(x[i], x[h]);
                    aux = g_div(numerator, denominator);

                    if (acum_is_empty) {
                        acum_is_empty = 0;
                        acum = aux;
                    }
                    else {
                        acum = g_mult(acum, aux);
                    }
                }
            }

            L0[i] = acum;
        }

        acum_is_empty = 1;
        acum = 0;
        
        for(int i=j; i<k; i++) {
            aux = g_mult(L0[i], y[i]);

            if (acum_is_empty) {
                acum_is_empty = 0;
                acum = aux;
            }
            else {
                acum = g_sum(acum, aux);
            }
        }

        s[j] = acum;

        for(int i=j+1; i<k; i++) {
            numerator = g_sub(y[i], s[j]);
            denominator = x[i];
            aux = g_div(numerator, denominator);

            y[i] = aux;
        }
    }
    s[k-1] = y[k-1];

    memcpy(coefs, s, k * sizeof(coefs[0]));

    return 0;
}