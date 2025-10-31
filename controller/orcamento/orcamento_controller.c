#include <stdlib.h>   
#include <stdio.h>
#include "../../model/orcamento/orcamento.h"
#include "../../view/orcamento/orcamento_view.h"
#include "../../view/main/main_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
#include "../../util/util.h"
#include "../../model/sessao/sessao.h"
#include "orcamento_controller.h"

void gerenciar_orcamento() {
    Orcamento* orcamento = NULL;
    int opcao;

    do {

        opcao = menu_orcamento();
        switch (opcao) {
            case 1: {
              Orcamento temp = ler_dados_orcamento(get_armazenamento());
                if (orcamento) {
                    exibir_mensagem("Já existe um orçamento. Delete-o antes de criar outro.");
                } else {
                     //criar_orcamento(&temp, get_armazenamento());
                    
                }
                break;
            }
            case 2: {
                break;
            }


            case 3:{ 
            }
            case 4:{ 
            }
            case 0:
                exibir_mensagem("Saindo...");
                break;
            default:
                exibir_mensagem("Opção inválida!");
        }
    } while (opcao != 0);

   
}
