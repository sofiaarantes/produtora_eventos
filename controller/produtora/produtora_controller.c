#include <stdlib.h>   
#include <stdio.h>
#include "../../model/produtora/produtora.h"
#include "../../view/produtora/produtora_view.h"
#include "../../view/main/main_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
#include "../../util/util.h"
#include "produtora_controller.h"

void gerenciar_produtora() {
    Produtora* produtora = NULL;
    int opcao;

    do {

        opcao = menu_produtora();
        switch (opcao) {
            case 1: {//criar produtora
                Produtora temp = ler_dados_produtora();
                if (produtora) {
                    exibir_mensagem("Já existe uma produtora. Delete-a antes de criar outra.");
                } else {
                    Produtora* produtora_salva = criar_produtora(&temp, get_armazenamento());
                    exibir_mensagem("Produtora criada com sucesso!");
                }
                break;
            }
            case 2: { // Atualizar produtora
                    // char cnpj_busca[15]; //14 digitos + '\0'
                    // ler_string("Digite o CNPJ da produtora a ser atualizada: ", cnpj_busca, sizeof(cnpj_busca));


                    // // Crio uma struct temporária para armazenar os novos dados
                    // Produtora novos_dados;

                    // // Preencho com os novos dados (menos o CPF que não muda)
                    // ler_dados_atualizados_produtora(
                    //     novos_dados.nome_fantasia, sizeof(novos_dados.nome_fantasia),
                    //     novos_dados.razao_social, sizeof(novos_dados.razao_social),
                    //     novos_dados.inscricao_estadual, sizeof(novos_dados.inscricao_estadual),
                    //     novos_dados.endereco_completo, sizeof(novos_dados.endereco_completo),
                    //     novos_dados.tel, sizeof(novos_dados.tel),
                    //     novos_dados.email, sizeof(novos_dados.email),
                    //     novos_dados.nome_resp, sizeof(novos_dados.nome_resp),
                    //     novos_dados.tel_resp, sizeof(novos_dados.tel_resp),
                    //     novos_dados.lucro, sizeof(novos_dados.lucro)
                    // );
                    // //passar tambm o tamanho dos arrays para a funçao ler_string para saber ate onde pode ler


                    // // Chamo a função genérica de atualizar
                    // Produtora* atualizado = atualizar_produtora(cnpj_busca, &novos_dados, get_armazenamento());

                    // if (!atualizado) {
                    //     exibir_mensagem("Produtora não encontrada para atualização!");
                    // }

                    // break;
                }


            case 3:{ //exibir produtora
                // char cnpj_busca[12];
                //     ler_string("Digite o CNPJ da produtora a ser exibida: ", cnpj_busca, sizeof(cnpj_busca));

                //     buscar_e_exibir_produtora(cnpj_busca, get_armazenamento());
                // break;

            }
            case 4:{ //deletar produtora
                // char cnpj_busca[12];
                //     ler_string("Digite o CNPJ da produtora a ser deletada: ", cnpj_busca, sizeof(cnpj_busca));
                //     deletar_produtora(cnpj_busca, get_armazenamento());
                // break;
            }
            case 0:{
                exibir_mensagem("Saindo...");
                break;
            }
            default: {
                exibir_mensagem("Opção inválida!");
            }
        }
    } while (opcao != 0);

   
}
