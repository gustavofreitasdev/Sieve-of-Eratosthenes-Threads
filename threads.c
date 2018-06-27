#include "threads.h"

void *sieve(void *param)
{
    unsigned id = *((int *)param); /* identificador da thread sieve (posição no vetor de anel da lista circular)*/
    unsigned idComunicacao = id+1; /* comunicação com thread ao lado (referente a posição no vetor de anel da lista circular)*/
    unsigned numeroTestar; /* variável para guardar valor a ser calculado */
    short testePrimo; /* variável para guardar valor de verificação se o número é primo */
    Resposta resultado; /* variável para guardar resposta (caso o número seje primo) */
    Sieve sieve; /* variável auxiliar, utilizada para fazer verificações */
    while (1)
    {
        /* Permasse no loop até o processo que o chamou o unir */

        /* Seção Crítica */
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (verificarSieveEstaDisponivelCalculo(sievies, id) == 0)
            {
                /* Continua no loop até sieve está pronta para fazer o cálculo de número primo */
                pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
            }
            /* Quando a sieve está pronta para fazer o cálculo do número primo... */            
            bloquearSieve(sievies, id);
            sieve = sievies->anel[id];
            numeroTestar = sieve.valorSerTestado;
            testePrimo = verificarSeElementosSaoDivisiveis(sieve, numeroTestar);
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
        /* Fim Seção Crítica */
        if(testePrimo){
            /* Se encontrou um divisor (número não é primo) */
            /* Seção Crítica */
            pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                while (resposta.estado == BLOQUEADO)
                {
                    /* Continua no laço e fica na espera até o resposta seja desbloqueada */
                    pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                }
                /* Quando a resposta está desbloqueada... */
                resposta.resultado = numeroTestar; /**/
                resposta.ePrimo = 0;
                resposta.divisor = testePrimo;
                resposta.sieve = id;
            pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
            /* Fim Seção Crítica */
            pthread_cond_signal(&gResultado); /* envia sinal para resultado */
        }
        else{
            /* Se não encontro um divisor */
            if(idComunicacao == QNTD_THREADS_SIEVE){
                /* Se já testou em toda a rede */
                /* Seção Crítica */
                pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                    while (resposta.estado == BLOQUEADO)
                    {
                        /* Continua no laço e fica na espera até o resposta seja desbloqueada */
                        pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                    }
                    /* Quando a resposta está desbloqueada... */
                    resposta.resultado = numeroTestar;
                    resposta.ePrimo = 1;
                    resposta.sieve = id;
                pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
                /* Fim Seção Crítica */
                pthread_cond_signal(&gResultado); /* envia sinal para resultado */
            }
            else{
                /* Seção Crítica */

                pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                    while (verificarSieveDisponivel(sievies, idComunicacao) == BLOQUEADO)
                    {
                        pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
                    }
                    if(sievies->anel[idComunicacao].qntdValoresTestados == 0){
                        while (resposta.estado == BLOQUEADO)
                        {
                            /* Continua no laço e fica na espera até o resposta seja desbloqueada */
                            pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                        }
                        /* Quando a resposta está desbloqueada... */
                        resposta.resultado = numeroTestar;
                        resposta.ePrimo = 1;
                        resposta.sieve = id;
                    }
                    else{
                        sievies->anel[idComunicacao].valorSerTestado = numeroTestar;
                    }                    
                pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
                /* Fim Seção Crítica */

                pthread_cond_signal(&gSieves);
            }
        }

        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            sievies->anel[id].valorSerTestado = 0;
            desbloquearSieve(sievies, id);
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
    }

    return 0;
}

void *resultado(void *param)
{
    Resposta resultado;
    while (1)
    {
        /* Continua no laço até Thread que a chamou uni-lá */
        
        /* INICIO SEÇÃO CRÍTICA */
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (resposta.estado == BLOQUEADO || resposta.resultado == 0)
            {
                /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
            }
            qntdNumerosCalculados++; /* atualiza quantos números já foram calculados */
            resultado = resposta;    /* cria uma váriavel para imprimir o resultado, para liberar a zona crítiza o mais rápido possível */
            if(resultado.ePrimo)
                adicionarElementoLista(sievies, resultado.resultado);
            
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
        /* FIM SEÇÃO CRÍTICA */

        if (resultado.ePrimo == 1)
        {
            printf("%d is prime (stored in sieve %d)\n", resultado.resultado, resultado.sieve); /* caso o número for primo */
        }
        else
        {
            printf("%d divided by %d at sieve %d\n", resultado.resultado, resultado.divisor, resultado.sieve); /* caso o número não for primo */
        }
        fflush(stdout);

        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            resposta.resultado = 0;
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);

        pthread_cond_signal(&gResultado);
    }
    return 0;
}