#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <stdint.h>

uint8_t * gauss_interpolation(uint8_t** values,uint8_t amount);
int lagrange_interpolation(uint8_t * og_x, uint8_t * og_y, int k, uint8_t * coefs);

#endif