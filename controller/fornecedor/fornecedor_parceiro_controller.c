#include <stdlib.h>   
#include <stdio.h>
#include "../../model/fornecedor/fornecedor_parceiro.h"
#include "../../view/fornecedor/fornecedor_parceiro_view.h"
#include "../../view/main/main_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
#include "../../util/util.h"
#include "fornecedor_parceiro_controller.h"

void gerenciar_fornecedor_parceiro() {
    Fornecedor_parceiro* fornecedor_parceiro = NULL;
    int opcao;

    do {

        opcao = menu_fornecedor();
        switch (opcao) {
            case 1: {//criar cliente
                Fornecedor_parceiro temp = ler_dados_fornecedor_parceiro();
                if (fornecedor_parceiro) {
                    exibir_mensagem("Já existe um fornecedor/parceiro. Delete-o antes de criar outro.");
                } else {
                     criar_fornecedor_parceiro(&temp, get_armazenamento());
                }
                break;
            }
            case 2: { // Atualizar fornecedor/parceiro
                    char cnpj_busca[15];
                    ler_string("Digite o CNPJ do fornecedor/parceiro a ser atualizado: ", cnpj_busca, sizeof(cnpj_busca));


                    // Crio uma struct temporária para armazenar os novos dados
                    Fornecedor_parceiro novos_dados;

                    // Preencho com os novos dados (menos o CPF que não muda)
                    ler_dados_atualizados_fornecedor_parceiro(
                        novos_dados.nome_fantasia,  
                        novos_dados.razao_social,
                        novos_dados.endereco_completo,
                        novos_dados.tel,
                        novos_dados.tipo_servico
                    );
                    

                    // Chamo a função genérica de atualizar
                    Fornecedor_parceiro* atualizado = atualizar_fornecedor_parceiro(cnpj_busca, &novos_dados, get_armazenamento());

                    if (!atualizado) {
                        exibir_mensagem("Fornecedor/Parceiro não encontrado para atualização!");
                    }

                    break;
                }


            case 3:{ //exibir fornecedor/parceiro
                char cnpj_busca[15];
                    ler_string("Digite o CNPJ do fornecedor/parceiro a ser exibido: ", cnpj_busca, sizeof(cnpj_busca));

                    buscar_e_exibir_fornecedor_parceiro(cnpj_busca, get_armazenamento());
                break;

            }
            case 4:{ //deletar fornecedor/parceiro
                char cnpj_busca[15];
                    ler_string("Digite o CNPJ do fornecedor/parceiro a ser deletado: ", cnpj_busca, sizeof(cnpj_busca));
                    deletar_fornecedor_parceiro(cnpj_busca, get_armazenamento());
                break;
            }
            case 0:{
                exibir_mensagem("Saindo...");
                break;
            }
            default:{
            exibir_mensagem("Opção inválida!");  
            } 
        }
    } while (opcao != 0);
}



