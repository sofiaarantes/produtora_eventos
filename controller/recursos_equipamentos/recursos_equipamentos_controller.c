#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../model/recursos_equipamentos/recursos_equipamentos.h"
#include "../view/recursos_equipamentos/recursos_equipamentos_view.h"
#include "../view/main/main_view.h"
#include "../model/config_armazenamento/config_armazenamento.h"
#include "recursos_equipamentos_controller.h"


void gerenciar_equipamento() {
    RecursosEquipamentos* equipamentos = NULL;
    int opcao;

    do {
        opcao = exibir_menu_equipamentos();
        switch (opcao) {
            // Adicionar equipamento
            case 1: {
                RecursosEquipamentos add = ler_dados_equipamento();
                equipamentos = adicionar_equipamento(&add, get_armazenamento());
                break;
            }
            // Atualizar equipamento
            case 2: {
                int tem_equipamentos = 0;
                TipoArmazenamento tipo = get_armazenamento();

                // Verifica se existem equipamentos no armazenamento atual
                switch (tipo) {
                    case MEMORIA:
                        if (get_qtd_equipamentos() > 0) {
                            tem_equipamentos = 1;
                        }
                        break;

                    case TEXTO: {
                        FILE* ft = fopen("equipamentos.txt", "r");
                        if (ft) {
                            char linha[300];
                            if (fgets(linha, sizeof(linha), ft)) {
                                tem_equipamentos = 1;
                            }
                            fclose(ft);
                        }
                        break;
                    }

                    case BINARIO: {
                        FILE* fb = fopen("equipamentos.bin", "rb");
                        if (fb) {
                            RecursosEquipamentos tmp;
                            if (fread(&tmp, sizeof(RecursosEquipamentos), 1, fb)) {
                                tem_equipamentos = 1;
                            }
                            fclose(fb);
                        }
                        break;
                    }

                    default:
                        exibir_mensagem("Tipo de armazenamento inválido!");
                        break;
                }

                if (!tem_equipamentos) {
                    exibir_mensagem("Nenhum equipamento cadastrado!");
                    break;
                }

                // Peço para o usuário digitar o id do equipamento que ele quer atualizar
                int id_busca;
                printf("Digite o id do equipamento que deseja atualizar: ");
                scanf(" %d", &id_busca);   

                // Variáveis que vão receber os novos dados
                char descricao[100];
                char categoria[50];
                int qtd_estoque;
                float preco_custo;
                float valor_diaria;

                // Lê os novos dados
                ler_dados_atualizados_equipamento(descricao, categoria, &qtd_estoque, &preco_custo, &valor_diaria);

                // Tenta atualizar no tipo de armazenamento configurado
                switch (tipo) {
                    case MEMORIA: {
                        RecursosEquipamentos* eq = buscar_equipamento_por_id_memoria(id_busca);
                        if (eq) atualizar_equipamento_memoria(eq, descricao, categoria, qtd_estoque, preco_custo, valor_diaria);
                        break;
                    }
                    case TEXTO: {
                        atualizar_equipamento_texto(id_busca, descricao, categoria, qtd_estoque, preco_custo, valor_diaria);
                        break;
                    }
                    case BINARIO: {
                        atualizar_equipamento_binario(id_busca, descricao, categoria, qtd_estoque, preco_custo, valor_diaria);
                        break;
                    }
                    default:
                        exibir_mensagem("Erro ao atualizar equipamento: tipo de armazenamento inválido!");
                        break;
                }
                break;
            }
            // Exibir equipamento
            case 3: {
                TipoArmazenamento tipo = get_armazenamento();

                int id_busca;
                printf("Digite o id do equipamento que deseja exibir: ");
                scanf(" %d", &id_busca);   

                RecursosEquipamentos* eq = NULL;

                switch (tipo) {
                    case MEMORIA:
                        eq = buscar_equipamento_por_id_memoria(id_busca);
                        break;

                    case TEXTO:
                        eq = buscar_equipamento_por_id_texto(id_busca);
                        break;

                    case BINARIO:
                        eq = buscar_equipamento_por_id_binario(id_busca);
                        break;

                    default:
                        exibir_mensagem("Tipo de armazenamento inválido!");
                        break;
                }

                exibir_equipamento(eq);
                break;
            }
            // Deletar equipamento
            case 4: {
                int id_busca;
                printf("Digite o id do equipamento que deseja deletar: ");
                scanf(" %d", &id_busca); 

                TipoArmazenamento tipo = get_armazenamento();
                int removido = 0;

                switch (tipo) {
                    case MEMORIA:
                        removido = deletar_equipamento_memoria(id_busca);
                        break;
                    case TEXTO:
                        removido = deletar_equipamento_texto(id_busca);
                        break;
                    case BINARIO:
                        removido = deletar_equipamento_binario(id_busca);
                        break;
                    default:
                        exibir_mensagem("Tipo de armazenamento inválido!");
                        break;
                }

                if (removido) {
                    exibir_mensagem("Equipamento deletado com sucesso!");
                } else {
                    exibir_mensagem("Equipamento não encontrado!");
                }
                break;
            }
            // Voltar ao menu anterior
            case 0: {
                exibir_mensagem("Saindo...");
                break;
            }
            default:{
                exibir_mensagem("Opção inválida!");
            }
        }
    } while (opcao != 0);
}
