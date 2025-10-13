#ifndef SESSAO_H
#define SESSAO_H

// Armazena o ID do operador logado atualmente
extern int operador_logado_id;

// Recebe um id e o seta como usuário logado
void set_operador_logado(int id);

// Retorna o id do usuário logado
int get_operador_logado();

// Retorna o valor inicial da variável(-1)
void limpar_sessao();

#endif
