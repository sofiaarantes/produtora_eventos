#include <stdlib.h>                         
#include "../model/cliente/cliente.h"        
#include "../view/cliente/cliente_view.h"               
#include "../view/main/main_view.h" 
#include "../cliente/cliente_controller.h" 

iniciar_sistema(){ //essa funçao é responsavel por controlar o primeiro menu so sistema e chamar cada menu correspondente
int opcao;
    do
    { opcao = primeiro_menu();
        switch (opcao)
        {
        case 1:{
            gerenciar_cliente();
        }
            
            break;
        
        default:
            break;
        }
        /* code */
    } while (opcao != 0);
    

}