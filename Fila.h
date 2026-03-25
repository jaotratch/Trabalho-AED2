
#ifndef fila_h
#define fila_h

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int fila[100];
    int inicio;
    int fim;
    int tamanho;
}Fila;

Fila *criafila();
void push(Fila *f, int x);
int pop(Fila *f);
int filaVazia(Fila *f);
int filaCheia(Fila *f);

#endif // fila_h
