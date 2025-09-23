#include <stdlib.h>   
#include "../../model/cliente/cliente.h"
#include "../../view/cliente/cliente_view.h"
#include "../../view/main/main_view.h"
#include "cliente_controller.h"

void gerenciar_cliente() {
    Cliente* cliente = NULL;
    int opcao;

    do {
        opcao = exibir_menu();
        switch (opcao) {
            case 1: {
                Cliente temp = ler_dados_cliente();
                if (cliente) {
                    exibir_mensagem("Já existe um cliente. Delete-o antes de criar outro.");
                } else {
                    cliente = criar_cliente(temp.id, temp.nome, temp.idade,
                                            temp.endereco_completo,
                                            temp.cpf_cnpj,
                                            temp.tel,
                                            temp.email,
                                            temp.nome_contato);
                    exibir_mensagem("Cliente criado com sucesso!");
                }
                break;
            }
            case 2: {
                if (!cliente) {
                    exibir_mensagem("Nenhum cliente cadastrado!");
                } else {
                    char nome[50];
                    int idade;
                    char endereco_completo[100];
                    char cpf_cnpj[20];
                    char tel[15];
                    char email[50];
                    char nome_contato[50];

                 ler_dados_atualizados_cliente(nome,&idade,endereco_completo,cpf_cnpj,tel,email,nome_contato);
                    atualizar_cliente(cliente, nome, idade,
                                      endereco_completo,
                                      cpf_cnpj,
                                      tel,
                                      email,
                                      nome_contato);
                    exibir_mensagem("Cliente atualizado!");
                }
                break;
            }
            case 3:
                exibir_cliente(cliente);
                break;
            case 4:
                if (!cliente) {
                    exibir_mensagem("Nenhum cliente para deletar!");
                } else {
                    deletar_cliente(cliente);
                    cliente = NULL;
                    exibir_mensagem("Cliente deletado!");
                }
                break;
            case 0:
                exibir_mensagem("Saindo...");
                break;
            default:
                exibir_mensagem("Opção inválida!");
        }
    } while (opcao != 0);

    if (cliente) deletar_cliente(cliente);
}
