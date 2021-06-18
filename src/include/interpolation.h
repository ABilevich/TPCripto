#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <stdint.h>

uint8_t * gauss_interpolation(uint8_t** values,uint8_t amount);
void lagrange_interpolation(uint8_t * x, uint8_t * y, int size, uint8_t * coefs);

#endif