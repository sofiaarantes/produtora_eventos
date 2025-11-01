#ifndef OPERADORES_SISTEMA_H
#define OPERADORES_SISTEMA_H

#include "../config_armazenamento/config_armazenamento.h"

typedef struct {
    int id;
    char nome[50];
    char usuario[50];
    char senha[20];
} Operadores;

Operadores* adicionar_operador(Operadores* operador);
int deletar_operador(const int id);
Operadores* buscar_operador_por_credenciais(const char* usuario, const char* senha);

#endif
