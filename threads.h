#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* TADS */
typedef struct tResposta{
    char *mensagem;
    int siege;
} Resposta;


/* CONSTANTES */
#define NUM_READERS 5
#define NUM_READS 5
#define NUM_WRITERS 5
#define NUM_WRITES 5

/* Dadois Compartilhados Globais */
Resposta resposta;
resposta.siege = -1;

pthread_mutex_t gBloqueioMemoriaCompartilhada = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gResultado = PTHREAD_COND_INITIALIZER;
pthread_cond_t gSieges = PTHREAD_COND_INITIALIZER;
pthread_cond_t gBuffer = PTHREAD_COND_INITIALIZER;