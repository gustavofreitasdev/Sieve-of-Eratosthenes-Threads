#include "resposta.h"


Resposta *criarResposta(){
    Resposta *resposta = (Resposta *)malloc(sizeof(Resposta)); /* alocando espaço de memória para Resposta */
    if(resposta) /* Caso consigar alocar espaço */
        desbloquearResposta(resposta); /* inicia a Resposta como desbloqueada */
    return resposta;
}
void liberarResposta(Resposta *resposta){
    free(resposta); /* libera espaço alocado para Resposta */
}
void bloquearResposta(Resposta *resposta){
    if(resposta && resposta->estado == DESBLOQUEADO){
        /* Caso a Resposta seja válida e estiver desbloqueado */
        resposta->estado = BLOQUEADO;
    }
};
void desbloquearResposta(Resposta *resposta){
    if(resposta && resposta->estado == BLOQUEADO){
        /* Caso a Resposta seja valida e estiver bloqueada, inicializa tudo com valores padrão e desbloqueia */
        resposta->dado = 0;
        resposta->divisor = 0;
        resposta->ePrimo = 0;
        resposta->sieve = -1;
        resposta->estado = DESBLOQUEADO;
    }
};
void setNumeroPrimoResposta(Resposta *resposta, unsigned sieve, unsigned numero){
    if(resposta && sieve >= 0 && numero > 0){
        /* Caso a Resposta, o id da Sieve e o número passado sejam válidos */
        resposta->ePrimo = 1; /* define flag que indica que número é primo */
        resposta->dado = numero; /* define o Resultado encontrado */
        resposta->sieve = sieve; /* define a Sieve que encontrou o Resultado */
    }   
};
void setNumeroNaoPrimoResposta(Resposta *resposta, unsigned sieve, unsigned numero, unsigned divisor){
    if(resposta && sieve >= 0 && numero > 0 && divisor > 0){
        /* Caso a Resposta, o id da Sieve, número passado e o divisor sejam válidos */
        resposta->ePrimo = 0; /* define flag que indica que número não é primo */
        resposta->dado = numero; /* define o Resultado encontrado */
        resposta->divisor = divisor; /* define o Divisor que foi testado e encontrado o Resultado que não é primo */
        resposta->sieve = sieve; /* define a Sieve que encontrou o Resultado */
    }   
};

short verificarRespostaNumeroPrimo(Resposta *resposta){
    short ePrimo = 0; /* flag que indica se número é primo (1), inicilizada como não primo(0) */
    if(resposta) /* caso a resposta for válida */
        ePrimo = resposta->ePrimo; /* captura flag da resposta */
    return ePrimo;
};