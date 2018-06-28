#include "threads.h"

void *sieve(void *param)
{
    unsigned id = *((int *)param); /* identificador da thread sieve (posição no vetor de anel da lista circular)*/
    unsigned idComunicacao = id+1; /* comunicação com thread ao lado (referente a posição no vetor de anel da lista circular)*/
    if(idComunicacao == QNTD_THREADS_SIEVE)
        idComunicacao = 0;
    unsigned valorTestado = 0;
    unsigned divisor = 0;
    short continuar = 1; /* flag que indica se Thread continuará a ser executada */
    // printf("(sieve %d) thread criada - compartilhamento %d\n", id, idComunicacao);
    // fflush(stdout);
    while (continuar)
    {
        // printf("(sieve %d) thread executando\n", id);
        // fflush(stdout);
        /* INICIO SEÇÃO CRÍTICA */
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (verificarSieveEstaDisponivelCalculo(sievies, id) == 0)
            {
                /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
                if(parada == 1){
                    /* Caso a Thread Principal sinalizou que trabalho devem ser parados */
                    continuar = 0; /* atualiza flag para sinalizar que Thread deve ser encerrada */
                    break; /* sai do primeiro loop */
                }
                // printf("(sieve %d) thread verificando possibilidade de cálculo\n", id);
            }
            bloquearSieve(sievies, id);
            valorTestado = sievies->anel[id].valorSerTestado;
            reiniciarValoreSerTestado(sievies, id);
            divisor = buscarDivisorSieve(sievies->anel[id], valorTestado);

            // printf("(sieve %d) thread fazendo cálculo de %d\n", id, valorTestado);
            // fflush(stdout);
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
        
        // printf("(sieve %d) %d / %d\n", id, valorTestado, divisor);

        if(divisor || idComunicacao == 0){
            /* Caso tenha encontrado um divisor, ou não tenha encontrado, mas esteja no final da Lista Circular Sieve */
            pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                while (respostaBloqueada(resposta))
                {
                    /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                    pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                }
                if(divisor == 0 && idComunicacao == 0){
                    /* Caso não tenha encontrado divisor mais esteja no final da Lista Circular Sieve (numero primo) */
                    setNumeroPrimoResposta(resposta, id, valorTestado);
                    propagarDadoNaRede(sievies, valorTestado);

                    // printf("(sieve %d) thread encontrou primo %d \n", id, valorTestado);
                    // fflush(stdout);
                }
                else{
                    setNumeroNaoPrimoResposta(resposta, id, valorTestado, divisor);

                    // printf("(sieve %d) thread encontrou não primo %d \n", id, valorTestado);
                    // fflush(stdout);
                }
                desbloquearSieve(sievies, id);
            pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);

            pthread_cond_signal(&gResultado);
        }
        else{
            /* Caso o dado tem que ser passado para testar na próxima Sieve */
            pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                while (verificarSieveDisponivel(sievies, idComunicacao) == BLOQUEADO)
                {
                    /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                    pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
                }
                setValorSerTestado(sievies, idComunicacao, valorTestado);

                desbloquearSieve(sievies, id);

                // printf("(sieve %d) thread enviando para seu vizinho %d testar o valor %d \n", id, idComunicacao, valorTestado);
                // fflush(stdout);
            pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);

            pthread_cond_broadcast(&gSieves);
        }
        pthread_cond_broadcast(&gSieves);
    }
    return 0;
}

void *resultado(void *param)
{
    short continuar = 1; /* flag que indica se Thread continuará a ser executada */
    while (continuar)
    {
        /* Continua no laço até Thread que a chamou uni-lá */
        /* INICIO SEÇÃO CRÍTICA */
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (resposta->dado == 0)
            {
                /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                if(parada == 1){
                    /* Caso a Thread Principal sinalizou que trabalho devem ser parados */
                    continuar = 0; /* atualiza flag para sinalizar que Thread deve ser encerrada */
                    break; /* sai do primeiro loop */
                }

                // printf("(resultado) verificando se tem dado %d\n", resposta->dado);
                // fflush(stdout);
            }
            bloquearResposta(resposta);
                imprimirResposta(resposta);
            desbloquearResposta(resposta);

            // printf("(resultado) desbloqueando resultado\n");
            // fflush(stdout);
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
        /* FIM SEÇÃO CRÍTICA */
        pthread_cond_signal(&gResultado);
        
        pthread_cond_broadcast(&gSieves); /* envia sinal para sieve 0 */
    }
    return 0;
}