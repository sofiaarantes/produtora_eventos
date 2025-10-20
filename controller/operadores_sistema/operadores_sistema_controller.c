#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../model/operadores_sistema/operadores_sistema.h"
#include "../../view/operadores_sistema/operadores_sistema_view.h"
#include "../../model/config_armazenamento/config_armazenamento.h"
#include "../../controller/config_armazenamento/config_armazenamento_controller.h"
#include "../../view/main/main_view.h"
#include "../../model/sessao/sessao.h"
#include "../../util/util.h"

void criptografar_senha(char* senha) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] = senha[i] + 3; // desloca cada caractere 3 posições (Cesar cipher)
    }
}

void gerenciar_login() {
    int opcao;
    do {
        opcao = exibir_menu_operadores();

        if (opcao == 1) { // Login
            Operadores o = ler_dados_operador_login();
            criptografar_senha(o.senha);

            FILE* fp = fopen("operadores.txt", "r");
            if (!fp) {
                printf("\nErro! Tente novamente com outras credenciais.\n");
                continue; // volta ao menu inicial
            }

            Operadores tmp;
            int encontrado = 0;
            while (fscanf(fp, "%d;%49[^;];%49[^;];%19[^;];%d;\n",
                          &tmp.id, tmp.nome, tmp.usuario, tmp.senha, (int*)&tmp.tipo) != EOF) {
                if (strcmp(o.usuario, tmp.usuario) == 0 &&
                    strcmp(o.senha, tmp.senha) == 0) {
                    encontrado = 1;
                    break;
                }
            }
            fclose(fp);

            if (encontrado) {
                printf("\nLogin realizado com sucesso! Bem-vindo(a), %s. Armazenando dados em %s\n", tmp.nome, 
                       tmp.tipo == 1 ? "Memoria" : tmp.tipo == 2 ? "Arquivo Texto" : tmp.tipo == 3 ? "Binario" : "Desconhecido");
                set_operador_logado(tmp.id); 
                set_armazenamento(tmp.tipo);
                return;
            } else {
                printf("\nErro: Credenciais invalidas! Tente novamente.\n");
            }
        } else if (opcao == 2) { // Cadastro
            Operadores novo = ler_dados_operador_cadastro();
            if (novo.tipo == 0) {
                continue;
            }
            criptografar_senha(novo.senha);

            Operadores* cadastrado = adicionar_operador(&novo);
            if (cadastrado == NULL) {
                printf("\nErro: Falha no cadastro. Tente novamente.\n");
                continue; // volta ao menu
            }

            set_operador_logado(cadastrado->id); 
            set_armazenamento(cadastrado->tipo);
            printf("\nCadastro realizado com sucesso! Bem-vindo(a), %s.\n", cadastrado->usuario);
            return;
        } else if ((opcao != 0) && (opcao != 1) && (opcao != 2)){
            printf("\nErro: Opção invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
    exit(0);
}

void editar_operador() {
    // Recebe o id do operador logado na sessão atual
    int id_logado = get_operador_logado();
    // Se o id recebido for -1, significa que houve um erro e não há operador logado
    if (id_logado == -1) {
        printf("\nErro: Nenhum operador logado!\n");
        return;
    }

    // Abre o arquivo de operadores em modo read
    FILE* fp = fopen("operadores.txt", "r");
    if (!fp) {
        printf("\nErro ao abrir arquivo de operadores.\n");
        return;
    }

    // Procura os dados do operador no arquivo, e se for encontrado salva como "atual"
    Operadores operador, atual;
    int encontrado = 0;

    // Buscar operador logado no arquivo
    while (fscanf(fp, "%d;%49[^;];%49[^;];%19[^;];%d;\n",
            &operador.id, operador.nome, operador.usuario, operador.senha, (int*)&operador.tipo) != EOF) {
        if (operador.id == id_logado) {
            encontrado = 1;
            atual = operador;
            break;
        }
    }
    fclose(fp);

    if (!encontrado) {
        printf("\nErro: Operador não encontrado.\n");
        return;
    }

    // Exibir informações do operador atual
    exibir_operador(&atual);

    // Menu de edição
    int opcao;
    do {
        opcao = exibir_menu_operadores_editar();
        if (opcao == 1) {
            // Editar dados
            ler_dados_atualizados_operador(atual.nome, atual.usuario);

            // Atualiza o arquivo
            FILE* original = fopen("operadores.txt", "r");
            FILE* temp = fopen("operadores_temp.txt", "w");
            if (!original || !temp) {
                printf("\nErro ao abrir arquivos para atualização.\n");
                if (original) fclose(original);
                if (temp) fclose(temp);
                return;
            }

            // Lê o arquivo original e busca se há o id do operador atual, se sim ele salva no arquivo os 
            // dados desse operador, se não ele mantém os dados anteriores
            Operadores tmp;
            while (fscanf(original, "%d;%49[^;];%49[^;];%19[^;];%d;\n",
                          &tmp.id, tmp.nome, tmp.usuario, tmp.senha, (int*)&tmp.tipo) != EOF) {
                if (tmp.id == atual.id) {
                    fprintf(temp, "%d;%s;%s;%s;\n",
                            atual.id, atual.nome, atual.usuario, tmp.senha);
                } else {
                    fprintf(temp, "%d;%s;%s;%s;\n",
                            tmp.id, tmp.nome, tmp.usuario, tmp.senha);
                }
            }

            fclose(original);
            fclose(temp);
            remove("operadores.txt");
            rename("operadores_temp.txt", "operadores.txt");

            printf("\nDados atualizados com sucesso!\n");

        } else if (opcao == 2) {
            printf("\n=== Editar Tipo de Armazenamento ===\n");

            int novo_tipo = mostrar_menu_configuracao(); // Mostra o menu e recebe o novo tipo
            if (novo_tipo == 0) {
                printf("\nErro: Operação cancelada.\n");
                return;
            }

            // Atualiza o tipo de armazenamento do operador atual
            atual.tipo = novo_tipo;

            // Abre arquivos para atualizar
            FILE* original = fopen("operadores.txt", "r");
            FILE* temp = fopen("operadores_temp.txt", "w");
            if (!original || !temp) {
                printf("\nErro ao abrir arquivos para atualização.\n");
                if (original) fclose(original);
                if (temp) fclose(temp);
                return;
            }

            Operadores tmp;
            // Copia todos os operadores, substituindo o tipo do operador logado
            while (fscanf(original, "%d;%49[^;];%49[^;];%19[^;];%d;\n",
                        &tmp.id, tmp.nome, tmp.usuario, tmp.senha, (int*)&tmp.tipo) != EOF) {
                if (tmp.id == atual.id) {
                    fprintf(temp, "%d;%s;%s;%s;%d;\n",
                            atual.id, atual.nome, atual.usuario, tmp.senha, atual.tipo);
                } else {
                    fprintf(temp, "%d;%s;%s;%s;%d;\n",
                            tmp.id, tmp.nome, tmp.usuario, tmp.senha, tmp.tipo);
                }
            }

            fclose(original);
            fclose(temp);
            remove("operadores.txt");
            rename("operadores_temp.txt", "operadores.txt");

            // Chama o controlador de armazenamento para atualizar o tipo no sistema
            set_armazenamento((TipoArmazenamento)novo_tipo);

            printf("\nTipo de armazenamento atualizado com sucesso!\n");
        } else if (opcao == 3) {
            // Deletar conta
            int confirmar;
           ler_int("\nTem certeza que deseja deletar sua conta? (1-Sim / 0-Nao): ", &confirmar);
            if (confirmar == 1) {
                deletar_operador(id_logado);
                limpar_sessao();
                printf("\nConta deletada com sucesso! Encerrando sessao...\n");
                gerenciar_login();
                return;
            }
        } else if (opcao == 0) {
            printf("\nVoltando ao menu principal...\n");
        } else {
            printf("\nErro: Opção invalida!\n");
        }
    } while (opcao != 0);
}