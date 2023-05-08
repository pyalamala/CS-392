// Credit: Shudong Hao

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: addone() must accept parameters that include a function pointer,
// the number of elements in the array and the size of each element
// it must NOT accept any parameters directly revealing the data type

void add_int(void* a){
    int* elem = (int*) a;
    int* temp = malloc(sizeof(int));
    *(temp) = *(elem) + 1;
    memcpy(a, temp, sizeof(int));
    free(temp);
}

void add_dbl(void* a){
    double* elem = (double*) a;
    double* temp = malloc(sizeof(double));
    *(temp) = *(elem) + 1;
    memcpy(a, temp, sizeof(double));
    free(temp);
}

void add_char(void* a){
    char* elem = (char*) a;
    char* temp = malloc(sizeof(char));
    *(temp) = *(elem) + 1;
    memcpy(a, temp, sizeof(char));
    free(temp);
}



void addone(void* array, void (*comp)(void* elem), size_t len, size_t elem_sz){
    size_t count = 0;
    while (count < len*elem_sz){
        comp((array + count*elem_sz));
        count += 1;
    }
}

int main() {

    int test1[] = {1,2,3,4,5};
    addone((void*) test1, add_int, sizeof(test1), sizeof(int));
    for (size_t i = 0; i < 5; i ++) printf("%d\n", test1[i]);

    double test2[] = {0.3, -0.2, 1.34};
    addone((void*) test2, add_dbl, sizeof(test2), sizeof(double));
    for (size_t i = 0; i < 3; i ++) printf("%lf\n", test2[i]);

    char test3[] = {'3', 'a', 'Z', '?'};
    addone((void*) test3, add_char, sizeof(test3), sizeof(char));
    for (size_t i = 0; i < 4; i ++) printf("%c\n", test3[i]);

    return 0;
}
