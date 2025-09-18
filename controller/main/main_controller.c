#include <stdlib.h>                         
#include "../model/cliente/cliente.h"        
#include "../view/cliente/cliente_view.h"               
#include "../view/main/main_view.h" 
#include "../cliente/cliente_controller.h" 
#include "../equipe_interna/equipe_interna_controller.h" 

//essa funçao é responsavel por controlar o primeiro menu do sistema e chamar cada menu correspondente
iniciar_sistema(){ 
    int opcao;
    do{ 
        opcao = primeiro_menu();
        switch (opcao){
        case 1:{
            gerenciar_cliente();
            break;
        }
        case 2:{
            break;
        }
        case 3:{
            exibir_menu_equipe_interna();
            break;
        }
        
        default:
            break;
        }
        /* code */
    } while (opcao != 0);
}