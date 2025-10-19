#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../model/recursos_equipamentos/recursos_equipamentos.h"
#include "../view/recursos_equipamentos/recursos_equipamentos_view.h"
#include "../view/main/main_view.h"
#include "../model/config_armazenamento/config_armazenamento.h"
#include "recursos_equipamentos_controller.h"
#include "../../util/util.h"

void gerenciar_equipamento() {
    int opcao;

    do {
        opcao = exibir_menu_equipamentos();
        TipoArmazenamento tipo = get_armazenamento(); 

        switch (opcao) {
            // Adicionar equipamento
            case 1: {
                RecursosEquipamentos add = ler_dados_equipamento();
                adicionar_equipamento(&add, tipo);
                break;
            }
            // Atualizar equipamento
            case 2: {
                // Lê o ID do equipamento a ser atualizado
                int id_busca;
                printf("+ --------------------------------------------------------------- +\n");
                ler_int("|  Digite o id do equipamento que deseja atualizar: ", &id_busca);
                printf("+ --------------------------------------------------------------- +\n");

                RecursosEquipamentos* existe = buscar_equipamento_por_id(id_busca, tipo);
                if (!existe){
                    exibir_mensagem("Erro: Equipamento não encontrado ou sem permissão!");
                }else {
                    // Lê os novos dados
                    RecursosEquipamentos novos_dados = ler_dados_equipamento();
                    
                    // Atualiza o equipamento
                    atualizar_equipamento(id_busca, &novos_dados, tipo);
                }
                break;
            }
            // Exibir equipamento
            case 3: {
                int id_busca;
                printf("+ --------------------------------------------------------------- +\n");
                ler_int("|  Digite o id do equipamento que deseja exibir: ", &id_busca);
                printf("+ --------------------------------------------------------------- +\n");

                RecursosEquipamentos* eq = buscar_equipamento_por_id(id_busca, tipo);
                if (!eq) {
                    exibir_mensagem("Erro: Equipamento não encontrado ou sem permissão para exibir!");
                    break;
                } 
                exibir_equipamento(eq);
                break;
            }
            // Deletar equipamento
            case 4: {
                int id_busca;
                printf("+ --------------------------------------------------------------- +\n");
                ler_int("|  Digite o id do equipamento que deseja deletar: ", &id_busca);
                printf("+ --------------------------------------------------------------- +\n");
                int removido = deletar_equipamento(id_busca, tipo);
                if (removido == 1){
                    exibir_mensagem("Equipamento deletado com sucesso!");
                } else{
                    exibir_mensagem("Erro: Equipamento não encontrado ou sem permissão para excluir!");
                }
                break;
            }
            // Voltar ao menu anterior
            case 0: {
                exibir_mensagem("Saindo...");
                break;
            }
            default: {
                exibir_mensagem("Erro: Opção inválida!");
            }
        }
    } while (opcao != 0);
}