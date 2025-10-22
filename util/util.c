#include <ctype.h>   // isdigit
#include <string.h>  // strcpy, strcspn
#include <stdio.h>   // printf, fgets
#include <stdlib.h>  // strtol
#include <stddef.h>  // size_t
#include "util.h"    // só 1 vez
#include <errno.h>
#include <float.h>


// ----------------------------------------------------
// Função genérica para ler strings de forma segura
// ----------------------------------------------------
// Eu criei essa função porque o scanf("%s") nao funciona
// bem para textos que têm espaços (por exemplo, "Rua A, 123").
// Além disso, o fgets evita problemas de estouro de buffer.
// Essa função pede uma mensagem, o destino (onde guardar o texto)
// e o tamanho máximo desse destino, para garantir segurança.
// ----------------------------------------------------
void ler_string(const char *mensagem, char *destino, size_t tamanho) {
    char buffer[256]; // buffer temporário maior que o destino
    while (1) {
        printf("%s", mensagem);

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Remove o '\n' do final se existir
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }

            // Copia no destino garantindo que nao ultrapasse o tamanho
            strncpy(destino, buffer, tamanho - 1);
            destino[tamanho - 1] = '\0'; // garante finalização segura

            // Se nao estiver vazio, saída
            if (destino[0] != '\0') return;
            printf("Entrada vazia. Tente novamente.\n");
        }
    }
}


// Função para ler um número inteiro do usuário de forma segura
// Parâmetros:
//   mensagem -> mensagem que será exibida para o usuário (ex: "Digite a idade: ")
//   destino  -> ponteiro para a variável onde o número lido será armazenado
void ler_int(const char *mensagem, int *destino) {
    char buffer[128];        // buffer seguro para uma linha
    char *endptr;
    long valor;

    while (1) {
        // Mostro a mensagem pro usuário (ex.: "Escolha uma opcao: ")
        printf("%s", mensagem);

        // Leio a linha inteira (inclui o '\n' se houver)
        if (!fgets(buffer, sizeof buffer, stdin)) {
            // se houve erro de leitura, reseto e tento de novo
            clearerr(stdin);
            printf("Erro de leitura. Tente novamente.\n");
            continue;
        }

        // Removo possíveis '\r' e '\n' do final (compatibilidade Windows/Unix)
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Preparando para strtol
        errno = 0;
        valor = strtol(buffer, &endptr, 10);

        // 1) Verifico se foi lido ao menos um dígito
        if (endptr == buffer) {
            printf("Entrada invalida (nao e numero). Tente novamente.\n");
            continue;
        }

        // 2) Pulo espaços em branco após o número (aceitar "1  " por ex.)
        while (isspace((unsigned char)*endptr)) endptr++;

        // 3) Se tiver qualquer outro caractere, rejeito (ex: "12abc")
        if (*endptr != '\0') {
            printf("Entrada invalida (caracteres extras). Tente novamente.\n");
            continue;
        }

        // Se cheguei aqui, está tudo certo — guardo no destino e retorno
        *destino = (int)valor;
        return;
    } // while
}

// ======================================
// Função para ler um número decimal (float)
// ======================================
void ler_float(const char *mensagem, float *destino) {
    char buffer[128];       // buffer para armazenar o que o usuário digitou
    char *endptr;           // ponteiro para verificar onde a conversão parou
    float valor;            // valor convertido temporariamente

    while (1) {
        // Mostro a mensagem para o usuário (ex.: "Margem de lucro: ")
        printf("%s", mensagem);

        // Leio a linha inteira digitada pelo usuário
        if (!fgets(buffer, sizeof buffer, stdin)) {
            // Se ocorrer erro de leitura, limpo e tento novamente
            clearerr(stdin);
            printf("Erro de leitura. Tente novamente.\n");
            continue;
        }

        // Removo o '\n' ou '\r' que podem vir no final da string (compatível com Windows e Linux)
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Substituo vírgula por ponto
        for (char *p = buffer; *p; p++) {
            if (*p == ',') *p = '.';
        }

        // Preparo para a conversão usando strtof (string para float)
        errno = 0;
        valor = strtof(buffer, &endptr);

        // 1) Verifico se o usuário digitou algo que é número
        if (endptr == buffer) {
            printf("Entrada invalida (nao é numero). Tente novamente.\n");
            continue;
        }

        // 2) Pulo possíveis espaços após o número (aceitar '9.5   ')
        while (isspace((unsigned char)*endptr)) endptr++;

        // 3) Se ainda sobrou algum caractere diferente de '\0', rejeito (ex: '9.5abc')
        if (*endptr != '\0') {
            printf("Entrada invalida (caracteres extras). Tente novamente.\n");
            continue;
        }

        // 4) Verifico se houve overflow ou underflow (muito grande ou muito pequeno)
        if (errno == ERANGE || valor > FLT_MAX || valor < -FLT_MAX) {
            printf("Número fora do intervalo permitido para float. Tente novamente.\n");
            continue;
        }

        // Se passou por todas as verificações, guardo o valor no destino
        *destino = valor;
        return;
    }
}
int validar_email(const char *email) {
    // strstr() busca uma substring dentro de outra string.
    // Se encontrar "@gmail", ela retorna o endereço da primeira ocorrência.
    // Se não encontrar, retorna NULL.
    if (strstr(email, "@gmail") != NULL)
        return 1; // Encontrou "@gmail" → e-mail é válido
    else
        return 0; // Não encontrou → e-mail é inválido
}

int validar_tel(const char *tel) {
    // strlen() retorna o número de caracteres na string (sem contar o '\0').
    if (strlen(tel) == 11)
        return 1; // telefone tem 11 caracteres → válido
    else
        return 0; // qualquer outro tamanho → inválido
}

int validar_cnpj(const char *cnpj) {
    // strlen() retorna o número de caracteres na string (sem contar o '\0').
    if (strlen(cnpj) == 14)
        return 1; // cnpj tem 11 caracteres → válido
    else
        return 0; // qualquer outro tamanho → inválido
}

int validar_inscr(const char *inscricao_estadual) {
    // strlen() retorna o número de caracteres na string (sem contar o '\0').
    if (strlen(inscricao_estadual) == 13)
        return 1; // inscricao_estadual tem 11 caracteres → válido
    else
        return 0; // qualquer outro tamanho → inválido
}