#include <stdlib.h>
#include "../model/config_armazenamento/config_armazenamento.h"
#include "../view/main/main_view.h"
#include "../controller/main/main_controller.h"
#include "../controller/config_armazenamento/config_armazenamento_controller.h"

int inicializar_armazenamento() { 
    int opcao = mostrar_menu_configuracao();

    switch (opcao) {
        case 1:
            set_armazenamento(MEMORIA);
            break;
        case 2:
            set_armazenamento(TEXTO);
            break;
        case 3:
            set_armazenamento(BINARIO);
            break;
        case 0:
            exibir_mensagem("Saindo da configuracao.\n");
            return 0;
        default:
            exibir_mensagem("Opção invalida\n");
            return inicializar_armazenamento(); // repete
    }

    exibir_mensagem("Armazenamento configurado!");
    iniciar_sistema(); // inicia e não volta aqui
    return 0;
}
