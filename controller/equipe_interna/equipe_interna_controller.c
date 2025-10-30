#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../model/equipe_interna/equipe_interna.h"
#include "../view/equipe_interna/equipe_interna_view.h"
#include "../view/main/main_view.h"
#include "../model/config_armazenamento/config_armazenamento.h"
#include "equipe_interna_controller.h"
#include "../../util/util.h"

void gerenciar_funcionario() {
    int opcao;

    do {
        opcao = exibir_menu_equipe_interna();
        TipoArmazenamento tipo = get_armazenamento();

        switch (opcao) {
            // Adicionar funcionário
            case 1: {
                EquipeInterna add = ler_dados_funcionario();
                adicionar_funcionario(&add, tipo);
                break;
            }
            // Atualizar Funcionário
            case 2: {
                // CPF do funcionário a ser atualizado
                char cpf_busca[20];
                printf("+ --------------------------------------------------------------- +\n");
                ler_string("|  Digite o CPF do funcionário que deseja atualizar: ", cpf_busca, sizeof(cpf_busca));
                printf("+ --------------------------------------------------------------- +\n");

                EquipeInterna* existe = buscar_funcionario_por_cpf(cpf_busca, tipo);
                if (!existe){
                    exibir_mensagem("Erro: Funcionário não encontrado ou sem permissão para atualização!");
                }else {
                    // Struct temporária para armazenar novos dados
                    EquipeInterna novos_dados;
                    ler_dados_atualizados_funcionario(novos_dados.nome, novos_dados.funcao, &novos_dados.valor_diaria);
                    atualizar_funcionario(cpf_busca, &novos_dados, tipo);
                }
                break;
            }
            // Exibir funcionário
            case 3: {
                char cpf_busca[20];
                printf("+ --------------------------------------------------------------- +\n");
                ler_string("|  Digite o CPF do funcionário que deseja exibir: ", cpf_busca, sizeof(cpf_busca));
                printf("+ --------------------------------------------------------------- +\n");
                EquipeInterna* func = buscar_funcionario_por_cpf(cpf_busca, tipo);
                if (!func) {
                    exibir_mensagem("Erro: Funcionário não encontrado ou sem permissão para exibir!");
                    break;
                }

                exibir_funcionario(func);
                break;
            }
            // Deletar funcionário
            case 4: {
                char cpf_busca[20];
                printf("+ --------------------------------------------------------------- +\n");
                ler_string("|  Digite o CPF do funcionário que deseja deletar: ", cpf_busca, sizeof(cpf_busca));
                printf("+ --------------------------------------------------------------- +\n");
                int removido = deletar_funcionario(cpf_busca, tipo);
                if (removido == 1){
                    exibir_mensagem("Funcionário deletado com sucesso!");
                } else{
                    exibir_mensagem("Erro: Funcionário não encontrado ou sem permissão para excluir!");
                }
                break;
            }
            // Voltar ao menu anterior
            case 0: {
                exibir_mensagem("Saindo...");
                break;
            }
            default:{
                exibir_mensagem("Erro: Opção inválida!");
            }
        }
    } while (opcao != 0);
}
