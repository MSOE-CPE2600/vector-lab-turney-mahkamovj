/* vector.c
 *
 * Implementation of vector operations
 *
 *
 * Author: Jaffar Mahkamov
 * Date: 9/29/25
 */

#include "vector.h"
#include <stdio.h>

vect add(vect a, vect b) {
    vect result = { "", {0,0,0}, 1 };
    result.comp[0] = a.comp[0] + b.comp[0];
    result.comp[1] = a.comp[1] + b.comp[1];
    result.comp[2] = a.comp[2] + b.comp[2];
    return result;
}

vect subtract(vect a, vect b){
    vect result = { "", {0,0,0}, 1 };
    result.comp[0] = a.comp[0] - b.comp[0];
    result.comp[1] = a.comp[1] - b.comp[1];
    result.comp[2] = a.comp[2] - b.comp[2];
    return result;
}

vect scalar_mult(vect a, double k){
    vect result = { "", {0,0,0}, 1};
    result.comp[0] = a.comp[0] * k;
    result.comp[1] = a.comp[1] * k;
    result.comp[2] = a.comp[2] * k;
    return result;
}

vect crossvec(vect a, vect b){
    vect result = { "", {0,0,0}, 1 };
    result.comp[0] = a.comp[1] * b.comp[2] - a.comp[2] * b.comp[1];
    result.comp[1] = a.comp[2] * b.comp[0] - a.comp[0] * b.comp[2];
    result.comp[2] = a.comp[0] * b.comp[1] - a.comp[1] * b.comp[0];
    return result;
}

double dotvec(vect a, vect b){
    double result = 0;
    result += a.comp[0] * b.comp[0] + a.comp[1] * b.comp[1] + a.comp[2] * b.comp[2];
    return result;
}

void list(vect *vectors){
    for (int i = 0; i < 10; i++){
        if (vectors[i].in_use){
            printf("%s: (%.2f, %.2f, %.2f)\n", vectors[i].name, vectors[i].comp[0], vectors[i].comp[1], vectors[i].comp[2]);
        }
    }
}







