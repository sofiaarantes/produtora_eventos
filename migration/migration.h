#ifndef MIGRACAO_H
#define MIGRACAO_H

#include "../../model/config_armazenamento/config_armazenamento.h"

// Função que migra todo o sistema do tipo origem para o tipo destino.
// Retorna 1 em sucesso, 0 em falha.
int migrar_todo_sistema(TipoArmazenamento origem, TipoArmazenamento destino);

#endif