#include <stdlib.h>
#include "../model/config_armazenamento/config_armazenamento.h"
#include "../view/main/main_view.h"
#include "../controller/main/main_controller.h"
#include "../controller/config_armazenamento/config_armazenamento_controller.h"

int inicializar_armazenamento() { 
    int opcao;
    do { 
        opcao = mostrar_menu_configuracao();
        switch (opcao){
        case 1:{
            set_armazenamento((TipoArmazenamento) MEMORIA);
            exibir_mensagem("Armazenamento configurado!");
            iniciar_sistema();
            
            break;
        }
        case 2:{
            set_armazenamento((TipoArmazenamento) TEXTO);
            exibir_mensagem("Armazenamento configurado!");
            iniciar_sistema();
            
            break;
        }
        case 3:{
            set_armazenamento((TipoArmazenamento) BINARIO);
            exibir_mensagem("Armazenamento configurado!");
            iniciar_sistema();
            
            break;
        }
        default:
            break;
        }
    } while (opcao != 0);

   
}