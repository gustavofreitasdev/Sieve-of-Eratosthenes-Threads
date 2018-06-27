#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* TADS */
typedef struct tResposta
{
    char *mensagem;
    int siege;
} Resposta;

/* CONSTANTES */

/* Dadois Compartilhados Globais */
Resposta resposta;
resposta.siege = -1;

pthread_mutex_t gBloqueioMemoriaCompartilhada = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gResultado = PTHREAD_COND_INITIALIZER;
pthread_cond_t gSieges = PTHREAD_COND_INITIALIZER;
pthread_cond_t gBuffer = PTHREAD_COND_INITIALIZER;

void *principal(void *param);
void *siege(void *param);
void *mostrarResultado(void *param);