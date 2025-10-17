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
            // Atualizar Funcionário
            case 2: {
                int tem_funcionarios = 0;
                TipoArmazenamento tipo = get_armazenamento();

                // Verifica se existem funcionários no armazenamento atual
                switch (tipo) {
                    case MEMORIA:
                        if (get_qtd_funcionarios() > 0) tem_funcionarios = 1;
                        break;
                    case TEXTO: {
                        FILE* ft = fopen("funcionarios.txt", "r");
                        if (ft) {
                            char linha[300];
                            if (fgets(linha, sizeof(linha), ft)) tem_funcionarios = 1;
                            fclose(ft);
                        }
                        break;
                    }
                    case BINARIO: {
                        FILE* fb = fopen("funcionarios.bin", "rb");
                        if (fb) {
                            EquipeInterna tmp;
                            if (fread(&tmp, sizeof(EquipeInterna), 1, fb)) tem_funcionarios = 1;
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

                // Solicita CPF do funcionário a ser atualizado
                char cpf_busca[20];
                ler_string("Digite o CPF do funcionário que deseja atualizar: ", cpf_busca, sizeof(cpf_busca));

                // Struct temporária para armazenar novos dados
                EquipeInterna novos_dados;
                ler_dados_atualizados_funcionario(novos_dados.nome, novos_dados.funcao, &novos_dados.valor_diaria);

                // Chama a função unificada de atualização
                EquipeInterna* atualizado = atualizar_funcionario(cpf_busca, &novos_dados, tipo);

                if (!atualizado) {
                    exibir_mensagem("Funcionário não encontrado ou sem permissão para atualização!");
                } else {
                    exibir_mensagem("Funcionário atualizado com sucesso!");
                }

                break;
            }
            // Exibir funcionário
            case 3: {
                TipoArmazenamento tipo = get_armazenamento();

                char cpf_busca[20];
                ler_string("Digite o CPF do funcionário que deseja exibir: ", cpf_busca, sizeof(cpf_busca));
                
                // Busca o funcionário (restrito ao operador logado)
                EquipeInterna* func = buscar_funcionario_por_cpf(cpf_busca, tipo);
                if (!func) {
                    exibir_mensagem("Funcionário não encontrado ou sem permissão para editar.");
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

                deletar_funcionario(cpf_busca, tipo);

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
