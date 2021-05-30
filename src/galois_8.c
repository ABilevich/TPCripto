#include "include/galois_8.h"
#include <stdio.h>

int pol_gen = 355;  // 1 0110 0011 --> x^8 + x^6 + x^5 + x + 1
int grado_pol_gen = 8;

int pol_gen_2 = 7; // 0111 --> x^2 + x + 1

uint16_t sum(uint8_t s1, uint8_t s2);
uint8_t mult(uint8_t s1, uint8_t s2); 

// int main(){
//     uint8_t a = 84;  // 0101 0100
//     uint8_t b = 13;  // 0000 1101
    
//     // printf("%d \n", sum(a,b)); // 0101 1001 = 89
//     // printf("%d \n", mult(a,b));
    
//     uint8_t asd[4] = {1, 6, 0, 3};
//     printf("pol en %d: %d\n", 3, evaluatePolinomial(3,asd,4));
//     return 0;
// }

uint8_t evaluatePolinomial(uint8_t x, uint8_t* segment, int degree){
    uint8_t total = 0, resp;
    for(int i = 0; i < degree ; i++){
        uint8_t aux = 1;
        for(int j = 0; j < i; j++){
            // printf("i: %d, j: %d \n",i,j);
            aux = mult(x,aux);
        }
        resp = mult(segment[i], aux);
        // printf("%d x %d = %u\n",segment[i], aux,resp);
        aux = total;
        total = sum(total, resp);
        // printf("%d + %d = %u\n",aux, resp,total);
    }
    return total;
}


uint16_t sum(uint8_t s1, uint8_t s2){
    return s1^s2;
}

uint8_t mult(uint8_t s1, uint8_t s2){
    // printf("----- Multiplication ------\n");
    uint16_t total = 0;
    uint8_t shift = 1;
    // 0110 y 0011
    // x^2+x y x+1 --> 1100 XOR 0110 = 1010
    for (uint8_t i = 0; i < 8;i++){
        // printf("aux: %d\n", aux);
        if((s2 & shift) == shift){
            // printf("Entered with: %u and i: %u\n",shift, i);
            // printf("value: %u\n", s1 << i);
            total = total ^ (s1 << i);
        }
        shift = shift << 1;  
                                                                            
    }
    // printf("After multiplication total: %u\n", total);
    // printf("----- Reducing ------\n");
    
    // Reducimos Counter con int pol_gen = 355;  // 1 0110 0011 --> x^8 + x^6 + x^5 + x + 1
    int keep_going = 1;
    uint16_t shifted_m;
    int i;
    int diff;
    do{
        i = 0;
        while((total >> i) > 1){
            i++;
        } // obtenemos el grado de total
        // printf("i:%d\n",i);
        diff = i - grado_pol_gen;
        if(diff >= 0){
            // printf("REDUCING");
            shifted_m = pol_gen << diff;
            total = total ^ shifted_m;
        }
    }
    while(diff >= 0);
    // printf("After reduction total: %u\n", total);
    return (uint8_t) total;
}

