/* main.c
 * Vector Calculator implementation
 * Author: Jaffar Mahkamov
 * Date: 9/29/25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

#define MAX_VECS 10

static vect vectors[MAX_VECS];

// Add or update a vector in the array
int addvect(vect v){
    for (int i =0; i< MAX_VECS;i++){
        if (vectors[i].in_use && strcmp(vectors[i].name, v.name) == 0){
            vectors[i]=v;
            return 0;
        }
    }
    
    for(int i= 0;i < MAX_VECS;i++){
        if (!vectors[i].in_use){
            vectors[i] =v;
            vectors[i].in_use =1;
            return 0;
        }
    }
    printf("Error: memory full, cannot add vector.\n");
    return -1;
}

// Find a vector by name
vect *findvect(char *name) {
    for(int i = 0; i < MAX_VECS; i++) {
        if(vectors[i].in_use && strcmp(vectors[i].name,name)==0){
            return &vectors[i];
        }
    }
    return NULL;
}

// Clear all vectors 
void clear(){
    for (int i = 0;i<MAX_VECS;i++){
        vectors[i].in_use = 0;
    }
    printf("All vectors cleared.\n");
}

// List all vectors
void listall() {
    for(int i = 0; i < MAX_VECS; i++){
        if(vectors[i].in_use) {
            printf("%s = %.2f %.2f %.2f\n", vectors[i].name,vectors[i].comp[0], vectors[i].comp[1], vectors[i].comp[2]);
        }
    }
}

// Print help message
void print_help(){
    printf("usage:\n");
    printf("  var = x y z   assign vector\n");
    printf("  var           display vector\n");
    printf("  var1 + var2   add vectors\n");
    printf("  var1 - var2   subtract vectors\n");
    printf("  var * num     scalar multiply\n");
    printf("  num * var     scalar multiply\n");
    printf("  clear         clears all vectors\n");
    printf("  list          lists all vectors\n");
    printf("  quit          exits program\n");
}

void vector_console(){
    char line[128];

    while (1) {
        printf("veclab> ");

        //Read a line from user input
        if (!fgets(line, sizeof(line), stdin))
            break;

        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Skip empty input
        if (strlen(line) == 0)
            continue;

          
        if (strcmp(line, "quit") == 0)
            break;

        if (strcmp(line, "clear") == 0) {
            clear();
            continue;
        }

        if (strcmp(line, "list") == 0) {
            listall();
            continue;
        }

        if (strcmp(line, "-h") == 0) {
            print_help();
            continue;
        }

        // Variables needed for parsing for some reason
        char lhs[16], rhs[100], v1[16], v2[16], op[4];
        double num;

        //Assign a vector : a = x y z  or  a = x y
        if(sscanf(line, "%15s = %[^\n]", lhs, rhs) == 2){
            double x, y, z;
            int is_numeric = 1;

            for(int i =0;rhs[i];i++){
                if ((rhs[i] < '0' || rhs[i] > '9') && rhs[i] != '.' && rhs[i] != '-' && rhs[i] != ' '){
                    is_numeric = 0;
                    break;
                }
            }

            if (is_numeric){
                int count = sscanf(rhs, "%lf %lf %lf", &x, &y, &z);
                if(count >= 2){
                    if(count == 2)z = 0;
                    vect v = {"", {x, y, z}, 1};
                    strcpy(v.name, lhs);
                    addvect(v);
                    printf("%s = %.2f %.2f %.2f\n", v.name, v.comp[0], v.comp[1], v.comp[2]);
                }else{
                    printf("Invalid vector format\n");
                }
                continue;
            }
        }

        // scalar multiplication
        // Ex: c = a * 2  or  c = 2 * a
        if (sscanf(line, "%15s = %15s * %lf", lhs, v1, &num) == 3 ||
            sscanf(line, "%15s = %lf * %15s", lhs, &num, v1) == 3) {

            vect *a = findvect(v1);
            if (!a) {
                printf("Error: vector not found.\n");
                continue;
            }

            vect result = scalar_mult(*a, num);
            strcpy(result.name, lhs);
            result.in_use = 1;
            addvect(result);

            printf("%s = %.2f %.2f %.2f\n",
                   result.name, result.comp[0], result.comp[1], result.comp[2]);
            continue;
        }

        // addition and subtraction assignment
        // Ex: c = a + b  or  d = b - a
        if (sscanf(line, "%15s = %15s %3s %15s", lhs, v1, op, v2) == 4) {
            vect *a = findvect(v1);
            vect *b = findvect(v2);
            if (!a || !b) {
                printf("Error: vector not found.\n");
                continue;
            }

            vect result;
            if (strcmp(op, "+") == 0)
                result = add(*a, *b);
            else if (strcmp(op, "-") == 0)
                result = subtract(*a, *b);
            else {
                printf("Unknown operator.\n");
                continue;
            }

            strcpy(result.name, lhs);
            result.in_use = 1;
            addvect(result);

            printf("%s = %.2f %.2f %.2f\n", result.name, result.comp[0], result.comp[1], result.comp[2]);
            continue;
        }

        // Scalar multiplication without assignment
        // Ex: a * 2   or   2 * a
        if (sscanf(line, "%15s * %lf", v1, &num)==2 ||sscanf(line, "%lf * %15s", &num, v1) == 2){

            vect *a = findvect(v1);
            if (!a) {
                printf("Vector %s not found.\n", v1);
                continue;
            }

            vect result = scalar_mult(*a, num);
            printf("ans = %.2f %.2f %.2f\n",
                   result.comp[0], result.comp[1], result.comp[2]);
            continue;
        }

        // Addition or subtraction without assignment
        // Ex: a + b   or   b - a
        if (sscanf(line, "%15s %3s %15s", v1, op, v2) == 3) {
            vect *a = findvect(v1);
            vect *b = findvect(v2);

            if (!a || !b) {
                printf("Error: vector not found.\n");
                continue;
            }

            vect result;
            if (strcmp(op, "+") == 0)
                result = add(*a, *b);
            else if (strcmp(op, "-") == 0)
                result = subtract(*a, *b);
            else {
                printf("Unknown operator.\n");
                continue;
            }

            printf("ans = %.2f %.2f %.2f\n",
                   result.comp[0], result.comp[1], result.comp[2]);
            continue;
        }

        // Display vector
        if (sscanf(line, "%15s", v1)==1){
            vect *a = findvect(v1);
            if (!a)
                printf("Vector %s not found.\n", v1);
            else
                printf("%s = %.2f %.2f %.2f\n",a->name, a->comp[0], a->comp[1], a->comp[2]);
            continue;
        }

        // Error catch-all
        printf("Unknown command. Type -h for help screen.\n");
    }
}



int main(int argc, char *argv[]){
    if (argc > 1 && strcmp(argv[1], "-h") == 0){
        print_help();
        return 0;
    }
    clear();
    vector_console();
    return 0;
}
