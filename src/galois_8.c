#include "include/galois_8.h"
#include <stdio.h>
#define MAX_BIT_REP 255

int pol_gen = 355;  // 1 0110 0011 --> x^8 + x^6 + x^5 + x + 1
int grado_pol_gen = 8;

int pol_gen_2 = 7; // 0111 --> x^2 + x + 1


// int main(){
//     uint8_t a = 84;  // 0101 0100
//     uint8_t b = 13;  // 0000 1101
    
//     // printf("%d \n", sum(a,b)); // 0101 1001 = 89
//     // printf("%d \n", mult(a,b));
    
//     uint8_t asd[4] = {1, 6, 0, 3};
//     printf("pol en %d: %d\n", 3, evaluatePolinomial(3,asd,4));
//     return 0;
// }

static uint8_t inv_table[1<<8]={0, 1, 177, 222, 233, 74, 111, 140, 197, 165, 37, 193, 134, 84, 70, 231, 211, 93, 227, 133, 163, 52, 209, 237, 67, 166, 42, 99, 35, 158, 194, 119, 216, 45, 159, 28, 192, 10, 243, 171, 224, 181, 26, 98, 217, 33, 199, 189, 144, 86, 83, 234, 21, 162, 128, 121, 160, 91, 79, 229, 97, 73, 138, 205, 108, 154, 167, 24, 254, 124, 14, 230, 96, 61, 5, 232, 200, 130, 228, 58, 112, 106, 235, 50, 13, 135, 49, 145, 221, 246, 161, 57, 210, 17, 239, 191, 72, 60, 43, 27, 152, 213, 117, 151, 187, 184, 81, 113, 64, 155, 141, 6, 80, 107, 156, 142, 150, 102, 195, 31, 129, 55, 149, 178, 69, 255, 215, 136, 54, 120, 77, 201, 226, 19, 12, 85, 127, 214, 62, 204, 7, 110, 115, 157, 48, 87, 175, 248, 179, 122, 116, 103, 100, 212, 65, 109, 114, 143, 29, 34, 56, 90, 53, 20, 196, 9, 25, 66, 183, 172, 242, 39, 169, 182, 249, 146, 223, 2, 123, 148, 225, 41, 173, 168, 105, 186, 185, 104, 198, 47, 238, 95, 36, 11, 30, 118, 164, 8, 188, 46, 76, 131, 219, 253, 139, 63, 250, 245, 236, 22, 92, 16, 153, 101, 137, 126, 32, 44, 252, 202, 247, 88, 3, 176, 40, 180, 132, 18, 78, 59, 71, 15, 75, 4, 51, 82, 208, 23, 190, 94, 241, 240, 170, 38, 251, 207, 89, 220, 147, 174, 206, 244, 218, 203, 68, 125};

uint8_t g_evaluatePolinomial(uint8_t x, uint8_t* segment, int degree){
    uint8_t total = 0, resp;
    for(int i = 0; i < degree ; i++){
        uint8_t aux = 1;
        for(int j = 0; j < i; j++){
            // printf("i: %d, j: %d \n",i,j);
            aux = g_mult(x,aux);
        }
        resp = g_mult(segment[i], aux);
        // printf("%d x %d = %u\n",segment[i], aux,resp);
        aux = total;
        total = g_sum(total, resp);
        // printf("%d + %d = %u\n",aux, resp,total);
    }
    return total;
}


uint16_t g_sum(uint8_t s1, uint8_t s2){
    return s1^s2;
}

uint16_t g_sub(uint8_t s1, uint8_t s2){
    return g_sum(s1,s2);
}

uint8_t g_mult(uint8_t s1, uint8_t s2){
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

uint8_t g_div(uint8_t s1, uint8_t s2){
    return g_mult(s1, inv_table[s2]);
}

uint8_t invers_mult(uint16_t a, uint16_t mod) {
    uint16_t v = mod;
    uint16_t g1 = 1;
    uint16_t g2 = 0;
    int j = g_degree(a) - 8;

    uint16_t aux;

    while (a != 1) {
    
        if (j < 0) {
            aux = v;
            v = a;
            a = aux;

            aux = g1;
            g1 = g2;
            g2 = aux;

            j = -j;
        }

        a ^= v << j;
        g1 ^= g2 << j;

        a %= 256;  // Emulating 8-bit overflow
        g1 %= 256; // Emulating 8-bit overflow

        j = g_degree(a) - g_degree(v);
    }

    return (uint8_t) g1;
}

uint8_t g_degree(uint8_t a) {
    uint8_t res = 0;
    a >>= 1;
    while (a != 0) {
        a >>= 1;
        res += 1;
    }
    return res;
}