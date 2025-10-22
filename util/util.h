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

// Função para ler um número decimal (float) de forma segura
void ler_float(const char *mensagem, float *destino);

//funçao que verifica se o email é valido
int validar_email(const char *email);

//funçao que verifica se o tel tem 11 digitos
int validar_tel(const char *tel);

//funçao que verifica se o cnpj tem 14 digitos
int validar_cnpj(const char *cnpj);

//funçao que verifica se o cnpj tem 13 digitos
int validar_inscr(const char *inscricao_estadual);

//funçao que verifica se o cpf tem 11 digitos
int validar_cpf(const char *cpf);
#endif
