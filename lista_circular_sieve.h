#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIEVE 10
#define BLOQUEADO 1
#define DESBLOQUEADO 0

/* Estrutura responsável por armazenar dados do Sieve */
typedef struct tSieve{
    unsigned *valoresTestados;      /* BUFFER contendo todos os valores já testados */
    unsigned valorSerTestado;       /* valor a ser testado, se for igual a 0 não tem nenhum valor a ser testado */

    unsigned qntdValoresTestados;   /* contador para quantidade de valores testados */
    unsigned posicao;               /* posicao do sieve no anel circular (identifica o sieve) */
    short estado;
    unsigned cursor;
} Sieve;
/* Estrutura que tem implementação parecida com uma lista circular */
typedef struct tListaCircularSieve
{
    unsigned tamanhoMaximo;         /* tamanho máximo que a lista pode atingir */
    Sieve *anel;                    /* anel conténdo todas as informações do sieves */
    unsigned cursor;
} ListaCircularSieve;

ListaCircularSieve *criar_Lista(int tamanho);
void liberarLista(ListaCircularSieve *lista);

void bloquearSieve(ListaCircularSieve *sieve, unsigned posicao);
void desbloquearSieve(ListaCircularSieve *sieves, unsigned posicao);
short verificarSieveDisponivel(ListaCircularSieve *sieve, unsigned posicao);
Sieve *getSieveDisponivelCalculo(ListaCircularSieve *sieves);
void setValorSerTestado(ListaCircularSieve *sieves, unsigned posicao, unsigned novoValor);
short adicionarElementoLista(ListaCircularSieve *sieves, unsigned elemento);

void imprimirLista(ListaCircularSieve *sieves);

short verificarSeElementosSaoDivisiveis(Sieve sieve, unsigned elementoComparado);