#include <stdlib.h>                         
#include "../controller/main/main_controller.h" 
#include "../view/main/main_view.h" 
#include "../controller/cliente/cliente_controller.h" 
#include "../controller/equipe_interna/equipe_interna_controller.h" 
#include "../controller/config_armazenamento/config_armazenamento_controller.h" 
#include "../controller/recursos_equipamentos/recursos_equipamentos_controller.h" 

//essa funçao é responsavel por controlar o primeiro menu do sistema e chamar cada menu correspondente
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
            break;
        }
        // Menu Operadores do Sistema
        case 6:{
            break;
        }
        // Menu Configuração de Armazenamento
        case 7:{ 
            inicializar_armazenamento();
            break;
        }
        
        default:
            break;
        }
    } while (opcao != 0);
}
