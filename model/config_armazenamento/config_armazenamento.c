#include "config_armazenamento.h"
#include "equipe_interna/equipe_interna.h"

// Variável global estática (acessada só via funções)
static TipoArmazenamento armazenamento = MEMORIA;

// Seta qual tipo de armazenamento o sistema usa
void set_armazenamento(TipoArmazenamento tipo) {
    armazenamento = tipo;
}

// Retorna qual tipo de armazenamento o sistema usa
TipoArmazenamento get_armazenamento() {
    return armazenamento;
}
