#include "include/interpolation.h"
#include "include/galois_8.h"
#include <stdlib.h>
#include <stdio.h>
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


void lagrange_interpolation(uint8_t * oc_x, uint8_t * oc_y, int size, uint8_t * coefs){
    //printf("1\n");
    uint8_t x[size];
	uint8_t y[size];

    for(int p=0; p<size; p++){
        x[p] = oc_x[p];
        y[p] = oc_y[p];
    }

    for(int h=0; h<size; h++){
        if(x[h] == 0){
            uint8_t aux_x = x[0];
            uint8_t aux_y = y[0];
            x[0] = x[h];
            y[0] = y[h];
            x[h] = aux_x;
            y[h] = aux_y;
            break;
        }
    }
   //printf("2\n");
    for(int j=0; j<size-1; j++){
        uint8_t L0[size];
        //printf("3\n");
        for(int i=j; i<size; i++){
            uint8_t prod = 0;
            //printf("4\n");
            for(int k=j; k<size; k++){
                //printf("5\n");
                if(i != k){
                    //printf("5.1\n");
                    uint8_t upper = g_sub(0, x[k]);
                    //printf("5.2\n");
                    uint8_t bot = g_sub(x[i], x[k]);
                    //printf("5.3\n");
                    uint8_t aux = g_div(upper, bot);
                    //printf("6\n");
                    if(prod == 0){
                        prod = aux;
                    }else{
                        prod = g_mult(prod, aux);
                    }
                }
            }
            L0[i] = prod;
        }
        //printf("7\n");
        uint8_t acum = 0;
        for(int i=j; i<size; i++){
            uint8_t aux = g_mult(L0[i], y[i]);
            if(acum == 0){
                acum = aux;
            }else{
                acum = g_sum(acum, aux);
            }
        }
        coefs[j] = acum;
        //printf("8\n");
        if(j<size-1){
            for(int i=j+1; i<size; i++){
                //printf("9\n");
                uint8_t upper = g_sub(y[i], coefs[j]);
                uint8_t bot = x[i];
                uint8_t aux = g_div(upper, bot);
                y[i] = aux;
            }
        }
    }
    //printf("10\n");
    coefs[size-1] = y[size-1];
}