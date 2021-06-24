#include "include/interpolation.h"
#include "include/galois_8.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// og_x, og_y arrays de coordenadas x e y respectivamente
// k numero minimo de sombras a partir de las cuales puedo recuperar el secreto
// coefs: array dentro del cual se incluyen los resultados de la interpolacion
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