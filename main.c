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

static vect vectors[MAX_VECS]; //storage

int addvect(vect v) {
    for (int i = 0; i < MAX_VECS; i++) {
        if (vectors[i].in_use && strcmp(vectors[i].name, v.name) == 0) {
            vectors[i] = v;
            return 0;
        }
    }
    
    for (int i = 0; i < MAX_VECS; i++) {
        if (!vectors[i].in_use) {
            vectors[i] = v;
            vectors[i].in_use = 1;
            return 0;
        }
    }
    printf("Error: memory full, cannot add vector.\n");
    return -1;
}

vect *findvect(char *name) {
    for (int i = 0; i < MAX_VECS; i++) {
        if (vectors[i].in_use && strcmp(vectors[i].name, name) == 0) {
            return &vectors[i];
        }
    }
    return NULL;
}

void clear() {
    for (int i = 0; i < MAX_VECS; i++) {
        vectors[i].in_use = 0;
    }
    printf("All vectors cleared.\n");
}

void listall() {
    for (int i = 0; i < MAX_VECS; i++) {
        if (vectors[i].in_use) {
            printf("%s = %.2f %.2f %.2f\n", vectors[i].name,
                   vectors[i].comp[0], vectors[i].comp[1], vectors[i].comp[2]);
        }
    }
}


void print_help() {
    printf("Usage:\n");
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

void repl() {
    char line[128];
    while (1) {
        printf("minimat> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = 0; // trim newline

        if (strcmp(line, "quit") == 0) break;
        if (strcmp(line, "clear") == 0) { clear(); continue; }
        if (strcmp(line, "list") == 0) { listall(); continue; }
        if (strcmp(line, "-h") == 0) { print_help(); continue; }
        if (strlen(line) == 0) continue;

        // ---- assignment form ----
        char lhs[16], rhs[100];
        if (sscanf(line, "%15s = %[^\n]", lhs, rhs) == 2) {
            double x, y, z;
            int count = sscanf(rhs, "%lf %lf %lf", &x, &y, &z);
            if (count == 2) z = 0.0;
            if (count >= 2) {
                vect v = { "", {x,y,z}, 1 };
                strcpy(v.name, lhs);
                addvect(v);
                printf("%s = %.2f %.2f %.2f\n", v.name, v.comp[0], v.comp[1], v.comp[2]);
            } else {
                printf("Invalid assignment.\n");
            }
            continue;
        }

        // ---- binary operations ----
        char v1[16], v2[16], op[4], lhs2[16];
        double num;
        // result = var1 + var2
        if (sscanf(line, "%15s = %15s %3s %15s", lhs2, v1, op, v2) == 4) {
            vect *a = findvect(v1), *b = findvect(v2);
            if (!a || !b) { printf("Error: vector not found.\n"); continue; }
            vect r;
            if (strcmp(op, "+") == 0) r = add(*a, *b);
            else if (strcmp(op, "-") == 0) r = subtract(*a, *b);
            else { printf("Unknown operator.\n"); continue; }
            strcpy(r.name, lhs2);
            r.in_use = 1;
            addvect(r);
            printf("%s = %.2f %.2f %.2f\n", r.name, r.comp[0], r.comp[1], r.comp[2]);
            continue;
        }

        // var1 + var2
        if (sscanf(line, "%15s %3s %15s", v1, op, v2) == 3) {
            vect *a = findvect(v1), *b = findvect(v2);
            if (!a || !b) { printf("Error: vector not found.\n"); continue; }
            if (strcmp(op, "+") == 0) {
                vect r = add(*a, *b);
                printf("ans = %.2f %.2f %.2f\n", r.comp[0], r.comp[1], r.comp[2]);
            } else if (strcmp(op, "-") == 0) {
                vect r = subtract(*a, *b);
                printf("ans = %.2f %.2f %.2f\n", r.comp[0], r.comp[1], r.comp[2]);
            } else if (strcmp(op, "*") == 0) {
                printf("Error: need scalar multiply with number.\n");
            }
            continue;
        }

        // scalar mult: var * num
        if (sscanf(line, "%15s * %lf", v1, &num) == 2) {
            vect *a = findvect(v1);
            if (!a) { printf("Error: vector not found.\n"); continue; }
            vect r = scalar_mult(*a, num);
            printf("ans = %.2f %.2f %.2f\n", r.comp[0], r.comp[1], r.comp[2]);
            continue;
        }

        // scalar mult: num * var
        if (sscanf(line, "%lf * %15s", &num, v1) == 2) {
            vect *a = findvect(v1);
            if (!a) { printf("Error: vector not found.\n"); continue; }
            vect r = scalar_mult(*a, num);
            printf("ans = %.2f %.2f %.2f\n", r.comp[0], r.comp[1], r.comp[2]);
            continue;
        }

        // display var
        if (sscanf(line, "%15s", v1) == 1) {
            vect *a = findvect(v1);
            if (!a) printf("Vector %s not found.\n", v1);
            else printf("%s = %.2f %.2f %.2f\n", a->name, a->comp[0], a->comp[1], a->comp[2]);
            continue;
        }

        printf("Unknown command. Type -h for help.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }
    clear(); 
    repl();
    return 0;
}
