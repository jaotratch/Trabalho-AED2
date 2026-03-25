#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

Fila *criafila(){
    Fila *f = (Fila*)malloc(sizeof(Fila));
    f->tamanho = 0;
    f->inicio = 0;
    f->fim = -1;
    return f;
}
int filaVazia(Fila *f){
    if(f->tamanho == 0)return 1;
    return 0;
}
int filaCheia(Fila *f)
{
    if(f->tamanho == 100)return 1;
    return 0;
}
void push(Fila *f, int x)
{
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % 100;
    f->fila[f->fim] = x;
    f->tamanho++;
}
int pop(Fila *f)
{
    if (filaVazia(f)) return 0;
    int x = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % 100;
    f->tamanho--;
    return x;
}
