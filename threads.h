#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Tads */
typedef struct tResposta
{
    unsigned resultado;
    unsigned divisor;
    short ePrimo;
    int siege;
} Resposta;

/* Constantes */
#define QNTD_THREADS_PRINCIPAL 1
#define QNTD_THREADS_SIEGE 8
#define QNTD_THREADS_RESULTADO 1


/* Dadois Compartilhados Globais */
Resposta resposta;
resposta.siege = -1;
unsigned int qntdNumerosCalculados = 0;
ListaCircularSieve *sievies;

/* Dados das Threads */
pthread_mutex_t gBloqueioMemoriaCompartilhada = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gResultado = PTHREAD_COND_INITIALIZER;
pthread_cond_t gSieves = PTHREAD_COND_INITIALIZER;
pthread_cond_t gBuffer = PTHREAD_COND_INITIALIZER;

/* Informações sobre as Threads */
void *principal(void *param);
void *siege(void *param);
void *mostrarResultado(void *param);