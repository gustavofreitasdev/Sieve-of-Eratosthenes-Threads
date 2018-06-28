#include "fila.c"
#include "threads.c"

int main(int argc, char argv[])
{
    /* Inicializadores */
    resposta = criarResposta();
    sievies = criarLista(QNTD_THREADS_SIEVE);

    /* Variáveis */
    unsigned n = 10;
    unsigned m = 2;

    /* Threads */
    pthread_t tSieves[QNTD_THREADS_SIEVE];
    pthread_t tResultado;

    /* Buffers */
    No *buffer = criarFila();
    unsigned valoresTestar[n]; /* vetor que contém todos os valores a ser testados */
    int aux = 2;
    for(int cursor=0; cursor<n; cursor++){
        /* Loop para preencher que os valores a seres testados */
        valoresTestar[cursor] = aux;
        aux++;
    }
    unsigned auxSieves[QNTD_THREADS_SIEVE]; /* buffer auxiliar para passagem de argumentos para threads sieve */
    for (int aux2 = 0; aux2 < QNTD_THREADS_SIEVE; aux2++)
        auxSieves[aux2] = aux2;
    
    /* Flags e Auxiliares */
    short continuar = 1; /* flag que indica se todos os valores já foram testados */
    unsigned qntdValoresEnviadoRede = 0; /* contador com quantidade de valores já enviados para rede */
    unsigned cursorBuffer = 0; /* cursor para percorrer o buffer */

    pthread_create(&tResultado, NULL, resultado, NULL); /* cria Thread de Resultado */
    /* Tarefas */
    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        /* Loop para criar todas as Threads de Sieve */
        pthread_create(&tSieves[i], NULL, sieve, &auxSieves[i]);
    }

    /* Tarefas */
    while(continuar){
        if(buffer->quantidade < m && cursorBuffer < n){
            /* Caso o buffer tiver espaço e ainda tiver valores a testar */
            inserirFila(buffer, valoresTestar[cursorBuffer]);
            cursorBuffer++;
            
            printf("(principal) jogando dados no buffer\n");
            fflush(stdout);
        }
        if(buffer->quantidade > 0){
            pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                while (verificarSieveDisponivel(sievies, 0) ==  BLOQUEADO)
                {
                    /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                    pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                }

                propagarDadoNaRede(sievies, retirarFila(buffer));
                qntdValoresEnviadoRede++;

                printf("(principal) propagando dados na rede\n");
                fflush(stdout);
            pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
            pthread_cond_broadcast(&gSieves);
        }
        
        pthread_cond_signal(&gResultado);
    }

    /* Uniões */
    pthread_join(tResultado, NULL);
    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        pthread_join(tSieves[i], NULL);
    }

    return 0;
}