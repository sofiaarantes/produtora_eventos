#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"
#include "../../view/cliente/cliente_view.h"
#include "../../view/main/main_view.h"
#include "../../model/sessao/sessao.h"


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

    cliente->id_logado = get_operador_logado(); // Associo o cliente ao operador logado

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

                cliente->id_logado = get_operador_logado(); // garante que está atualizado

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
                    int lidos = sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%11[^;];%49[^;];%49[^;];%d;%d",
                                        &tmp.id, tmp.nome, &tmp.idade,
                                        tmp.endereco_completo, tmp.cpf_cnpj,
                                        tmp.tel, tmp.email, tmp.nome_contato,
                                        (int*)&tmp.tipo_doc,&tmp.id_logado);

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

            cliente->id_logado = get_operador_logado(); // Associo o cliente ao operador logado

            // Agora abre o arquivo em modo append (“a”) para adicionar o novo cliente no final
            fp = fopen("clientes.txt", "a");
            if (!fp) {
                perror("Erro ao abrir clientes.txt");
                return NULL;
            }

            // Grava os dados do cliente em uma linha formatada, separados por ponto e vírgula
            fprintf(fp, "%d;%s;%d;%s;%s;%s;%s;%s;%d;%d\n",
                cliente->id,
                cliente->nome,
                cliente->idade,
                cliente->endereco_completo,
                cliente->cpf_cnpj,
                cliente->tel,
                cliente->email,
                cliente->nome_contato,
                cliente->tipo_doc,
                cliente->id_logado
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

            cliente->id_logado = get_operador_logado(); // Associo o cliente ao operador logado
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

    // Verifico se os parâmetros passados são válidos antes de qualquer coisa
    if (!cpf_cnpj_busca || !novos_dados) return NULL;

    // Pego o ID do operador que está logado no momento (para verificar permissões)
    int operador_atual = get_operador_logado();

    // Uso um switch para tratar de acordo com o tipo de armazenamento escolhido
    switch (tipo) {

        // =======================================================
        // CASO 1: Atualização de cliente armazenado na MEMÓRIA
        // =======================================================
        case MEMORIA: {
            // Percorro todos os clientes armazenados na memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CPF/CNPJ atual com o que foi informado pelo usuário
                if (strcmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca) == 0) {

                    // Verifico se o operador logado é o mesmo que criou esse cliente
                    if (clientes_memoria[i].id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para atualizar este cliente \n");
                        return NULL; // se não for o mesmo, não permito a atualização
                    }

                    // Guardo o tipo de documento e o id_logado original
                    TipoDocumento tipo_doc_original = clientes_memoria[i].tipo_doc;
                    int id_logado_original = clientes_memoria[i].id_logado;

                    // Substituo o cliente antigo pelos novos dados
                    clientes_memoria[i] = *novos_dados;

                    // Mas garanto que o CPF/CNPJ, tipo_doc e id_logado originais não sejam alterados
                    strcpy(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca);
                    clientes_memoria[i].tipo_doc = tipo_doc_original;
                    clientes_memoria[i].id_logado = id_logado_original;

                    // Aviso que o cliente foi atualizado
                    printf("Cliente %s atualizado em MEMORIA!\n", clientes_memoria[i].nome);

                    // Retorno o ponteiro para o cliente atualizado
                    return &clientes_memoria[i];
                }
            }

            // Se chegar aqui, é porque o CPF/CNPJ não foi encontrado
            printf("Cliente com CPF/CNPJ %s nao encontrado em MEMORIA!\n", cpf_cnpj_busca);
            return NULL;
        }

        // =======================================================
        // CASO 2: Atualização de cliente armazenado em ARQUIVO TEXTO
        // =======================================================
        case TEXTO: {
            // Abro o arquivo original de clientes em modo leitura
            FILE* fp = fopen("clientes.txt", "r");
            if (!fp) {
                perror("Erro ao abrir clientes.txt");
                return NULL;
            }

            // Crio um arquivo temporário onde vou escrever as atualizações
            FILE* temp = fopen("clientes_tmp.txt", "w");
            if (!temp) {
                perror("Erro ao criar clientes_tmp.txt");
                fclose(fp);
                return NULL;
            }

            char linha[400]; // buffer para ler as linhas do arquivo
            int atualizado = 0; // flag para indicar se algum cliente foi atualizado

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                Cliente c = {0};

                // Quebro a linha em campos separados por ';'
                int lidos = sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%14[^;];%49[^;];%49[^;];%d;%d",
                                   &c.id, c.nome, &c.idade, c.endereco_completo,
                                   c.cpf_cnpj, c.tel, c.email, c.nome_contato,
                                   (int*)&c.tipo_doc, (int*)&c.id_logado);

                // Caso o tipo_doc não tenha sido lido corretamente, calculo de novo
                if (lidos < 9)
                    c.tipo_doc = identificar_documento(c.cpf_cnpj);

                // Comparo o CPF/CNPJ lido com o buscado
                if (strcmp(c.cpf_cnpj, cpf_cnpj_busca) == 0) {

                    // Confiro se o operador logado tem permissão para atualizar esse cliente
                    if (c.id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para atualizar este cliente\n");
                        fclose(fp);
                        fclose(temp);
                        remove("clientes_tmp.txt"); // removo o arquivo temporário, já que deu erro
                        return NULL; // e saio direto
                    }

                    // Aqui sobrescrevo a linha do cliente com os novos dados
                    // mantendo CPF/CNPJ, tipo_doc e id_logado originais
                    fprintf(temp, "%d;%s;%d;%s;%s;%s;%s;%s;%d;%d\n",
                            c.id,
                            novos_dados->nome,
                            novos_dados->idade,
                            novos_dados->endereco_completo,
                            c.cpf_cnpj,
                            novos_dados->tel,
                            novos_dados->email,
                            novos_dados->nome_contato,
                            c.tipo_doc,
                            c.id_logado);

                    atualizado = 1; // marco que o cliente foi atualizado
                } else {
                    // Se não for o cliente buscado, só copio a linha normalmente
                    fputs(linha, temp);
                }
            }

            // Fecho os dois arquivos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo novo com as atualizações
            remove("clientes.txt");
            rename("clientes_tmp.txt", "clientes.txt");

            // Exibo mensagem de acordo com o resultado
            if (atualizado) {
                printf("Cliente %s atualizado em TEXTO!\n", novos_dados->nome);
                return novos_dados;
            } else {
                printf("Cliente com CPF/CNPJ %s nao encontrado em TEXTO!\n", cpf_cnpj_busca);
                return NULL;
            }
        }

        // =======================================================
        // CASO 3: Atualização de cliente armazenado em ARQUIVO BINÁRIO
        // =======================================================
        case BINARIO: {
            // Abro o arquivo binário para leitura e escrita
            FILE* fp = fopen("clientes.bin", "rb+");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return NULL;
            }

            Cliente c;
            int encontrado = 0; // flag para marcar se encontrei o cliente

            // Leio cliente por cliente no arquivo
            while (fread(&c, sizeof(Cliente), 1, fp) == 1) {
                // Garante que o CPF/CNPJ está terminado corretamente com '\0'
                c.cpf_cnpj[sizeof(c.cpf_cnpj) - 1] = '\0';

                // Comparo o CPF/CNPJ lido com o buscado
                if (strcmp(c.cpf_cnpj, cpf_cnpj_busca) == 0) {
                    encontrado = 1;

                    // Verifico se o operador logado é o mesmo que criou o cliente
                    if (c.id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para atualizar este cliente\n");
                        fclose(fp);
                        return NULL; // retorno imediatamente, sem exibir "não encontrado"
                    }

                    // Volto o cursor do arquivo para reescrever esse mesmo cliente
                    fseek(fp, -(long)sizeof(Cliente), SEEK_CUR);

                    // Copio os dados fixos que não podem ser alterados
                    novos_dados->id = c.id;
                    strcpy(novos_dados->cpf_cnpj, c.cpf_cnpj);
                    novos_dados->tipo_doc = c.tipo_doc;
                    novos_dados->id_logado = c.id_logado;

                    // Gravo os novos dados no arquivo (sobrescrevendo o cliente antigo)
                    if (fwrite(novos_dados, sizeof(Cliente), 1, fp) != 1) {
                        perror("Erro ao escrever cliente atualizado em clientes.bin");
                        fclose(fp);
                        return NULL;
                    }

                    // Fecho o arquivo e informo o sucesso
                    fclose(fp);
                    printf("Cliente %s atualizado em BINARIO!\n", novos_dados->nome);
                    return novos_dados;
                }
            }

            // Fecho o arquivo caso o cliente não tenha sido encontrado
            fclose(fp);

            // Se não encontrei o cliente, aviso o usuário
            if (!encontrado) {
                printf("Cliente com CPF/CNPJ %s nao encontrado em BINARIO!\n", cpf_cnpj_busca);
            }

            return NULL;
        }

        // =======================================================
        // Caso padrão — tipo de armazenamento inválido
        // =======================================================
        default:
            printf("Tipo de armazenamento invalido!\n");
            return NULL;
    }
}


//-----------------------------
void buscar_e_exibir_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo) {

    // --------------------------------------------------------
    // Primeiro, verifico se o parâmetro CPF/CNPJ é válido.
    // Se for NULL (nulo), não adianta prosseguir — o usuário
    // provavelmente esqueceu de digitar o documento.
    // --------------------------------------------------------
    if (!cpf_cnpj_busca) {
        exibir_mensagem("+--------------------------+\n");
        exibir_mensagem("| CPF/CNPJ invalido!       |\n");
        exibir_mensagem("+--------------------------+\n");
        return; // interrompo a função aqui
    }

    // --------------------------------------------------------
    // Capturo o ID do operador logado (quem está usando o sistema)
    // para controlar as permissões de visualização.
    // Só pode visualizar o cliente quem o criou (id_logado igual).
    // --------------------------------------------------------
    int operador_atual = get_operador_logado();

    // --------------------------------------------------------
    // Crio um ponteiro para Cliente que usarei conforme o tipo
    // de armazenamento (pode ser carregado da memória, texto ou binário).
    // --------------------------------------------------------
    Cliente* cliente = NULL;

    // --------------------------------------------------------
    // Agora, começo a verificar qual tipo de armazenamento
    // o sistema está usando no momento (switch case).
    // --------------------------------------------------------
    switch (tipo) {

        // =====================================================
        // Caso 1: Busca em memória (dados já carregados no vetor)
        // =====================================================
        case MEMORIA: {
            // Percorro o vetor global 'clientes_memoria' até a quantidade cadastrada
            for (int i = 0; i < qtd; i++) {
                // Comparo o CPF/CNPJ de cada cliente com o que foi buscado
                if (strcmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca) == 0) {

                    // Se encontrar o cliente, verifico se pertence ao operador logado
                    if (clientes_memoria[i].id_logado != operador_atual) {
                        // Se for de outro operador, não pode visualizar
                        printf("Erro: voce nao tem permissao para visualizar este cliente.\n");
                        return; // Encerro imediatamente
                    }

                    // Se o operador for o dono, exibo os dados
                    ver_cliente(&clientes_memoria[i]);
                    return; // Saio da função pois já encontrei
                }
            }

            // Se percorrer todo o vetor e não encontrar o cliente, informo isso
            printf("+--------------------------+\n");
            printf("| Cliente nao encontrado!  |\n");
            printf("+--------------------------+\n");
            break; // Termina o case MEMORIA
        }

        // =====================================================
        // Caso 2: Busca em arquivo texto (clientes.txt)
        // =====================================================
        case TEXTO: {
            // Tento abrir o arquivo texto para leitura
            FILE* fp = fopen("clientes.txt", "r");
            if (!fp) {
                // Se não conseguir abrir, mostro erro do sistema (perror)
                perror("Erro ao abrir clientes.txt");
                return;
            }

            // Aloco memória temporária para armazenar os dados lidos do arquivo
            cliente = malloc(sizeof(Cliente));
            char linha[512]; // buffer para ler cada linha do arquivo

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Cada linha contém os dados separados por ';'
                // Uso sscanf para extrair os campos para a estrutura Cliente
                sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%14[^;];%49[^;];%49[^;];%d;%d",
                       &cliente->id,
                       cliente->nome,
                       &cliente->idade,
                       cliente->endereco_completo,
                       cliente->cpf_cnpj,
                       cliente->tel,
                       cliente->email,
                       cliente->nome_contato,
                       (int*)&cliente->tipo_doc,
                       &cliente->id_logado);

                // Comparo o CPF/CNPJ lido com o buscado
                if (strcmp(cliente->cpf_cnpj, cpf_cnpj_busca) == 0) {

                    // Verifico se o operador logado é o dono do cliente
                    if (cliente->id_logado != operador_atual) {
                        // Se não for, o sistema bloqueia a visualização
                        printf("Erro: voce nao tem permissao para visualizar este cliente.\n");
                        fclose(fp);
                        free(cliente);
                        return;
                    }

                    // Se tiver permissão, exibo os dados do cliente
                    ver_cliente(cliente);
                    fclose(fp);
                    free(cliente);
                    return; // Já encontrei, então retorno
                }
            }

            // Se chegou aqui, é porque leu o arquivo todo e não achou o CPF/CNPJ
            fclose(fp);
            free(cliente);
            cliente = NULL; // segurança: aponto para NULL após liberar memória

            printf("+--------------------------+\n");
            printf("| Cliente nao encontrado!  |\n");
            printf("+--------------------------+\n");

            break;
        }

        // =====================================================
        // Caso 3: Busca em arquivo binário (clientes.bin)
        // =====================================================
        case BINARIO: {
            // Tento abrir o arquivo binário em modo leitura
            FILE* fp = fopen("clientes.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return;
            }

            // Aloco um cliente temporário para armazenar os dados lidos do binário
            cliente = malloc(sizeof(Cliente));

            // Leio o arquivo cliente por cliente
            while (fread(cliente, sizeof(Cliente), 1, fp) == 1) {
                // Garante que a string CPF/CNPJ termina com '\0'
                cliente->cpf_cnpj[sizeof(cliente->cpf_cnpj) - 1] = '\0';

                // Comparo com o CPF/CNPJ buscado
                if (strcmp(cliente->cpf_cnpj, cpf_cnpj_busca) == 0) {

                    // Verifico se o operador logado é o dono do cliente
                    if (cliente->id_logado != operador_atual) {
                        // Se não for, mostro erro e não permito exibir
                        printf("Erro: voce nao tem permissao para visualizar este cliente.\n");
                        fclose(fp);
                        free(cliente);
                        return;
                    }

                    // Se o operador for o dono, exibo os dados normalmente
                    ver_cliente(cliente);
                    fclose(fp);
                    free(cliente);
                    return;
                }
            }

            // Se o loop terminou e não encontrou o cliente, aviso o usuário
            fclose(fp);
            free(cliente);
            cliente = NULL;

            printf("+--------------------------+\n");
            printf("| Cliente nao encontrado!  |\n");
            printf("+--------------------------+\n");

            break;
        }

        // =====================================================
        // Caso padrão (tipo de armazenamento inválido)
        // =====================================================
        default:
            exibir_mensagem("Tipo de armazenamento invalido!\n");
            return;
    }
}

// Função para deletar um cliente a partir do CPF/CNPJ e do tipo de armazenamento
void deletar_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo) {
    // Primeiro verifico se o parâmetro CPF/CNPJ recebido é válido
    // Se for nulo, já mostro uma mensagem de erro e saio da função.
    if (!cpf_cnpj_busca) {
        printf("CPF/CNPJ invalido!\n");
        return;
    }

    // Pego o ID do operador atualmente logado (para checar permissões)
    int operador_atual = get_operador_logado();

    // Uso o switch para tratar cada tipo de armazenamento (memória, texto ou binário)
    switch (tipo) {

        // =====================================================
        // Caso 1 — Deletar cliente armazenado em MEMÓRIA
        // =====================================================
        case MEMORIA: {
            // Percorro o vetor global de clientes armazenados em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CPF/CNPJ digitado com o do cliente atual
                if (strcmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca) == 0) {
                    // Achei o cliente, agora verifico se o operador atual tem permissão
                    if (clientes_memoria[i].id_logado != operador_atual) {
                        // Se o ID logado for diferente, mostro a mensagem e saio da função
                        printf("Erro: voce nao tem permissao para deletar este cliente.\n");
                        return;
                    }

                    // Se tiver permissão, preciso remover o cliente do vetor
                    // Para isso, desloco todos os próximos elementos uma posição para trás
                    for (int j = i; j < qtd - 1; j++) {
                        clientes_memoria[j] = clientes_memoria[j + 1];
                    }

                    // Reduzo a contagem total de clientes em memória
                    qtd--;

                    // Exibo mensagem de sucesso e saio da função
                    printf("Cliente %s deletado em MEMORIA!\n",clientes_memoria[i].nome);
                    return;
                }
            }

            // Se cheguei até aqui, o CPF/CNPJ não foi encontrado
            printf("Cliente com CPF/CNPJ %s nao encontrado em MEMORIA!\n", cpf_cnpj_busca);
            return;
        }

        // =====================================================
        // Caso 2 — Deletar cliente em ARQUIVO TEXTO (.txt)
        // =====================================================
        case TEXTO: {
            // Abro o arquivo de clientes em modo leitura
            FILE* fp = fopen("clientes.txt", "r");
            if (!fp) {
                perror("Erro ao abrir clientes.txt");
                return;
            }

            // Crio um arquivo temporário onde vou regravar apenas os clientes válidos
            FILE* temp = fopen("temp.txt", "w");
            if (!temp) {
                perror("Erro ao criar temp.txt");
                fclose(fp);
                return;
            }

            char linha[512];    // Buffer usado para ler cada linha do arquivo
            Cliente cliente;    // Struct temporária para armazenar os dados lidos
            int deletado = 0;   // Variável de controle para saber se excluí algum cliente

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Tento extrair todos os campos, incluindo tipo_doc e id_logado
                 sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%14[^;];%49[^;];%49[^;];%d;%d",
                                   &cliente.id,
                                   cliente.nome,
                                   &cliente.idade,
                                   cliente.endereco_completo,
                                   cliente.cpf_cnpj,
                                   cliente.tel,
                                   cliente.email,
                                   cliente.nome_contato,
                                   (int*)&cliente.tipo_doc,
                                   &cliente.id_logado);

                // Verifico se esta linha pertence ao cliente que quero deletar
                if (strcmp(cliente.cpf_cnpj, cpf_cnpj_busca) == 0) {
                    // Se achei o cliente, verifico a permissão
                    if (cliente.id_logado != operador_atual) {
                        // Sem permissão: apago o arquivo temporário e retorno imediatamente
                        printf("Erro: voce nao tem permissao para deletar este cliente.\n");
                        fclose(fp);
                        fclose(temp);
                        remove("temp.txt");
                        return;
                    } else {
                        // Se tiver permissão, simplesmente não escrevo essa linha no arquivo temporário
                        // Isso faz com que o cliente seja removido
                        deletado = 1;
                        continue; // Pulo para a próxima linha
                    }
                }

                // Caso não seja o cliente que quero excluir, regravo a linha normalmente
                fputs(linha, temp);
            }

            // Fecho os arquivos originais e temporários
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo novo (sem o cliente deletado)
            remove("clientes.txt");
            rename("temp.txt", "clientes.txt");

            // Se o cliente foi encontrado e deletado, exibo sucesso
            if (deletado) {
                printf("Cliente %s deletado em ARQUIVO TEXTO!\n",cliente.nome);
            } else {
                // Caso contrário, o cliente não existia
                printf("Cliente com CPF/CNPJ %s nao encontrado em TEXTO!\n", cpf_cnpj_busca);
            }
            return;
        }

        // =====================================================
        // Caso 3 — Deletar cliente em ARQUIVO BINÁRIO (.bin)
        // =====================================================
        case BINARIO: {
            // Abro o arquivo binário original em modo leitura
            FILE* fp = fopen("clientes.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return;
            }

            // Crio um arquivo temporário onde vou regravar apenas os clientes válidos
            FILE* temp = fopen("temp.bin", "wb");
            if (!temp) {
                perror("Erro ao criar temp.bin");
                fclose(fp);
                return;
            }

            Cliente cliente;   // Struct temporária usada para leitura
            int deletado = 0;  // Controle de exclusão bem-sucedida

            // Leio cada cliente do arquivo binário
            while (fread(&cliente, sizeof(Cliente), 1, fp) == 1) {
                // Garante que o CPF termine corretamente
                cliente.cpf_cnpj[sizeof(cliente.cpf_cnpj) - 1] = '\0';

                // Comparo o CPF lido com o CPF digitado
                if (strcmp(cliente.cpf_cnpj, cpf_cnpj_busca) == 0) {
                    // Achei o cliente, agora verifico permissão
                    if (cliente.id_logado != operador_atual) {
                        // Se o operador atual não for o dono, apenas mostro o erro e saio
                        printf("Erro: voce nao tem permissao para deletar este cliente.\n");
                        fclose(fp);
                        fclose(temp);
                        remove("temp.bin");
                        return;
                    } else {
                        // Se tiver permissão, não escrevo esse cliente no arquivo temporário
                        deletado = 1;
                        continue;
                    }
                }

                // Caso não seja o cliente a deletar, regravo normalmente no arquivo temporário
                fwrite(&cliente, sizeof(Cliente), 1, temp);
            }

            // Fecho ambos os arquivos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo antigo pelo temporário atualizado
            remove("clientes.bin");
            rename("temp.bin", "clientes.bin");

            // Exibo a mensagem final apropriada
            if (deletado)
                printf("Cliente %s deletado em ARQUIVO BINARIO!\n",cliente.nome);
            else
                printf("Cliente com CPF/CNPJ %s nao encontrado em BINARIO!\n", cpf_cnpj_busca);

            return;
        }

        // =====================================================
        // Caso padrão — tipo de armazenamento inválido
        // =====================================================
        default:
            printf("Tipo de armazenamento invalido!\n");
            return;
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