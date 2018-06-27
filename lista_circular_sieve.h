#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIEVE 10
#define BLOQUEADO 1
#define DESBLOQUEADO 0

/* Estrutura responsável por armazenar dados do Sieve */
typedef struct tSieve
{
    unsigned *valoresTestados; /* BUFFER contendo todos os valores já testados */
    unsigned valorSerTestado;  /* valor a ser testado, se for igual a 0 não tem nenhum valor a ser testado */

    unsigned qntdValoresTestados; /* contador para quantidade de valores testados */
    unsigned posicao;             /* posicao do sieve no anel circular (identifica o sieve) */
    short estado;
    unsigned cursor;
} Sieve;
/* Estrutura que tem implementação parecida com uma lista circular */
typedef struct tListaCircularSieve
{
    unsigned tamanhoMaximo; /* tamanho máximo que a lista pode atingir */
    Sieve *anel;            /* anel conténdo todas as informações do sieves */
    unsigned cursor;
} ListaCircularSieve;

/* Função responsável por criar uma Lista Circular Sieve */
ListaCircularSieve *criarLista(int tamanho);
/* Função responsável por liberar espaço alocado por uma Lista Circular Sieve */
void liberarLista(ListaCircularSieve *lista);
/* Função responsável por bloquear uma sieve, dada a Lista Circular Sieve e a posição do Sieve */
void bloquearSieve(ListaCircularSieve *sieves, unsigned posicao);
/* Função responsável por desbloquear uma sieve, dada a Lista Circular Sieve e a posição do Sieve */
void desbloquearSieve(ListaCircularSieve *sieves, unsigned posicao);
/* Função responsável por mudar valor a ser testado de um Sieve, dada uma Lista Circular Sieve, a posição do Sieve e o novo valor */
void setValorSerTestado(ListaCircularSieve *sieves, unsigned posicao, unsigned novoValor);
/* Função responsável por imprimir uma Lista Circular Sieve */
void imprimirLista(ListaCircularSieve *sieves);

/* Função responsável por verificar se uma Sieve está disponível, da uma Lista Circular Sieve e a posição do Sieve */
short verificarSieveDisponivel(ListaCircularSieve *sieve, unsigned posicao);
/* Função responsável por verificar se uma Sieve está disponível para cálculo, da uma Lista Circular Sieve e a posição do Sieve */
short verificarSieveEstaDisponivelCalculo(ListaCircularSieve *sieves, unsigned posicao);
/* Função responsável por adicionar elementos de forma balanceada na Lista Circular Sieve, dado a Lista Circular e o novo elemento a ser adicionado */
short adicionarElementoLista(ListaCircularSieve *sieves, unsigned elemento);
/* Função responsável por verificar se existe algum elemento divisível (valores guardados em valoresTestados), dado uma Sieve e um elemento a ser comparado */
short verificarSeElementosSaoDivisiveis(Sieve sieve, unsigned elementoComparado);