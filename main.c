#include "controller/operadores_sistema/operadores_sistema_controller.h"
#include "../../controller/main/main_controller.h"
#include "../../view/main/main_view.h"
#include "../../model/sessao/sessao.h"

// =============================
// PONTO DE ENTRADA DO SISTEMA
// =============================

int main() {
    while (1) {
        // Faz login ou cadastro
        gerenciar_login();
        // Entra no menu principal
        exibir_mensagem("+ --------------------------------------------------------------- +");
        exibir_mensagem("|                       Seja Bem Vindo(a)!                        |");
        exibir_mensagem("+ --------------------------------------------------------------- +");
        iniciar_sistema();   

        // Quando o usuário escolher "0 - Sair" no menu principal, voltará aqui
        limpar_sessao();     // Limpa operador logado
        exibir_mensagem("Você foi desconectado.\n\n");
    }
    return 0;
}

