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
        switch (opcao) {
            // Adicionar funcionário
            case 1: {
                EquipeInterna add = ler_dados_funcionario();
                adicionar_funcionario(&add, get_armazenamento());
                break;
            }
            // Atualizar funcionário
            case 2: {
                int tem_funcionarios = 0;
                TipoArmazenamento tipo = get_armazenamento();

                // Verifica se existem funcionários no armazenamento atual
                switch (tipo) {
                    case MEMORIA:
                        if (get_qtd_funcionarios() > 0) {
                            tem_funcionarios = 1;
                        }
                        break;

                    case TEXTO: {
                        FILE* ft = fopen("funcionarios.txt", "r");
                        if (ft) {
                            char linha[300];
                            if (fgets(linha, sizeof(linha), ft)) {
                                tem_funcionarios = 1;
                            }
                            fclose(ft);
                        }
                        break;
                    }

                    case BINARIO: {
                        FILE* fb = fopen("funcionarios.bin", "rb");
                        if (fb) {
                            EquipeInterna tmp;
                            if (fread(&tmp, sizeof(EquipeInterna), 1, fb)) {
                                tem_funcionarios = 1;
                            }
                            fclose(fb);
                        }
                        break;
                    }

                    default:
                        exibir_mensagem("Tipo de armazenamento inválido!");
                        break;
                }

                if (!tem_funcionarios) {
                    exibir_mensagem("Nenhum funcionário cadastrado!");
                    break;
                }

                // Peço para o usuário digitar o CPF do funcionário que ele quer atualizar
                char cpf_busca[20];
                ler_string("Digite o CPF do funcionário que deseja atualizar: ", cpf_busca, sizeof(cpf_busca));
                

                // Variáveis que vão receber os novos dados
                char nome[50];
                char funcao[100];
                float valor_diaria;

                // Lê os novos dados
                ler_dados_atualizados_funcionario(nome, funcao, &valor_diaria);

                // Tenta atualizar no tipo de armazenamento configurado
                switch (tipo) {
                    case MEMORIA: {
                        EquipeInterna* func = buscar_funcionario_por_cpf_memoria(cpf_busca);
                        if (func) atualizar_funcionario_memoria(func, nome, funcao, valor_diaria);
                        break;
                    }
                    case TEXTO: {
                        atualizar_funcionario_texto(cpf_busca, nome, funcao, valor_diaria);
                        break;
                    }
                    case BINARIO: {
                        atualizar_funcionario_binario(cpf_busca, nome, funcao, valor_diaria);
                        break;
                    }
                    default:
                        exibir_mensagem("Erro ao atualizar funcionário: tipo de armazenamento inválido!");
                        break;
                }
                break;
            }
            // Exibir funcionário
            case 3: {
                TipoArmazenamento tipo = get_armazenamento();

                char cpf_busca[20];
                ler_string("Digite o CPF do funcionário que deseja exibir: ", cpf_busca, sizeof(cpf_busca));
                
                EquipeInterna* func = NULL;

                switch (tipo) {
                    case MEMORIA:
                        func = buscar_funcionario_por_cpf_memoria(cpf_busca);
                        break;

                    case TEXTO:
                        func = buscar_funcionario_por_cpf_texto(cpf_busca);
                        break;

                    case BINARIO:
                        func = buscar_funcionario_por_cpf_binario(cpf_busca);
                        break;

                    default:
                        exibir_mensagem("Tipo de armazenamento inválido!");
                        break;
                }

                exibir_funcionario(func);
                break;
            }
            // Deletar funcionário
            case 4: {
                char cpf_busca[20];
                printf("Digite o CPF do funcionário que deseja deletar: ");
                scanf(" %19s", cpf_busca); // lê até 19 caracteres

                TipoArmazenamento tipo = get_armazenamento();
                int removido = 0;

                switch (tipo) {
                    case MEMORIA:
                        removido = deletar_funcionario_memoria(cpf_busca);
                        break;
                    case TEXTO:
                        removido = deletar_funcionario_texto(cpf_busca);
                        break;
                    case BINARIO:
                        removido = deletar_funcionario_binario(cpf_busca);
                        break;
                    default:
                        exibir_mensagem("Tipo de armazenamento inválido!");
                        break;
                }

                if (removido) {
                    exibir_mensagem("Funcionário deletado com sucesso!");
                } else {
                    exibir_mensagem("Funcionário não encontrado!");
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
