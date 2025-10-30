/* main.c 
 * Vector Calculator improved implementation
 * Using memory allocation and File I/O
 * Author: Jaffar Mahkamov
 * Date: 10/14/25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

static Node *head = NULL;

// Add or update a vector in the array
int addvect(vect v){
    Node *current = head;
    while(current){
        if (strcmp(current->data.name, v.name) == 0){
            current->data = v; //update existing vector
            return 0;
        }
        current = current->next;
    }
    
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(!new_node){
        printf("Memory allocation failed\n");
        return -1;
    }
    new_node->data = v;
    new_node->data.in_use = 1;
    new_node->next = head;
    head = new_node;
    return 0;
}

// Load vectors from a CSV file
void load_file(const char *filename){
    FILE *fp= fopen(filename,"r");
    if (!fp){
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }

    char line[128];
    int line_num = 0;
    while (fgets(line, sizeof(line), fp)){
        line_num++;

        // Skip empty lines
        if (strlen(line) <= 1) continue;

        // Remove newline characters
        line[strcspn(line, "\r\n")] = 0;

        char name[16];
        double x, y, z;

        if (sscanf(line,"%15[^,],%lf,%lf,%lf",name, &x, &y, &z)== 4){
            vect v;
            strcpy(v.name, name);
            v.comp[0] =x;
            v.comp[1] =y;
            v.comp[2] =z;
            v.in_use =1;
            addvect(v);
        } else {
            printf("Warning: Invalid format on line %d, skipped.\n", line_num);
        }
    }

    fclose(fp);
    printf("File '%s' loaded successfully.\n", filename);
}

// Save vectors to a CSV file
void save_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not open file '%s' for writing\n", filename);
        return;
    }

    Node *current = head;
    while (current) {
        if (current->data.in_use) {
            fprintf(fp, "%s,%.4f,%.4f,%.4f\n",
                    current->data.name,
                    current->data.comp[0],
                    current->data.comp[1],
                    current->data.comp[2]);
        }
        current = current->next;
    }

    fclose(fp);
    printf("Vectors saved to '%s'\n", filename);
}


// Find a vector by name
vect *findvect(char *name) {
    Node *current = head;
    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0){
            return &current->data;
        }
        current = current->next;
    }
    return NULL;
}

// Clear all vectors 
void clear(){
    Node *current = head;
    while(current){
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
}

// List all vectors
void listall() {
    Node *current = head;
    while(current){
        if(current->data.in_use){
            printf("%s = %.2f %.2f %.2f\n",current->data.name,current->data.comp[0],current->data.comp[1],current->data.comp[2]);
        }
        current = current->next;
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
    printf("  load <file>   loads vectors from file\n");
    printf("  save <file>   saves vectors to file\n");
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

        //Handle load file
        if(strncmp(line,"load ",5)==0){
            char filename[64];
            if (sscanf(line + 5, "%63s", filename) == 1){
                load_file(filename);
            } else {
                printf("Usage: load <filename>\n");
            }
            continue;
        }

        //Save file using entered filename
        if(strncmp(line,"save ",5)==0){
            char filename[64];
            if (sscanf(line + 5,"%63s",filename) ==1){
                save_file(filename);
            } else {
                printf("Usage: save <filename>\n");
            }
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
        if (sscanf(line,"%15s", v1)==1){
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
    atexit(clear);
    return 0;
}
