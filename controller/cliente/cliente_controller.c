#include <stdlib.h>
#include "../model/cliente/cliente.h"
#include "../view/cliente/cliente_view.h"
#include "../view/main/main_view.h"
#include "../cliente/cliente_controller.h"


void gerenciar_cliente() {
    Cliente* cliente = NULL;
    int opcao;

    do {
        opcao = exibir_menu();
        switch (opcao) {
            case 1: {
                // Agora usamos a função com retorno
                Cliente temp = ler_dados_cliente();

                if (cliente) {
                    exibir_mensagem("Já existe um cliente. Delete-o antes de criar outro.");
                } else {
                    cliente = criar_cliente(temp.id, temp.nome, temp.idade);
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
                    ler_dados_atualizacao(nome, &idade);
                    atualizar_cliente(cliente, nome, idade);
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
