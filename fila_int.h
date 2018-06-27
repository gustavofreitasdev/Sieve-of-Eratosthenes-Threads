#include <stdlib.h>
#include <stdio.h>
//estrutura do no da fila
typedef struct tNo{
    int numero;
    struct tNo *prox;
    int quantidade;
}No;

