#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>  // para size_t
#include <stdio.h>   // para printf, fgets

// Função para manter apenas números em uma string
void limpar_digitos(char* str);

// Função para ler strings de forma segura, evitando estouro de buffer
void ler_string(const char *mensagem, char *destino, size_t tamanho);

// Função para ler um número inteiro de forma segura
void ler_int(const char *mensagem, int *destino);
#endif
