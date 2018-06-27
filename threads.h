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

/* Dadois Compartilhados Globais */
Resposta resposta;
resposta.siege = -1;
unsigned int qntdNumerosCalculados = 0;

/* Dados das Threads */
pthread_mutex_t gBloqueioMemoriaCompartilhada = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gResultado = PTHREAD_COND_INITIALIZER;
pthread_cond_t gSieges = PTHREAD_COND_INITIALIZER;
pthread_cond_t gBuffer = PTHREAD_COND_INITIALIZER;

/* Informações sobre as Threads */
void *principal(void *param);
void *siege(void *param);
void *mostrarResultado(void *param);