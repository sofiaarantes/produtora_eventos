#include <stdio.h>
#include "orcamento/orcamento_view.h"
#include "../../util/util.h"
#include "../../view/main/main_view.h"
#include "../../model/cliente/cliente.h"
#include "../../model/equipe_interna/equipe_interna.h"
#include "../../model/recursos_equipamentos/recursos_equipamentos.h"
#include "../../model/fornecedor/fornecedor_parceiro.h"
#include "../../model/orcamento/orcamento.h"
#include "../../model/config_armazenamento/config_armazenamento.h"

int menu_orcamento() {
    
     int opcao;
    printf("====================================================================\n");
    printf("|                        Menu de Orcamento                         |\n");
    printf("====================================================================\n");
    printf("|  1 - Fazer um novo Orcamento                                     |\n");
    printf("|  2 - Aprovar um Orçamento                                        |\n");
    printf("|  3 - Exibir um Orçamento                                         |\n");
    printf("|  4 - Deletar um Orçamento                                        |\n");
    printf("|  0 - Voltar ao menu anterior                                     |\n");
    printf("+ ---------------------------------------------------------------- +\n");
    ler_int("Escolha uma opcao: ", &opcao);
    return opcao;
}

Orcamento ler_dados_orcamento(TipoArmazenamento tipo) {
    Orcamento o;
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                        Novo Orcamento                           |\n");
    printf("+ --------------------------------------------------------------- +\n");

    // ----------------------------------------------------------------
    // Leitura dos dados básicos do orçamento
    // ----------------------------------------------------------------
    ler_string("|  Local: ", o.local, sizeof(o.local));
    ler_string("|  Data de Inicio do Evento (formato DD/MM/YYYY): ", o.dataInicio, sizeof(o.dataInicio));
    ler_string("|  Data de Fim do Evento (formato DD/MM/YYYY): ", o.dataFim, sizeof(o.dataFim));
    ler_string("|  Horario Inicio do Evento (formato HH:MM): ", o.horaInicio, sizeof(o.horaInicio));
    ler_string("|  Horario Fim do Evento (formato HH:MM): ", o.horaFim, sizeof(o.horaFim));
    printf("+ --------------------------------------------------------------- +\n");

    // ----------------------------------------------------------------
    // Exibição e escolha do cliente
    // ----------------------------------------------------------------
    
    listar_clientes(tipo); // exibe lista de clientes do operador logado
    ler_int("\n|  Digite o ID do cliente que deseja associar ao orçamento: ", &o.idCliente);

    // ----------------------------------------------------------------
    // Escolha dos SERVIÇOS (fornecedores)
    // ----------------------------------------------------------------
    
    listar_fornecedores(tipo); // exibe lista de fornecedores cadastrados

    int escolhas_fornecedores[100]; // guarda os IDs escolhidos
    int qtd_fornecedores = 0;       // contador de quantos foram escolhidos

    printf("\n|  Digite os IDs dos servicos que deseja adicionar (0 para encerrar):\n");
    while (1) {
        int idEscolha;
        ler_int("|  ID: ", &idEscolha);
        if (idEscolha == 0)
            break; // encerra quando o usuário digitar 0
        escolhas_fornecedores[qtd_fornecedores++] = idEscolha;
    }

    // ----------------------------------------------------------------
    // Escolha da EQUIPE INTERNA
    // ----------------------------------------------------------------
    
    listar_equipe_interna(tipo); // exibe lista de equipe interna cadastrada

    int escolhas_equipeInterna[100];
    int qtd_equipeInterna = 0;

    printf("\n|  Digite os IDs dos membros da equipe interna que deseja adicionar (0 para encerrar):\n");
    while (1) {
        int idEscolha;
        ler_int("|  ID: ", &idEscolha);
        if (idEscolha == 0)
            break;
        escolhas_equipeInterna[qtd_equipeInterna++] = idEscolha;
    }

    // ----------------------------------------------------------------
    // Escolha dos RECURSOS
    // ----------------------------------------------------------------
    listar_equipamentos(tipo); // exibe lista de recursos cadastrados

    int escolhas_recursos[100];
    int qtd_recursos = 0;

    printf("\n|  Digite os IDs dos recursos que deseja adicionar (0 para encerrar):\n");
    printf("|  Repita o ID para adicionar mais unidades do mesmo recurso.\n");
    while (1) {
        int idEscolha;
        ler_int("|  ID : ", &idEscolha);
        if (idEscolha == 0)
            break;
        escolhas_recursos[qtd_recursos++] = idEscolha;
    }

    // ----------------------------------------------------------------
    // Resumo das escolhas feitas 
    // ----------------------------------------------------------------
    printf("\n+ --------------------------------------------------------------- +\n");
    printf("| Resumo das escolhas feitas:                                     |\n");
    printf("+ --------------------------------------------------------------- +\n");
    printf("|  Cliente selecionado: ID %d\n", o.idCliente);

    printf("|  Servicos escolhidos (quantidade: %d): ", qtd_fornecedores);
    for (int i = 0; i < qtd_fornecedores; i++)
        printf("%d ", escolhas_fornecedores[i]);
    printf("\n");

    printf("|  Equipe interna escolhida (quantidade: %d): ", qtd_equipeInterna);
    for (int i = 0; i < qtd_equipeInterna; i++) {
        printf("%d ", escolhas_equipeInterna[i]);
    }
    printf("\n");

    printf("|  Recursos escolhidos (quantidade: %d): ", qtd_recursos);
    for (int i = 0; i < qtd_recursos; i++)
        printf("%d ", escolhas_recursos[i]);
    printf("\n");

    printf("+ --------------------------------------------------------------- +\n");



    
    return o;  // retorna a struct preenchida
}
