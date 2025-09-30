#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../model/equipe_interna/equipe_interna.h"
#include "../view/equipe_interna/equipe_interna_view.h"
#include "../view/main/main_view.h"
#include "../model/config_armazenamento/config_armazenamento.h"
#include "equipe_interna_controller.h"


void gerenciar_funcionario() {
    EquipeInterna* equipeInterna = NULL;
    int opcao;

    do {
        opcao = exibir_menu_equipe_interna();
        switch (opcao) {
            // Adicionar funcionário
            case 1: {
                EquipeInterna add = ler_dados_funcionario();
                EquipeInterna* funcionario_salvo = adicionar_funcionario_eqIn(&add, get_armazenamento());
                break;
            }// ================================
// Atualizar funcionário
// ================================
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
    int cpf_busca;
    printf("Digite o CPF do funcionário que deseja atualizar: ");
    scanf("%d", &cpf_busca);

    // Variáveis que vão receber os novos dados
    char nome[50];
    char funcao[100];
    float valor_diaria;

    // Lê os novos dados
    ler_dados_atualizados_funcionario(nome, funcao, &valor_diaria);

    // Tenta atualizar no tipo de armazenamento configurado
    switch (tipo) {
        case MEMORIA: {
            EquipeInterna* funcionario_mem = buscar_funcionario_por_cpf_memoria(cpf_busca);
            if (funcionario_mem) {
                atualizar_funcionario_memoria(funcionario_mem, nome, funcao, valor_diaria);
                exibir_mensagem("Funcionário atualizado na MEMÓRIA!");
            } else {
                exibir_mensagem("Funcionário não encontrado na memória!");
            }
            break;
        }

        case TEXTO:
            if (atualizar_funcionario_texto(cpf_busca, nome, funcao, valor_diaria)) {
                exibir_mensagem("Funcionário atualizado no ARQUIVO TEXTO!");
            } else {
                exibir_mensagem("Funcionário não encontrado no arquivo texto!");
            }
            break;

        case BINARIO:
            if (atualizar_funcionario_binario(cpf_busca, nome, funcao, valor_diaria)) {
                exibir_mensagem("Funcionário atualizado no ARQUIVO BINÁRIO!");
            } else {
                exibir_mensagem("Funcionário não encontrado no arquivo binário!");
            }
            break;

        default:
            exibir_mensagem("Erro ao atualizar funcionário: tipo de armazenamento inválido!");
            break;
    }
    break;
}

            // Exibir funcionário
            case 3: {
                if (!equipeInterna) { 
                    exibir_funcionario(equipeInterna);
                    break;
                }
                break;
            }
            // Deletar funcionário
            case 4: {
                if (!equipeInterna) {
                    exibir_mensagem("Nenhum funcionário para deletar!");
                } else {
                    deletar_funcionario(equipeInterna);
                    equipeInterna = NULL;
                    exibir_mensagem("Funcionário deletado!");
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
