#include <ctype.h>   // isdigit
#include <string.h>  // strcpy, strcspn
#include <stdio.h>   // printf, fgets
#include <stdlib.h>  // strtol
#include <stddef.h>  // size_t

#include "util.h"    // só 1 vez


// ----------------------------------------------------
// Função genérica para ler strings de forma segura
// ----------------------------------------------------
// Eu criei essa função porque o scanf("%s") não funciona
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

            // Copia no destino garantindo que não ultrapasse o tamanho
            strncpy(destino, buffer, tamanho - 1);
            destino[tamanho - 1] = '\0'; // garante finalização segura

            // Se não estiver vazio, saída
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
            printf("Entrada inválida (não é número). Tente novamente.\n");
            continue;
        }

        // 2) Pulo espaços em branco após o número (aceitar "1  " por ex.)
        while (isspace((unsigned char)*endptr)) endptr++;

        // 3) Se tiver qualquer outro caractere, rejeito (ex: "12abc")
        if (*endptr != '\0') {
            printf("Entrada inválida (caracteres extras). Tente novamente.\n");
            continue;
        }

        // 4) Verifico overflow/underflow do strtol
        if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            printf("Número fora do intervalo. Tente novamente.\n");
            continue;
        }

        // Se cheguei aqui, está tudo certo — guardo no destino e retorno
        *destino = (int)valor;
        return;
    } // while
}
