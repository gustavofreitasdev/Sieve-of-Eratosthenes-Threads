#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lista_circular_sieve.c"

/* Tads */
typedef struct tResposta
{
    unsigned resultado;
    unsigned divisor;
    short ePrimo;
    int sieve;
} Resposta;

/* Constantes */
#define QNTD_THREADS_PRINCIPAL 1
#define QNTD_THREADS_SIEVE 8
#define QNTD_THREADS_RESULTADO 1


/* Dadois Compartilhados Globais */
Resposta resposta;
unsigned int qntdNumerosCalculados;
ListaCircularSieve *sievies;

/* Dados das Threads */
pthread_mutex_t gBloqueioMemoriaCompartilhada = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gResultado = PTHREAD_COND_INITIALIZER;
pthread_cond_t gSieves = PTHREAD_COND_INITIALIZER;
pthread_cond_t gBuffer = PTHREAD_COND_INITIALIZER;

/* Informações sobre as Threads */
void *siege(void *param);
void *resultado(void *param);