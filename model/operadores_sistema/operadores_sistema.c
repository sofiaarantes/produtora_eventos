#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operadores_sistema.h"

// Adicionar operador
Operadores* adicionar_operador(Operadores* operador) {
    if (!operador) return NULL;

    // Verifica se já existe um operador com o mesmo nome de usuário
    FILE* fp = fopen("operadores.txt", "r");
    if (fp) {
        Operadores tmp;
        while (fscanf(fp, "%d;%49[^;];%49[^;];%19[^;];\n",
                      &tmp.id, tmp.nome, tmp.usuario, tmp.senha) == 4) {

            // Se encontrar um usuário igual, avisa e cancela o cadastro
            if (strcmp(tmp.usuario, operador->usuario) == 0) {
                printf("\nErro: Já existe um operador com o usuário '%s'.\n", operador->usuario);
                fclose(fp);
                return NULL;
            }
        }
        fclose(fp);
    }

    // Gera novo ID
    int novo_id = 1;
    fp = fopen("operadores.txt", "r");
    if (fp) {
        Operadores tmp;
        while (fscanf(fp, "%d;%49[^;];%49[^;];%19[^;];\n", 
            &tmp.id, tmp.nome, tmp.usuario, tmp.senha) == 4) 
        {
            novo_id = tmp.id + 1; 
        }
        fclose(fp);
    }
    operador->id = novo_id;

    // Salva o novo operador 
    fp = fopen("operadores.txt", "a");
    if (!fp) {
        perror("Erro ao abrir operadores.txt");
        return NULL;
    }

    fprintf(fp, "%d;%s;%s;%s;\n",
            operador->id,
            operador->nome,
            operador->usuario,
            operador->senha);
    fclose(fp);

    printf("\nOperador '%s' cadastrado com sucesso!\n", operador->usuario);
    return operador;
}

// Deletar Operador
int deletar_operador(const int id) {
    FILE* fp = fopen("operadores.txt", "r");
    FILE* temp = fopen("operadores_temp.txt", "w");
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0;
    }

    Operadores f;
    int removido = 0;
    // Lê linha por linha do arquivo original
    while (fscanf(fp, "%d;%49[^;];%49[^;];%19[^;];\n",
                  &f.id, f.nome, f.usuario, f.senha) == 4) {
        // Só regrava se não for o operador que queremos remover
        if (f.id != id) {
            fprintf(temp, "%d;%s;%s;%s;\n",
                f.id, f.nome, f.usuario, f.senha);
        } else {
            removido = 1; // operador encontrado e removido
        }
    }
    
    fclose(fp);
    fclose(temp);
    remove("operadores.txt");
    rename("operadores_temp.txt", "operadores.txt");

    return removido;
}

Operadores* buscar_operador_por_credenciais(const char* usuario, const char* senha) {
    FILE* fp = fopen("operadores.txt", "r");
    if (!fp) return NULL;

    Operadores* encontrado = malloc(sizeof(Operadores));
    while (fscanf(fp, "%d;%49[^;];%49[^;];%19[^;];\n",
              &encontrado->id, encontrado->nome, encontrado->usuario, encontrado->senha) == 4) { 
        if (strcmp(encontrado->usuario, usuario) == 0 &&
            strcmp(encontrado->senha, senha) == 0) {
            fclose(fp);
            return encontrado;
        }
    }

    fclose(fp);
    free(encontrado);
    return NULL;
}
