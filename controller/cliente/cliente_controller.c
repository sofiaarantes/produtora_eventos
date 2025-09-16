#include <stdlib.h>                         
#include "../model/cliente/cliente.h"        
#include "../view/cliente/cliente_view.h"    
#include "cliente_controller.h"             
#include "../view/main/main_view.h"   

void iniciar_sistema() {
    int escolher;

    do
    { escolher = primeiro_menu(); //esse é o menu que da acesso as funcionalidades do sistema
       
        switch (escolher)
        {
        case 1:{Cliente* cliente = NULL;  // Ponteiro para armazenar o cliente criado dinamicamente. Inicialmente NULL.
    int opcao;                // Variável para armazenar a opção escolhida no menu

    /* Loop principal do menu do cliente */
    do {
        opcao = exibir_menu();  // Exibe o menu de opções (criar, atualizar, mostrar, deletar) e lê a escolha do usuário

        switch (opcao) {

            case 1: {  // Criar cliente
                // Lê os dados do cliente para uma struct temporária (na pilha)
                Cliente temp = ler_dados_cliente();

                // Verifica se já existe um cliente alocado
                if (cliente) {
                    // Se o ponteiro já aponta para um cliente existente, não criamos outro
                    // Isso evita vazamento de memória (memory leak)
                    exibir_mensagem("Já existe um cliente. Delete-o antes de criar outro.");
                } else {
                    // Cria um cliente na memória dinâmica (heap) e guarda o ponteiro
                    // Agora cliente aponta para um bloco de memória válido
                    cliente = criar_cliente(temp.id, temp.nome, temp.idade);
                    exibir_mensagem("Cliente criado com sucesso!");
                }
                break;  // Sai do switch
            }

            case 2: {  // Atualizar cliente
                if (!cliente) {
                    // Se não houver cliente alocado, informa o usuário
                    exibir_mensagem("Nenhum cliente cadastrado!");
                } else {
                    char nome[50];  // Buffer para armazenar o novo nome
                    int idade;      // Variável para armazenar a nova idade

                    // Lê os novos dados do usuário
                    ler_dados_atualizacao(nome, &idade);

                    // Atualiza os dados do cliente apontado por cliente
                    atualizar_cliente(cliente, nome, idade);
                    exibir_mensagem("Cliente atualizado!");
                }
                break;
            }

            case 3:{  // Exibir cliente
                // Chama função de view para mostrar os dados do cliente
                // A função deve tratar cliente == NULL internamente para evitar segfault
                exibir_cliente(cliente);
            }
                break;

            case 4:{  // Deletar cliente
                if (!cliente) {
                    // Se não houver cliente, informa o usuário
                    exibir_mensagem("Nenhum cliente para deletar!");
                } else {
                    // Libera a memória do cliente dinamicamente alocado
                    deletar_cliente(cliente);
                    cliente = NULL;  // Atualiza ponteiro para NULL para evitar ponteiro pendente
                    exibir_mensagem("Cliente deletado!");
                }
            }
                break;

            case 0:{  // Sair do menu
                exibir_mensagem("Saindo...");  // Mensagem de saída
            }
                break;

            default:{  // Qualquer opção inválida
                exibir_mensagem("Opção inválida!");  // Feedback ao usuário
            }
        }
    } while (opcao != 0);  // Repete enquanto o usuário não escolher sair

    /* Limpeza final: caso o usuário saia sem deletar o cliente */
    if (cliente) deletar_cliente(cliente);  // Evita vazamento de memória

        }break;
        
        default:
            break;
        }
    } while (escolher != 0 );
    
    
}
