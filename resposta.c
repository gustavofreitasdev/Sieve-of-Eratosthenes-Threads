#include "resposta.h"

Resposta *criarResposta(void)
{
    Resposta *resposta = (Resposta *)malloc(sizeof(Resposta)); /* alocando espaço de memória para Resposta */
    if (resposta)                                              /* Caso consigar alocar espaço */
        desbloquearResposta(resposta);                         /* inicia a Resposta como desbloqueada */
    return resposta;
}
Resposta *copiarResposta(Resposta *resposta)
{
    Resposta *copiaResposta = criarResposta();
    if (resposta && copiaResposta)
    {
        /* Caso a resposta seja válida e consiga alocar espaço de memória para copia. Copia os dados*/
        copiaResposta->dado = resposta->dado;
        copiaResposta->divisor = resposta->divisor;
        copiaResposta->ePrimo = resposta->ePrimo;
        copiaResposta->sieve = resposta->sieve;
        copiaResposta->estado = resposta->estado;
    }
    return copiaResposta;
}
void liberarResposta(Resposta *resposta)
{
    free(resposta); /* libera espaço alocado para Resposta */
}

void bloquearResposta(Resposta *resposta)
{
    if (resposta && resposta->estado == DESBLOQUEADO)
    {
        /* Caso a Resposta seja válida e estiver desbloqueado */
        resposta->estado = BLOQUEADO;
    }
};
void desbloquearResposta(Resposta *resposta)
{
    if (resposta && resposta->estado == BLOQUEADO)
    {
        /* Caso a Resposta seja valida e estiver bloqueada, inicializa tudo com valores padrão e desbloqueia */
        resposta->dado = 0;
        resposta->divisor = 0;
        resposta->ePrimo = 0;
        resposta->sieve = -1;
        resposta->estado = DESBLOQUEADO;
    }
};
void setNumeroPrimoResposta(Resposta *resposta, unsigned sieve, unsigned numero)
{
    if (resposta && sieve >= 0 && numero > 0)
    {
        /* Caso a Resposta, o id da Sieve e o número passado sejam válidos */
        resposta->ePrimo = 1;    /* define flag que indica que número é primo */
        resposta->dado = numero; /* define o Resultado encontrado */
        resposta->sieve = sieve; /* define a Sieve que encontrou o Resultado */
    }
};
void setNumeroNaoPrimoResposta(Resposta *resposta, unsigned sieve, unsigned numero, unsigned divisor)
{
    if (resposta && sieve >= 0 && numero > 0 && divisor > 0)
    {
        /* Caso a Resposta, o id da Sieve, número passado e o divisor sejam válidos */
        resposta->ePrimo = 0;        /* define flag que indica que número não é primo */
        resposta->dado = numero;     /* define o Resultado encontrado */
        resposta->divisor = divisor; /* define o Divisor que foi testado e encontrado o Resultado que não é primo */
        resposta->sieve = sieve;     /* define a Sieve que encontrou o Resultado */
    }
};
void imprimirResposta(Resposta *resposta)
{
    if (resposta && resposta->dado != 0)
    {
        /* Caso Resposta for válida e tenha algum dado para ser impresso */
        if (verificarRespostaNumeroPrimo(resposta))
        {
            /* Caso o número for primo */
            printf("%d is prime (stored in sieve %d)\n", resposta->dado, resposta->sieve); /* caso o número for primo */
        }
        else
        {
            /* Caso o número não for primo */
            printf("%d divided by %d at sieve %d\n", resposta->dado, resposta->divisor, resposta->sieve); /* caso o número não for primo */
        }
        fflush(stdout);
    }
}

short respostaBloqueada(Resposta *resposta)
{
    short bloqueada = 0;

    return bloqueada;
}
short verificarRespostaNumeroPrimo(Resposta *resposta)
{
    short ePrimo = 0;              /* flag que indica se número é primo (1), inicilizada como não primo(0) */
    if (resposta)                  /* caso a resposta for válida */
        ePrimo = resposta->ePrimo; /* captura flag da resposta */
    return ePrimo;
};