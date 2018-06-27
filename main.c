#include "fila.c"
#include "threads.c"

int main(int argc, char argv[])
{
    /* Inicializadores */
    resposta.resultado = 0;
    resposta.estado = DESBLOQUEADO;
    sievies = criarLista(QNTD_THREADS_SIEVE);

    /* Variáveis */
    unsigned n = 10;
    unsigned m = 2;

    /* Threads */
    pthread_t tSieves[QNTD_THREADS_SIEVE];
    pthread_t tResultado;

    /* Buffer */
    No *buffer = criarFila();
    unsigned valoresTestar[n]; /* vetor que contém todos os valores a ser testados */
    int aux = 2;
    for(int cursor=0; cursor<n; cursor++){
        /* Loop para preencher que os valores a seres testados */
        valoresTestar[cursor] = aux;
        aux++;
    }
    aux = 0; /* reinicilizar váriável para poder ser usada em outras partes do código */
    unsigned auxSieves[QNTD_THREADS_SIEVE]; /* buffer auxiliar para passagem de argumentos para threads sieve */
    for (aux = 0; aux < QNTD_THREADS_SIEVE; aux++)
        auxSieves[aux] = aux;
    
    /* Flags e Auxiliares */
    short continuar = 1; /* flag que indica se todos os valores já foram testados */
    unsigned qntdValoresEnviadoRede = 0; /* contador com quantidade de valores já enviados para rede */
    unsigned cursorBuffer = 0; /* cursor para percorrer o buffer */

    /* Tarefas */
    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        /* Loop para criar todas as Threads de Sieve */
        pthread_create(&tSieves[i], NULL, sieve, &auxSieves[i]);
    }
    pthread_create(&tResultado, NULL, resultado, NULL); /* cria Thread de Resultado */

    while(continuar){
        /* Enquanto todos os valores não forem enviados e testados na rede */
        if(buffer->quantidade < m){
            /* Se buffer tem espaço */
            inserirFila(buffer, valoresTestar[cursorBuffer]); /* inseri novo elemento */
            cursorBuffer++; /* atualiza cursor de buffer */
        }
        if(buffer->quantidade > 0){
            /* Se tiver algum número a ser testado no buffer */
            /* Seção Crítica */
            pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                while (verificarSieveDisponivel(sievies, 0) == BLOQUEADO)
                {
                    /* Continua no laço e fica o sieve 0 esteje desbloqueado */
                    pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
                }
                bloquearSieve(sievies, 0);
                aux = retirarFila(buffer); /* retira o número a ser testado */
                cursorBuffer--; /* atualiza cursor do buffer */
                sievies->anel[0].valorSerTestado = aux;
                desbloquearSieve(sievies, 0);
            pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
            /* Fim Seção Crítica */
            pthread_cond_signal(&gSieves); /* envia sinal para sieve 0 */
        }
    }

    /* Uniões */
    pthread_join(tResultado, NULL);
    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        pthread_join(tSieves[i], NULL);
    }

    return 0;
}