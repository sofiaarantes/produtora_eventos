#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"
#include "../../view/cliente/cliente_view.h"
#include "../../view/main/main_view.h"


#define MAX_CLIENTES 100
static Cliente clientes_memoria[MAX_CLIENTES]; // cada posição guarda um cliente
static int qtd = 0; // contador de quantos clientes já estão salvos na memória


Cliente* criar_cliente(Cliente* cliente, TipoArmazenamento tipo) {
    // Se o ponteiro cliente for nulo, nao há como salvar — retorno NULL
    if (!cliente) return NULL;

    // Antes de salvar, garanto que o tipo de documento (CPF ou CNPJ) foi identificado corretamente
    cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);

    // Se o documento for invalido, aviso e encerro a função
    if (cliente->tipo_doc == TIPO_DESCONHECIDO) {
        printf("Erro: documento invalido (%s). Deve ter 11 (CPF) ou 14 (CNPJ) digitos.\n", cliente->cpf_cnpj);
        printf("Cliente NAO salvo.\n");
        return NULL;
    }

    // Começo o ID em 1. Caso haja clientes anteriores, ele será atualizado mais abaixo
    int novo_id = 1;

    // Escolho o tipo de armazenamento que o sistema está usando (memória, texto ou binário)
    switch (tipo) {

        // ============================================
        // CASO 1 - SALVAR CLIENTE NA MEMORIA
        // ============================================
        case MEMORIA: {
            // Verifico se ainda há espaço no array de clientes em memória
            if (qtd < MAX_CLIENTES) {

                // Se já existir pelo menos um cliente, pego o último ID e incremento
                if (qtd > 0)
                    novo_id = clientes_memoria[qtd - 1].id + 1;

                // Atribuo o novo ID ao cliente atual
                cliente->id = novo_id;

                // Recalculo o tipo de documento para garantir consistência
                cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);

                // Salvo o cliente no vetor em memória
                clientes_memoria[qtd] = *cliente;

                // Crio um ponteiro que aponta para o cliente recém-salvo
                Cliente* salvo = &clientes_memoria[qtd];

                // Incremento a contagem total de clientes
                qtd++;

                // Mostro na tela o tipo de documento do cliente salvo
                const char* tipoTexto = (cliente->tipo_doc == TIPO_CPF) ? "CPF" :
                                        (cliente->tipo_doc == TIPO_CNPJ) ? "CNPJ" : "DESCONHECIDO";

                printf("Cliente %s salvo em MEMORIA! Tipo de documento: %s\n", cliente->nome, tipoTexto);

                // Retorno o ponteiro do cliente que acabou de ser salvo
                return salvo;
            } else {
                // Caso o vetor esteja cheio, nao consigo salvar
                printf("Erro: limite de clientes na memória atingido!\n");
                return NULL;
            }
        }


        // ============================================
        // CASO 2 - SALVAR CLIENTE EM ARQUIVO TEXTO
        // ============================================
        case TEXTO: {
            // Abro o arquivo de texto para leitura, para descobrir o último ID usado
            FILE* fp = fopen("clientes.txt", "r");
            if (fp) {
                char linha[512];  // buffer para armazenar cada linha lida
                Cliente tmp;      // struct temporária para ler os dados do arquivo

                // Lê o arquivo linha por linha de forma segura (fgets evita travamentos)
                while (fgets(linha, sizeof(linha), fp)) {
                    // Tenta ler até 9 campos formatados por ponto e vírgula
                    // %[^;] significa “ler até encontrar um ponto e vírgula”
                    int lidos = sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%11[^;];%49[^;];%49[^;];%d",
                                        &tmp.id, tmp.nome, &tmp.idade,
                                        tmp.endereco_completo, tmp.cpf_cnpj,
                                        tmp.tel, tmp.email, tmp.nome_contato,
                                        (int*)&tmp.tipo_doc);

                    // Se conseguiu ler pelo menos o ID, atualiza o próximo id
                    if (lidos >= 1) {
                        novo_id = tmp.id + 1;
                    }
                }
                // Fecha o arquivo após ler todas as linhas
                fclose(fp);
            }

            // Define o novo ID no cliente que será inserido
            cliente->id = novo_id;

            // Recalcula o tipo de documento por segurança
            cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);

            // Agora abre o arquivo em modo append (“a”) para adicionar o novo cliente no final
            fp = fopen("clientes.txt", "a");
            if (!fp) {
                perror("Erro ao abrir clientes.txt");
                return NULL;
            }

            // Grava os dados do cliente em uma linha formatada, separados por ponto e vírgula
            fprintf(fp, "%d;%s;%d;%s;%s;%s;%s;%s;%d\n",
                cliente->id,
                cliente->nome,
                cliente->idade,
                cliente->endereco_completo,
                cliente->cpf_cnpj,
                cliente->tel,
                cliente->email,
                cliente->nome_contato,
                cliente->tipo_doc
            );

            // Fecha o arquivo após a escrita
            fclose(fp);

            // Identifica o tipo do documento para exibir mensagem ao usuário
            const char* tipoTexto = (cliente->tipo_doc == TIPO_CPF) ? "CPF" :
                                    (cliente->tipo_doc == TIPO_CNPJ) ? "CNPJ" : "DESCONHECIDO";

            printf("Cliente %s salvo em TEXTO! Tipo de documento: %s\n", cliente->nome, tipoTexto);

            // Retorna o ponteiro para o cliente criado
            return cliente;
        }


        // ============================================
        // CASO 3 - SALVAR CLIENTE EM ARQUIVO BINARIO
        // ============================================
        case BINARIO: {
            // Abre o arquivo binário em modo leitura ("rb") para descobrir o último ID
            FILE* fp = fopen("clientes.bin", "rb");
            if (fp) {
                Cliente tmp;
                // Lê cliente por cliente até o final do arquivo
                while (fread(&tmp, sizeof(Cliente), 1, fp) == 1) {
                    novo_id = tmp.id + 1; // sempre pega o último ID e soma 1
                }
                fclose(fp);
            }

            // Define o novo ID para o cliente atual
            cliente->id = novo_id;

            // Recalcula o tipo de documento antes de salvar
            cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);

            // Garante que os campos string terminam com '\0'
            cliente->cpf_cnpj[sizeof(cliente->cpf_cnpj) - 1] = '\0';
            cliente->tel[sizeof(cliente->tel) - 1] = '\0';

            // Abre o arquivo binário em modo append binário ("ab")
            fp = fopen("clientes.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return NULL;
            }

            // Escreve o cliente no arquivo em formato binário
            if (fwrite(cliente, sizeof(Cliente), 1, fp) != 1) {
                perror("Erro ao gravar cliente em clientes.bin");
                fclose(fp);
                return NULL;
            }

            // Fecha o arquivo após gravar
            fclose(fp);

            // Exibe mensagem de sucesso com o tipo de documento
            const char* tipoTexto = (cliente->tipo_doc == TIPO_CPF) ? "CPF" :
                                    (cliente->tipo_doc == TIPO_CNPJ) ? "CNPJ" : "DESCONHECIDO";

            printf("Cliente %s salvo em BINARIO! Tipo de documento: %s\n", cliente->nome, tipoTexto);

            // Retorna o ponteiro do cliente que foi salvo
            return cliente;
        }
    }

    // Caso o tipo de armazenamento nao seja reconhecido, retorno NULL
    return NULL;
}



// Função que atualiza um cliente a partir do CPF ou CNPJ informado
// Pode atualizar tanto em memória, quanto em arquivo texto ou binário
Cliente* atualizar_cliente(const char* cpf_cnpj_busca, Cliente* novos_dados, TipoArmazenamento tipo) {

    // Verifica se os parâmetros são válidos
    if (!cpf_cnpj_busca || !novos_dados) return NULL;

    // Escolhe o tipo de armazenamento que será atualizado
    switch (tipo) {

        // ==========================
        // CASO 1: CLIENTES EM MEMORIA
        // ==========================
        case MEMORIA: {
            // Percorro todos os clientes armazenados em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CPF/CNPJ do cliente atual com o que estou buscando
                if (strcmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca) == 0) {

                    // Guardo o tipo de documento original (CPF ou CNPJ)
                    TipoDocumento tipo_doc_original = clientes_memoria[i].tipo_doc;

                    // Substituo o cliente antigo pelos novos dados
                    clientes_memoria[i] = *novos_dados;

                    // Mantenho o CPF/CNPJ original (pois esse é o identificador)
                    strcpy(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca);

                    // Restaura o tipo de documento original
                    clientes_memoria[i].tipo_doc = tipo_doc_original;

                    // Mensagem de sucesso
                    printf(" Cliente %s atualizado em MEMORIA!\n", clientes_memoria[i].nome);

                    // Retorna o endereço do cliente atualizado
                    return &clientes_memoria[i];
                }
            }

            // Caso o cliente nao seja encontrado
            printf(" Cliente com CPF/CNPJ %s nao encontrado em MEMORIA!\n", cpf_cnpj_busca);
            return NULL;
        }

        // ==========================
        // CASO 2: ARQUIVO TEXTO
        // ==========================
        case TEXTO: {
                        // Abre o arquivo de clientes no modo leitura
                        FILE* fp = fopen("clientes.txt", "r");
                        if (!fp) {
                            perror("Erro ao abrir clientes.txt");
                            return NULL;
                        }

                        // Cria um arquivo temporário onde salvará as alterações
                        FILE* temp = fopen("clientes_tmp.txt", "w");
                        if (!temp) {
                            perror("Erro ao criar clientes_tmp.txt");
                            fclose(fp);
                            return NULL;
                        }

                        char linha[400];
                        int atualizado = 0; // flag para verificar se houve atualização

                        // Lê o arquivo linha por linha
                        while (fgets(linha, sizeof(linha), fp)) {
                            Cliente c = {0};

                           
                            int lidos = sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%14[^;];%49[^;];%49[^;];%d",
                                            &c.id, c.nome, &c.idade, c.endereco_completo,
                                            c.cpf_cnpj, c.tel, c.email, c.nome_contato, (int*)&c.tipo_doc);

                            // Se a linha nao tiver o campo tipo_doc (arquivo antigo), forço recalcular
                            if (lidos < 9)
                                c.tipo_doc = identificar_documento(c.cpf_cnpj);

                            // Se o CPF/CNPJ for o que deve ser atualizado
                            if (strcmp(c.cpf_cnpj, cpf_cnpj_busca) == 0) {
                                // Sobrescreve a linha com os novos dados, mantendo ID, CPF/CNPJ e tipo_doc original
                                fprintf(temp, "%d;%s;%d;%s;%s;%s;%s;%s;%d\n",
                                        c.id, //mantem o id original
                                        novos_dados->nome,
                                        novos_dados->idade,
                                        novos_dados->endereco_completo,
                                        c.cpf_cnpj,            // mantém o CPF/CNPJ original
                                        novos_dados->tel,
                                        novos_dados->email,
                                        novos_dados->nome_contato,
                                        c.tipo_doc);           // mantém o tipo_doc original

                                atualizado = 1;
                            } else {
                                // Copia a linha como está
                                fputs(linha, temp);
                            }
                        }

                        fclose(fp);
                        fclose(temp);

                        // Substitui o arquivo antigo pelo novo atualizado
                        remove("clientes.txt");
                        rename("clientes_tmp.txt", "clientes.txt");

                        // Exibe mensagem de resultado
                        if (atualizado) {
                            printf("Cliente %s atualizado em TEXTO!\n", novos_dados->nome);
                            return novos_dados;
                        } else {
                            printf("Cliente com CPF/CNPJ %s nao encontrado em TEXTO!\n", cpf_cnpj_busca);
                            return NULL;
                        }
                    }

        // ==========================
        // CASO 3: ARQUIVO BINARIO
        // ==========================
        case BINARIO: {
            // Abro o arquivo binário no modo leitura/escrita
            FILE* fp = fopen("clientes.bin", "rb+");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return NULL;
            }

            Cliente c;
            // Leio cada cliente do arquivo
            while (fread(&c, sizeof(Cliente), 1, fp)) {
                // Comparo o CPF/CNPJ
                if (strcmp(c.cpf_cnpj, cpf_cnpj_busca) == 0) {
                    // Volto o ponteiro do arquivo para sobrescrever o registro atual
                    fseek(fp, -(long)sizeof(Cliente), SEEK_CUR);

                    // Mantenho o CPF/CNPJ original
                    strcpy(novos_dados->cpf_cnpj, cpf_cnpj_busca);

                    // Também preservo o tipo de documento (CPF/CNPJ)
                    novos_dados->tipo_doc = c.tipo_doc;

                    // Sobrescrevo os dados do cliente
                    fwrite(novos_dados, sizeof(Cliente), 1, fp);

                    // Fecho o arquivo e retorno
                    fclose(fp);
                    printf("Cliente %s atualizado em BINARIO!\n", novos_dados->nome);
                    return novos_dados;
                }
            }

            // Caso nao encontre o cliente
            fclose(fp);
            printf(" Cliente com CPF/CNPJ %s nao encontrado em BINARIO!\n", cpf_cnpj_busca);
            return NULL;
        }

        // ==========================
        // CASO INVÁLIDO
        // ==========================
        default:
            printf("Tipo de armazenamento invalido!\n");
            return NULL;
    }
}




//------------------------------------

// Retorna quantos clientes já estão salvos em memória
// Aqui eu só quero saber a quantidade de clientes que já cadastrei na memória
int get_qtd_clientes() { 
    return qtd; // retorno o valor do contador, que tá sendo incrementado toda vez que salvo um cliente
} 
//-----------------------------
// Função para buscar e exibir diretamente um cliente pelo CPF/CNPJ
void buscar_e_exibir_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo) {
    // Verifico se o CPF/CNPJ passado é nulo (invalido)
    if (!cpf_cnpj_busca) {
        exibir_mensagem("+--------------------------+\n");
        exibir_mensagem("| CPF/CNPJ invalido!       |\n");
        exibir_mensagem("+--------------------------+\n");
        return; // se for invalido, saio da função
    }

    Cliente* cliente = NULL; // Ponteiro que vai armazenar o cliente encontrado, se existir

    // Escolho o tipo de armazenamento usado
    switch (tipo) {

        // =========================
        // Caso 1: busca em memória
        // =========================
        case MEMORIA: {
            // Percorro o array de clientes em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CPF/CNPJ de cada cliente com o CPF/CNPJ digitado
                if (strncmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca, sizeof(clientes_memoria[i].cpf_cnpj)) == 0) {
                    // Se encontrar, guardo o endereço do cliente encontrado
                    cliente = &clientes_memoria[i];
                    // Exibo as informações desse cliente
                    ver_cliente(cliente);
                    break; // paro o loop, pois já encontrei o cliente
                }
            }
            break;
        }

        // ==============================
        // Caso 2: busca em arquivo texto
        // ==============================
        case TEXTO: {
            // Abro o arquivo texto para leitura
            FILE* fp = fopen("clientes.txt", "r");
            if (!fp) {
                // Se nao conseguir abrir, mostro o erro e saio
                perror("Erro ao abrir clientes.txt");
                return;
            }

            // Aloco memória para armazenar temporariamente um cliente
            cliente = malloc(sizeof(Cliente));
            char linha[512]; // buffer para armazenar cada linha do arquivo

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Extraio os campos separados por ponto e vírgula
                sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%11[^;];%49[^;];%49[^;];%d",
                    &cliente->id,
                    cliente->nome,
                    &cliente->idade,
                    cliente->endereco_completo,
                    cliente->cpf_cnpj,
                    cliente->tel,
                    cliente->email,
                    cliente->nome_contato,
                    (int*)&cliente->tipo_doc   // <-- adiciona esta leitura!
                );


                // Comparo o CPF lido com o CPF buscado
                if (strncmp(cliente->cpf_cnpj, cpf_cnpj_busca, sizeof(cliente->cpf_cnpj)) == 0) {
                    // Se encontrar, fecho o arquivo e exibo o cliente
                    fclose(fp);
                    ver_cliente(cliente);
                    free(cliente); // libero a memória alocada
                    return; // retorno, pois já encontrei o cliente
                }
            }

            // Se chegar aqui, o cliente nao foi encontrado
            fclose(fp);   // fecho o arquivo
            free(cliente); // libero a memória usada
            cliente = NULL; // deixo o ponteiro nulo
            break;
        }

        // =================================
        // Caso 3: busca em arquivo binário
        // =================================
        case BINARIO: {
            // Abro o arquivo binário para leitura
            FILE* fp = fopen("clientes.bin", "rb");
            if (!fp) {
                // Se nao conseguir abrir, mostro o erro e saio
                perror("Erro ao abrir clientes.bin");
                return;
            }

            // Aloco memória para armazenar temporariamente um cliente
            cliente = malloc(sizeof(Cliente));

            // Leio o arquivo binário cliente por cliente
            while (fread(cliente, sizeof(Cliente), 1, fp) == 1) {
                // Garante que a string do CPF termina em '\0'
                cliente->cpf_cnpj[sizeof(cliente->cpf_cnpj)-1] = '\0';

                // Comparo o CPF lido com o CPF buscado
                if (strncmp(cliente->cpf_cnpj, cpf_cnpj_busca, sizeof(cliente->cpf_cnpj)) == 0) {
                    // Se encontrar, fecho o arquivo e exibo as informações
                    fclose(fp);
                    ver_cliente(cliente);
                    free(cliente); // libero a memória alocada
                    return; // retorno, pois já encontrei o cliente
                }
            }

            // Se terminar o arquivo e nao encontrar, libero tudo
            fclose(fp);
            free(cliente);
            cliente = NULL;
            break;
        }

        // Caso o tipo de armazenamento nao seja reconhecido
        default:
            exibir_mensagem("Tipo de armazenamento invalido!\n");
            return;
    }

    // Se nenhum cliente foi encontrado nos casos acima, mostro mensagem padrão
    if (!cliente) {
        printf("+--------------------------+\n");
        printf("| Cliente nao encontrado!  |\n");
        printf("+--------------------------+\n");
    }
}
// Função para deletar um cliente a partir do CPF/CNPJ e do tipo de armazenamento
void deletar_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo) {
    // Verifico se o parâmetro cpf_cnpj_busca é nulo antes de continuar
    if (!cpf_cnpj_busca) {
        printf("CPF/CNPJ invalido!\n");
        return;
    }

    // Uso o switch para tratar cada tipo de armazenamento (memória, texto e binário)
    switch (tipo) {

        // =====================================================
        // Caso 1 — Deletar cliente armazenado em MEMORIA
        // =====================================================
        case MEMORIA: {
            int encontrado = 0; // variável para controlar se o cliente foi achado

            // Percorro o vetor global de clientes armazenados em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CPF/CNPJ do cliente atual com o CPF digitado
                if (strncmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca, sizeof(clientes_memoria[i].cpf_cnpj)) == 0) {

                    // Quando encontro o cliente, preciso removê-lo deslocando os próximos para trás
                    for (int j = i; j < qtd - 1; j++) {
                        clientes_memoria[j] = clientes_memoria[j + 1];
                    }

                    // Reduzo a quantidade total de clientes armazenados
                    qtd--;

                    // Marco que encontrei o cliente
                    encontrado = 1;
                    printf("Cliente %s deletado em MEMORIA!\n", clientes_memoria[i].nome);
                    break; // saio do laço, já deletei
                }
            }

            // Caso o cliente nao tenha sido encontrado, aviso o usuário
            if (!encontrado)
                 printf("Cliente com CPF/CNPJ %s nao encontrado em MEMORIA!\n", cpf_cnpj_busca);

            break;
        }

        // =====================================================
        // Caso 2 — Deletar cliente de ARQUIVO TEXTO
        // =====================================================
        case TEXTO: {
            // Abro o arquivo de clientes em modo leitura
            FILE* fp = fopen("clientes.txt", "r");
            if (!fp) {
                perror("Erro ao abrir clientes.txt");
                return;
            }

            // Crio um arquivo temporário para regravar os dados válidos
            FILE* temp = fopen("temp.txt", "w");
            if (!temp) {
                perror("Erro ao criar temp.txt");
                fclose(fp);
                return;
            }

            char linha[512];   // buffer para ler linha por linha do arquivo
            int deletado = 0;  // controle se o cliente foi removido
            Cliente cliente;   // struct temporária para armazenar os dados lidos

            // Leio o arquivo original linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Uso sscanf para quebrar os dados lidos separados por ';'
                sscanf(linha, "%d;%49[^;];%d;%99[^;];%11[^;];%11[^;];%49[^;];%49[^\n]",
                    &cliente.id,
                    cliente.nome,
                    &cliente.idade,
                    cliente.endereco_completo,
                    cliente.cpf_cnpj,
                    cliente.tel,
                    cliente.email,
                    cliente.nome_contato
                );

                // Se o CPF nao for o que quero excluir, regravo essa linha no arquivo temporário
                if (strncmp(cliente.cpf_cnpj, cpf_cnpj_busca, sizeof(cliente.cpf_cnpj)) != 0) {
                    fprintf(temp, "%d;%s;%d;%s;%s;%s;%s;%s\n",
                        cliente.id,
                        cliente.nome,
                        cliente.idade,
                        cliente.endereco_completo,
                        cliente.cpf_cnpj,
                        cliente.tel,
                        cliente.email,
                        cliente.nome_contato
                    );
                } else {
                    // Caso o CPF seja o que quero deletar, apenas marco a exclusão
                    deletado = 1;
                }
            }

            // Fecho os arquivos abertos
            fclose(fp);
            fclose(temp);

            // Excluo o arquivo antigo e renomeio o temporário como o novo "clientes.txt"
            remove("clientes.txt");
            rename("temp.txt", "clientes.txt");
            
            // Exibo mensagem informando o resultado da operação
            if (deletado)
                printf("Cliente %s deletado em ARQUIVO TEXTO!\n", cliente.nome);
            else
                printf("Cliente com CPF/CNPJ %s nao encontrado no ARQUIVO TEXTO!\n", cpf_cnpj_busca);

            break;
        }

        // =====================================================
        // Caso 3 — Deletar cliente de ARQUIVO BINARIO
        // =====================================================
        case BINARIO: {
            // Abro o arquivo binário original em modo leitura binária
            FILE* fp = fopen("clientes.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return;
            }

            // Crio um arquivo temporário para regravar os clientes que continuarão existindo
            FILE* temp = fopen("temp.bin", "wb");
            if (!temp) {
                perror("Erro ao criar temp.bin");
                fclose(fp);
                return;
            }

            Cliente cliente;   // struct temporária para armazenar o cliente lido
            int deletado = 0;  // controle se algum cliente foi realmente removido

            // Leio cada cliente do arquivo binário original
            while (fread(&cliente, sizeof(Cliente), 1, fp) == 1) {
                // Garanto que o CPF termina com '\0' para evitar erro de comparação
                cliente.cpf_cnpj[sizeof(cliente.cpf_cnpj) - 1] = '\0';

                // Se o CPF nao for o que deve ser deletado, gravo esse cliente no arquivo temporário
                if (strncmp(cliente.cpf_cnpj, cpf_cnpj_busca, sizeof(cliente.cpf_cnpj)) != 0) {
                    fwrite(&cliente, sizeof(Cliente), 1, temp);
                } else {
                    // Caso o CPF seja o do cliente a excluir, apenas marco que deletei
                    deletado = 1;
                }
            }

            // Fecho os arquivos abertos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo temporário atualizado
            remove("clientes.bin");
            rename("temp.bin", "clientes.bin");

            // Exibo mensagens de status ao usuário
            if (deletado)
                 printf("Cliente %s deletado em ARQUIVO BINARIO!\n", cliente.nome);
            else
                printf("Cliente com CPF/CNPJ %s nao encontrado no ARQUIVO BINARIO!\n", cpf_cnpj_busca);

            break;
        }

        // =====================================================
        // Caso padrão — tipo de armazenamento invalido
        // =====================================================
        default:
            printf("Tipo de armazenamento invalido!\n");
            break;
    }
}

TipoDocumento identificar_documento(const char *doc) {
    int len = 0;

    // Conta apenas os dígitos
    for (int i = 0; doc[i] != '\0'; i++) {
        if (isdigit((unsigned char)doc[i])) {
            len++;
        }
    }

    // Garante que nao ultrapasse o tamanho máximo esperado
    if (len == 11) {
        return TIPO_CPF;
    } else if (len == 14) {
        return TIPO_CNPJ;
    } else {
        return TIPO_DESCONHECIDO;
    }
}

// Lista todos os clientes do tipo especificado. Retorna array alocado e seta out_count.
// Se não houver registros, retorna NULL e out_count = 0.
Cliente* listar_todos_clientes(TipoArmazenamento tipo, int* out_count) {
    if (!out_count) return NULL;
    *out_count = 0;

    switch (tipo) {
        case MEMORIA: {
            if (qtd == 0) return NULL;
            // Aloca um array com exatamente qtd elementos
            Cliente* arr = malloc(sizeof(Cliente) * qtd);
            if (!arr) return NULL;
            for (int i = 0; i < qtd; i++) arr[i] = clientes_memoria[i];
            *out_count = qtd;
            return arr;
        }
        case TEXTO: {
            FILE* fp = fopen("clientes.txt", "r");
            if (!fp) return NULL;
            // Leitura dinâmica: primeiro conta linhas / registros
            int count = 0;
            char linha[512];
            while (fgets(linha, sizeof(linha), fp)) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            Cliente* arr = malloc(sizeof(Cliente) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fgets(linha, sizeof(linha), fp) && idx < count) {
                Cliente c = {0};
                sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%11[^;];%49[^;];%49[^;];%d",
                    &c.id, c.nome, &c.idade, c.endereco_completo,
                    c.cpf_cnpj, c.tel, c.email, c.nome_contato, (int*)&c.tipo_doc);
                // garante terminação
                c.cpf_cnpj[sizeof(c.cpf_cnpj)-1] = '\0';
                arr[idx++] = c;
            }
            fclose(fp);
            *out_count = idx;
            return arr;
        }
        case BINARIO: {
            FILE* fp = fopen("clientes.bin", "rb");
            if (!fp) return NULL;
            // Descobre quantidade lendo o arquivo
            int count = 0;
            Cliente tmp;
            while (fread(&tmp, sizeof(Cliente), 1, fp) == 1) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            Cliente* arr = malloc(sizeof(Cliente) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fread(&arr[idx], sizeof(Cliente), 1, fp) == 1) {
                // garante terminação de strings
                arr[idx].cpf_cnpj[sizeof(arr[idx].cpf_cnpj)-1] = '\0';
                idx++;
            }
            fclose(fp);
            *out_count = idx;
            return arr;
        }
        default:
            return NULL;
    }
}

// Remove todos os registros do armazenamento especificado. Retorna 1 em sucesso, 0 caso contrário.
int limpar_clientes(TipoArmazenamento tipo) {
    switch (tipo) {
        case MEMORIA:
            qtd = 0;
            // opcional: zerar array
            // memset(clientes_memoria, 0, sizeof(clientes_memoria));
            return 1;
        case TEXTO:
            remove("clientes.txt"); // ignora erro se não existir
            return 1;
        case BINARIO:
            remove("clientes.bin");
            return 1;
        default:
            return 0;
    }
}