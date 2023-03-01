#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int data0;
    char *data1;
} Interface;

typedef struct {
    int data0;
    char *data1;
    float data2;
} Class0;

Class0* NewClass0(int data0, char *data1, float data2) {
    Class0 *class0 = (Class0*) malloc(sizeof(Class0));
    class0->data0 = data0;
    class0->data1 = data1;
    class0->data2 = data2;
    return class0;
}

typedef struct {
    int data0;
    char *data1;
    Class0 *data2;
} Class1;

Class1* NewClass1(int data0, char *data1, Class0 *data2) {
    Class1 *class1 = (Class1*) malloc(sizeof(Class1));
    class1->data0 = data0;
    class1->data1 = data1;
    class1->data2 = data2;
    return class1;
}


void printClass(Interface *interface) {
    printf("%d %s\n", interface->data0, interface->data1);
}

void main(void) {
    Class0 *class0 = NewClass0(1488, "Bavovna", 3.14);
    Class1 *class1 = NewClass1(666, "YOY", class0);

    printClass((Interface*)class0);
    printClass((Interface*)class1);
}