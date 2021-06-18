#ifndef GALOIS_8_H
#define GALOIS_8_H
#include <stdint.h>

uint8_t g_evaluatePolinomial(uint8_t x, uint8_t* segment,int degree);
uint16_t g_sum(uint8_t s1, uint8_t s2);
uint16_t g_sub(uint8_t s1, uint8_t s2);
uint8_t g_mult(uint8_t s1, uint8_t s2); 
uint8_t g_div(uint8_t s1, uint8_t s2);
uint8_t invers_mult(uint16_t a, uint16_t mod);
uint8_t g_degree(uint8_t a);

#endif

