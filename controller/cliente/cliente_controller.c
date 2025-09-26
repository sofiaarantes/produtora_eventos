#include <stdlib.h>   
#include <stdio.h>
#include "../../model/cliente/cliente.h"
#include "../../view/cliente/cliente_view.h"
#include "../../view/main/main_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
#include "cliente_controller.h"

void gerenciar_cliente() {
    Cliente* cliente = NULL;
    int opcao;

    do {
        
        opcao = exibir_menu();
        switch (opcao) {
            case 1: {//criar cliente
                Cliente temp = ler_dados_cliente();
                if (cliente) {
                    exibir_mensagem("Já existe um cliente. Delete-o antes de criar outro.");
                } else {
                    Cliente* cliente_salvo = criar_cliente(&temp, get_armazenamento());
                    exibir_mensagem("Cliente criado com sucesso!");
                }
                break;
            }
            case 2: { // Atualizar cliente
                // Primeiro verifico se existe algum cliente cadastrado
                if (get_qtd_clientes == 0) {
                    exibir_mensagem("Nenhum cliente cadastrado!");
                } else {
                    // Peço para o usuário digitar o CPF/CNPJ do cliente que ele quer atualizar
                    char cpf_cnpj_busca[20];
                    printf("Digite o CPF/CNPJ do cliente que deseja atualizar: ");
                    scanf(" %19s", cpf_cnpj_busca); // uso %19s para não estourar o buffer

                    // Declaro as variáveis que vão receber os novos dados do cliente
                    char nome[50];
                    int idade;
                    char endereco_completo[100];
                    char tel[15];
                    char email[50];
                    char nome_contato[50];

                    // Aqui eu chamo a função que lê os novos dados do cliente
                    // IMPORTANTE: não vou atualizar o CPF/CNPJ, ele serve apenas para buscar o cliente
                    
                    ler_dados_atualizados_cliente(nome, &idade, endereco_completo, tel, email, nome_contato);

                    // ================================
                    // BUSCO O CLIENTE NA MEMÓRIA
                    // ================================
                    Cliente* cliente_mem = buscar_cliente_por_cpf_memoria(cpf_cnpj_busca);
                    if (cliente_mem) {
                        // Achei o cliente na memória, atualizo apenas os campos que podem mudar
                        atualizar_cliente_memoria(cliente_mem, nome, idade, endereco_completo, tel, email,nome_contato);
                        exibir_mensagem("Cliente atualizado na MEMÓRIA!");
                        break; // já atualizei, não preciso verificar os outros tipos
                    }

                    // ================================
                    // BUSCO O CLIENTE NO ARQUIVO TEXTO
                    // ================================
                    Cliente* cliente_txt = buscar_cliente_por_cpf_texto(cpf_cnpj_busca);
                    if (cliente_txt) {
                        atualizar_cliente_texto(cliente_txt, nome, idade, endereco_completo, tel, email, nome_contato);
                        exibir_mensagem("Cliente atualizado no ARQUIVO TEXTO!");
                        break;
                    }

                    // ================================
                    // BUSCO O CLIENTE NO ARQUIVO BINÁRIO
                    // ================================
                    Cliente* cliente_bin = buscar_cliente_por_cpf_binario(cpf_cnpj_busca);
                    if (cliente_bin) {
                        atualizar_cliente_binario(cliente_bin, nome, idade, endereco_completo, tel, email, nome_contato);
                        exibir_mensagem("Cliente atualizado no ARQUIVO BINÁRIO!");
                        break;
                    }

                    // Se não encontrar em nenhum lugar, exibo mensagem
                    exibir_mensagem("Cliente não encontrado!");
                }
                break;
            }

            case 3:{ //exibir cliente
                if (!cliente) { 
                exibir_cliente(cliente);
                break;
                }
            }
            case 4:{ //deletar cliente
                if (!cliente) {
                    exibir_mensagem("Nenhum cliente para deletar!");
                } else {
                    deletar_cliente(cliente);
                    cliente = NULL;
                    exibir_mensagem("Cliente deletado!");
                }
                break;
            }
            case 0:
                exibir_mensagem("Saindo...");
                break;
            default:
                exibir_mensagem("Opção inválida!");
        }
    } while (opcao != 0);

    if (cliente) deletar_cliente(cliente);
}
