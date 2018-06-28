#define BLOQUEADO 1
#define DESBLOQUEADO 0

/* Estrutura responsável por armazenar a Resposta que o resultado irá imprimir */
typedef struct tResposta
{
    unsigned dado; /* irá manter o dado que foi testado na rede, diferente de 0 caso tenha um resultado */
    unsigned divisor; /* caso o resultado for um número não primo, essa váriavel será utiliza para guardar a informação de qual elemento foi testado e provado que não é primo*/
    short ePrimo; /* flag que indica se númeor é primo (1), ou não (0) */
    int sieve; /* id da sieve que conseguiu chegar ao resultado */
    short estado; /* estado que a resposta se entra, BLOQUEADO ou DESBLOQUEADO */
} Resposta;

/* Função responsável por criar Resposta */
Resposta *criarResposta();
/* Função responsável por liberar espaço de memória de uma Resposta */
void liberarResposta(Resposta *resposta);

/* Função responsável por bloquear resposta */
void bloquearResposta(Resposta *resposta);
/* Função responsável por desbloquear resposta */
void desbloquearResposta(Resposta *resposta);
/* Função responsável por mudar valor de resposta (quando for um número primo)*/
void setNumeroPrimoResposta(Resposta *resposta, unsigned numero);
/* Função responsável por mudar valor de resposta (quando for um número não primo)*/
void setNumeroNaoPrimoResposta(Resposta *resposta, unsigned numero, unsigned divisor);

/* Função responsável por verificar se Resposta esa bloqueada */
short respostaBloqueada(Resposta *resposta);
/* Função responsável por verificar se Resposa é de um número primo */
short verificarRespostaNumeroPrimo(Resposta *resposta);