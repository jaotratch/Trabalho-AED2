#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

typedef struct nodo{
    int vertice;
    struct nodo* proximo;
} nodo;

void liberaListaAdj(nodo* lista){
    nodo* atual = lista;
    while(atual != NULL){
        nodo* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

int min(int a, int b){
    if (a < b){
        return a;
    } else{
        return b;
    }
}

// dfs_ponte_articulacao(): Função para encontrar pontes e articulações
// Uma aresta (u, v) é ponte se nem o filho (v) e nem nenhum dos seus descendentes possui uma aresta de retorno para o pai (u) e seus ancestrais
// Um vértice (u) é articulação se existe um filho (v) tal que nenhum dos seus descendentes possui uma aresta de retorno para o pai (u) e seus ancestrais
// disc[u]: Tempo de descoberta do vértice (u)
// low[v]: Menor tempo de descoberta do ancestral mais antigo que (v) ou qualquer um de seus descendentes pode alcançar
void dfs_ponte_articulacao(int u, int* tempo, int* low, int* disc, int* pai, nodo** listaAdj, int* ponte, int* articulacao){
    int filhos = 0;
    disc[u] = low[u] = ++(*tempo);

    nodo* atual = listaAdj[u];
    while (atual != NULL){
        int v = atual->vertice; // (v) é o filho de (u)

        // Caso 1: Vértice (v) não foi visitado
        if (disc[v] == -1){
            filhos++;
            pai[v] = u;

            dfs_ponte_articulacao(v, tempo, low, disc, pai, listaAdj, ponte, articulacao); // Explora todos os descendentes de (v)

            low[u] = min(low[u], low[v]); // Atualiza low[u] porque (v) sabe o ancestral mais antigo que seus descendentes podem alcançar

            // Se o menor tempo de descoberta do ancestral mais antigo que (v) ou qualquer um de seus descendentes é maior que o tempo de descoberta de (u), então a aresta (u, v) é uma ponte
            if(low[v] > disc[u]){
                *ponte = 1;
                if (u < v){
                    printf("{%d, %d}\n", u, v);
                } else{
                    printf("{%d, %d}\n", v, u);
                }
            }

            // Verifica se (u) é uma articulação
            if(pai[u] != -1 && low[v] >= disc[u]){
                articulacao[u] = 1; // (u) é uma articulação não raiz
            }

            if(pai[u] == -1 && filhos > 1){
                articulacao[u] = 1; // (u) é uma articulação raiz
            }
        }
        // Caso 2: Vértice (v) visitado e não é o pai de (u) é uma aresta de retorno
        else if (v != pai[u]){
            low[u] = min(low[u], disc[v]); // Atualiza low[u] porque (v) é um ancestral de (u)
        }

        atual = atual->proximo;
    }
}

void imprime_bsf(int N, int* pai){

    if(pai[N] == -1 && N == 1){
        printf("%d", N);
        return;
    } else if (pai[N]== -1 && N != 1){
        printf("nenhum");
        return;
    }

    imprime_bsf(pai[N], pai);
    printf(",%d", N);
}

void bsf_minimo(int N, int* pai, int* dist, int* fila, nodo** listaAdj, Fila* f){
    int x = 1;
    push(f, x); // enfilera o primeiro vertice
    fila[x] = 1; // marca como visitado

    while(!filaVazia(f)){
        int v = pop(f); //desenfilera o primeiro da fila

        for(nodo* atual = listaAdj[v]; atual != NULL; atual = atual->proximo){
            int w = atual->vertice;
            if(fila[w] == 1)continue;
            push(f, w);
            fila[w] = 1; // marca como visitado
            dist[w] = dist[v] + 1; // atualiza a distancia do verticie principal
            pai[w] = v;
        }

    }
}

int main(){
    Fila *f = criafila();
    int N, M;
    scanf("%d %d", &N, &M);

    nodo** listaAdj = (nodo**)malloc((N+1) * sizeof(nodo*));

    for(int i = 0; i <= N; i++){
        listaAdj[i] = NULL;
    }

    for(int i = 0; i < M; i++){
        int u, v;
        scanf("%d %d", &u, &v);
        // Para um grafo não direcionado
        // Adiciona a aresta u->v
        nodo* novoNodo1 = (nodo*)malloc(sizeof(nodo));
        novoNodo1->vertice = v;
        novoNodo1->proximo = listaAdj[u];
        listaAdj[u] = novoNodo1;

        // Adiciona a aresta v->u
        nodo* novoNodo2 = (nodo*)malloc(sizeof(nodo));
        novoNodo2->vertice = u;
        novoNodo2->proximo = listaAdj[v];
        listaAdj[v] = novoNodo2;
    }

    int* disc = (int*)malloc((N + 1) * sizeof(int)); 
    int* low = (int*)malloc((N + 1) * sizeof(int)); 
    int* pai = (int*)malloc((N + 1) * sizeof(int));
    int* dist = (int*)malloc((N + 1) * sizeof(int)); // array do tamanho dos caminhos
    int* fila = (int*)malloc((N + 1) * sizeof(int)); // array com os vertices visitados
    int* articulacao = (int*)malloc((N + 1) * sizeof(int));

    int tempo = 0;

    int ponte_encontrada = 0;

    for(int i = 0; i <= N; i++){
        articulacao[i] = 0;
        disc[i] = -1;
        low[i] = -1;
        pai[i] = -1;
    }

    printf("Pontes:\n");

    for(int i = 1; i <= N; i++){
        if(disc[i] == -1){
            dfs_ponte_articulacao(i, &tempo, low, disc, pai, listaAdj, &ponte_encontrada, articulacao);
        }
    }

    if(!ponte_encontrada){
        printf("nenhuma.\n");
    }

    int articulacao_encontrada = 0;

    printf("\nArticulacoes:\n");
    for(int i = 1; i <= N; i++){
        if(articulacao[i]){
            articulacao_encontrada = 1;
            printf("%d\n", i);
        }
    }

    if(!articulacao_encontrada){
        printf("nenhuma.\n");
    }

    for(int i = 0; i <= N; i++){
        pai[i] = -1;
        dist[i] = 0;
        fila[i] = 0;
    }

    bsf_minimo(N, pai, dist, fila, listaAdj, f);
    printf("\nCaminho minimo de 1 a %d:\n", N);
    imprime_bsf(N, pai);
    printf("\nNumero de passos: %d", dist[N]);
    
    free(disc);
    free(low);
    free(pai);
    free(dist);
    free(fila);
    free(f);
    free(articulacao);

    for(int i = 0;i <= N; i++){
        liberaListaAdj(listaAdj[i]);
    }
    free(listaAdj);

    return 0;
}
