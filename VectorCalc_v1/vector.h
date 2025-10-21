/* vector.h
 *
 * Header file for vector operations
 *
 * Author: Jaffar Mahkamov
 * Date: 9/29/25
 */
#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    char name[16];   
    double comp[3];  
    int in_use;      
} vect;

vect add(vect a, vect b);
vect subtract(vect a, vect b);
vect scalar_mult(vect a, double k);
vect crossvec(vect a, vect b);
double dotvec(vect a, vect b);

#endif
