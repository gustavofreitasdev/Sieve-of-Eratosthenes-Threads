#include "fila.h"


//cria fila e inicializa variaveis
No *criarFila(){
    No *fila;
    fila = (No *)malloc(sizeof(No));
    if(fila){
        fila->prox = NULL;
        fila->quantidade = 0;
        return fila;
    }else{
        return 0;
    }

}

//insere elemento no inicio da fila
void inserirFila(No *fila,int valor){
    No *aux = fila->prox;
    No *elemento = (No *)malloc(sizeof(No));
    elemento->numero = valor;
    if(elemento != NULL){
        fila->prox = elemento;
        elemento->prox = aux;
        fila->quantidade++;
    }
}

//retira e le elemento do fim da fila
int retirarFila(No *fila){
    No *aux = fila->prox;
    int i,valor;
    if(fila->quantidade == 1) {
        valor = aux->numero;
        free(fila->prox);
        fila->prox = NULL;

    }else if(fila->quantidade > 1){
        No *aux2 = aux->prox;
        for(i = 0 ; aux2->prox != NULL ; aux2 = aux2->prox , aux = aux->prox);
        valor = aux2->numero;
        free(aux->prox);

        aux->prox = NULL;
    }
    fila->quantidade--;
    return valor;
}