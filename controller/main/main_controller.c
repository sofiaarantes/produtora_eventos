#include <stdlib.h>                         
#include "../controller/main/main_controller.h"  
#include "../controller/equipe_interna/equipe_interna_controller.h"  
#include "../controller/recursos_equipamentos/recursos_equipamentos_controller.h" 
#include "../controller/cliente/cliente_controller.h"
#include "../controller/fornecedor/fornecedor_parceiro_controller.h"
#include "../controller/produtora/produtora_controller.h"
#include "../controller/operadores_sistema/operadores_sistema_controller.h"
#include "../view/main/main_view.h"

// Controla o primeiro menu do sistema e chama cada menu correspondente
void iniciar_sistema(){ 
    int opcao;
    do{ 
        opcao = menu_main();
        switch (opcao){
        // Menu Cliente
        case 1:{
            gerenciar_cliente();
            break;
        }
        // Menu Produtora
        case 2:{
            gerenciar_produtora();
            break;
        }
        // Menu Equipe Interna
        case 3:{
            gerenciar_funcionario();
            break;
        }
        // Menu Recursos e Equipamentos
        case 4:{
            gerenciar_equipamento();
            break;
        }
        // Menu Fornecedores e Parceiros
        case 5:{
            gerenciar_fornecedor_parceiro();
            break;
        }
        // Editar conta
        case 7:{ 
            editar_operador();
            break;
        }
        // Sair
        case 0:{
            exibir_mensagem("\nEncerrando sessão...\n");
            return; // Volta pro while do main()
        }

        default:
            exibir_mensagem("Opção inválida. Tente novamente.\n");
            break;
        }
    } while (opcao != 0);
}
