#include "sessao.h"

// Variável de sessão
int operador_logado_id = -1; // -1 = ninguém logado

void set_operador_logado(int id) {
    operador_logado_id = id;
}

int get_operador_logado() {
    return operador_logado_id;
}

void limpar_sessao() {
    operador_logado_id = -1;
}
