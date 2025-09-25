#include <stdlib.h>
#include "../model/equipe_interna/equipe_interna.h"
#include "../view/equipe_interna/equipe_interna_view.h"
#include "../view/main/main_view.h"
#include "../equipe_interna/equipe_interna_controller.h"


void gerenciar_funcionario() {
    EquipeInterna* equipeInterna = NULL;
    int opcao;
    TipoArmazenamento tipo = get_armazenamento(); // Obtém o tipo de armazenamento configurado

    do {
        opcao = exibir_menu_equipe_interna();
        switch (opcao) {
            case 1: {
                // Agora usamos a função com retorno
                EquipeInterna add = ler_dados_funcionario();
                equipeInterna = adicionar_funcionario_eqIn(add, tipo);
                exibir_mensagem("Funcionário criado com sucesso!");
                break;
            }
            case 2: {
                if (!equipeInterna) {
                    exibir_mensagem("Nenhum funcionário cadastrado!");
                } else {
                    char nome[50];
                    int cpf;
                    char funcao[100];
                    float valor_diaria;
                    ler_dados_atualizacao_funcionario(nome, &cpf, funcao, &valor_diaria);
                    atualizar_funcionario_eqIn(equipeInterna, nome, cpf, funcao, valor_diaria);
                    exibir_mensagem("Funcionário atualizado!");
                }
                break;
            }
            case 3:
                exibir_funcionario(equipeInterna);
                break;
            case 4:
                if (!equipeInterna) {
                    exibir_mensagem("Nenhum funcionário para deletar!");
                } else {
                    deletar_funcionario_eqIn(equipeInterna);
                    equipeInterna = NULL;
                    exibir_mensagem("Funcionário deletado!");
                }
                break;
            case 0:
                exibir_mensagem("Saindo...");
                break;
            default:
                exibir_mensagem("Opção inválida!");
        }
    } while (opcao != 0);

    if (equipeInterna) deletar_funcionario_eqIn(equipeInterna, tipo);
}
