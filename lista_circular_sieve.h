#include <stdlib.h>
#include <stdio.h>

typedef struct tSieve{
    unsigned *valoresTestados;      /* BUFFER contendo todos os valores já testados */
    unsigned valorSerTestado;       /* valor a ser testado, se for igual a 0 não tem nenhum valor a ser testado */

    unsigned qntdValoresTestados;   /* contador para quantidade de valores testados */
    unsigned posicao;               /* posicao do sieve na lista circular (identifica o sieve) */
} Sieve;

typedef struct tListaCircularSieve
{
    unsigned tamanhoMaximo;
    unsigned tamanhoAtual;
    int cursor;
    Sieve *lista;
} ListaCircularSieve;

ListaCircularSieve *criarLista(int tamanho);
void liberarLista(ListaCircularSieve *lista);

bool adicionarElementoLista(ListaCircularSieve *sieves, unsigned posicao, unsigned elemento);

Sieve proximoDaLista(ListaCircularSieve *sieves, Sieve sieve){
    unsigned posicao = sieve.posicao;
    if(posicao+1 == sieves->tamanhoMaximo)
        posicao = 0;
    else
        posicao--;
    return sieves->lista[posicao];
}

bool listaCheia(ListaCircularSieve *sieves);
void imprimirLista(ListaCircularSieve *sieves);