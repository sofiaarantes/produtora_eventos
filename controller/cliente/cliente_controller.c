#include <stdlib.h>   
#include <stdio.h>
#include "../../model/cliente/cliente.h"
#include "../../view/cliente/cliente_view.h"
#include "../../view/main/main_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
#include "../../util/util.h"
#include "cliente_controller.h"

void gerenciar_cliente() {
    Cliente* cliente = NULL;
    int opcao;

    do {
        
        opcao = menu_cliente();
        switch (opcao) {
            case 1: {//criar cliente
                Cliente temp = ler_dados_cliente();
                if (cliente) {
                    exibir_mensagem("Já existe um cliente. Delete-o antes de criar outro.");
                } else {
                     criar_cliente(&temp, get_armazenamento());
                    
                }
                break;
            }
            case 2: { // Atualizar cliente
                    char cpf_cnpj_busca[20];
                    ler_string("Digite o CPF/CNPJ do cliente a ser atualizado: ", cpf_cnpj_busca, sizeof(cpf_cnpj_busca));


                    // Crio uma struct temporária para armazenar os novos dados
                    Cliente novos_dados;

                    // Preencho com os novos dados (menos o CPF que não muda)
                    ler_dados_atualizados_cliente(
                        novos_dados.nome,   
                        &novos_dados.idade,
                        novos_dados.endereco_completo,
                        novos_dados.tel,
                        novos_dados.email,
                        novos_dados.nome_contato
                    );
                        


                    // Chamo a função genérica de atualizar
                   atualizar_cliente(cpf_cnpj_busca, &novos_dados, get_armazenamento());

                

                    break;
                }


            case 3:{ //exibir cliente
                char cpf_cnpj_busca[20];
                    ler_string("Digite o CPF/CNPJ do cliente a ser exibido: ", cpf_cnpj_busca, sizeof(cpf_cnpj_busca));

                    buscar_e_exibir_cliente(cpf_cnpj_busca, get_armazenamento());
                break;

            }
            case 4:{ //deletar cliente
                char cpf_cnpj_busca[20];
                    ler_string("Digite o CPF/CNPJ do cliente a ser deletado: ", cpf_cnpj_busca, sizeof(cpf_cnpj_busca));
                    deletar_cliente(cpf_cnpj_busca, get_armazenamento());
                break;
            }
            case 0:
                exibir_mensagem("Saindo...");
                break;
            default:
                exibir_mensagem("Opção inválida!");
        }
    } while (opcao != 0);

   
}
