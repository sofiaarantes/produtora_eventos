#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor_parceiro.h"
#include "../../view/fornecedor/fornecedor_parceiro_view.h"
#include "../../view/main/main_view.h"
#include "../../view/main/main_view.h"
#include "../../model/sessao/sessao.h"
#include "../../util/util.h"


#define MAX_FORNECEDORES 100
static Fornecedor_parceiro fornecedores_memoria[MAX_FORNECEDORES]; // cada posição guarda um fornecedor/parceiro
static int qtd = 0; // contador de quantos fornecedores/parceiros já estão salvos na memória

int novo_id = 1; // Id começa em 1 por padrão

Fornecedor_parceiro* criar_fornecedor_parceiro(Fornecedor_parceiro* fornecedor, TipoArmazenamento tipo) {
    // Se o ponteiro fornecedor for nulo, nao há como salvar — retorno NULL
    if (!fornecedor) return NULL;

    // Começo o ID em 1. Caso haja fornecedores anteriores, ele será atualizado mais abaixo
    int novo_id = 1;

   

    //antes de salvar verifica se o tel é valido
                if (validar_tel(fornecedor->tel) == 0) {
                    printf("Erro: telefone invalido. Deve conter 11 digitos.\n");
                    printf("Fornecedor/Parceiro NAO cadastrado\n");
                    return NULL;
                }
    //antes de salvar verifica se o cnpj é valido
                if (validar_cnpj(fornecedor->cnpj) == 0) {
                    printf("Erro: CNPJ invalido. Deve conter 14 digitos.\n");
                    printf("Fornecedor/Parceiro NAO cadastrado\n");
                    return NULL;
                }

    // Escolho o tipo de armazenamento que o sistema está usando (memória, texto ou binário)
    switch (tipo) {
        

        // ============================================
        // CASO 1 - SALVAR NA MEMORIA
        // ============================================
        case MEMORIA: {
            // Verifico se ainda há espaço no array de fornecedors em memória
            if (qtd < MAX_FORNECEDORES) {

                 // Antes de salvar, verifica se o cnpj inserido já existe em memória
                for (int i = 0; i < qtd; i++) {
                    if (strcmp(fornecedores_memoria[i].cnpj, fornecedor->cnpj) == 0 ) {
                        printf("\nErro: Ja existe um fornecedor/parceiro com o CNPJ '%s' cadastrado.\n", fornecedor->cnpj);
                        printf("Fornecedor/Parceiro NAO cadastrado\n\n");
                        return NULL;
                    }
                }

                // Se já existir pelo menos um fornecedor, pego o último ID e incremento
                if (qtd > 0)
                    novo_id = fornecedores_memoria[qtd - 1].id + 1;

                // Atribuo o novo ID ao fornecedor atual
                fornecedor->id = novo_id;

                // Salvo o fornecedor no vetor em memória
                fornecedores_memoria[qtd] = *fornecedor;

                // Crio um ponteiro que aponta para o fornecedor recém-salvo
                Fornecedor_parceiro* salvo = &fornecedores_memoria[qtd];

                fornecedor->id_logado = get_operador_logado(); // garante que está atualizado
                
                // Incremento a contagem total de fornecedors
                qtd++;

                printf("Fornecedor/Parceiro %s salvo em MEMORIA!\n",fornecedor->nome_fantasia);

                // Retorno o ponteiro do fornecedores que acabou de ser salvo
                return salvo;
            } else {
                // Caso o vetor esteja cheio, nao consigo salvar
                printf("Erro: limite de fornedores/parceiros na memória atingido!\n");
                return NULL;
            }
        }


        // ============================================
        // CASO 2 - SALVAR EM ARQUIVO TEXTO
        // ============================================
        case TEXTO: {
            // Abro o arquivo de texto para leitura, para descobrir o último ID usado
            FILE* fp = fopen("fornecedores.txt", "r");
            if (fp) {
                char linha[512];  // buffer para armazenar cada linha lida
                Fornecedor_parceiro tmp;      // struct temporária para ler os dados do arquivo

                // Lê o arquivo linha por linha de forma segura (fgets evita travamentos)
                while (fgets(linha, sizeof(linha), fp)) {
                    // Tenta ler os campos formatados por ponto e vírgula
                    // %[^;] significa “ler até encontrar um ponto e vírgula”
                    int lidos = sscanf(linha, "%d;%49[^;];%49[^;];%14[^;];%99[^;];%11[^;];%49[^;];%d;%f",
                                        &tmp.id, tmp.nome_fantasia, tmp.razao_social,
                                        tmp.endereco_completo,tmp.cnpj, 
                                        tmp.tel, tmp.tipo_servico, &tmp.id_logado, &tmp.valor);

                    // Se conseguiu ler pelo menos o ID, atualiza o próximo id
                    if (lidos >= 1) {
                        novo_id = tmp.id + 1;
                    }
                    // Se encontrar mesmo CNPJ o dentro do aqr txt ja cadastrado impede o cadastro
                    if (strcmp(tmp.cnpj, fornecedor->cnpj) == 0) {
                        printf("\nErro: Ja existe um fornecedor/parceiro com o CNPJ '%s' cadastrado .\n",
                            fornecedor->cnpj);
                            printf("Fornecedor/Parceiro NAO cadastrado\n");
                        fclose(fp);
                        return NULL;
                    }
                }
                // Fecha o arquivo após ler todas as linhas
                fclose(fp);
            }

            // Define o novo ID no fornecedor que será inserido
            fornecedor->id = novo_id;

            fornecedor->id_logado = get_operador_logado(); // Associo o fornecedor ao operador logado

            // Agora abre o arquivo em modo append (“a”) para adicionar o novo fornecedor no final
            fp = fopen("fornecedores.txt", "a");
            if (!fp) {
                perror("Erro ao abrir fornecedores.txt");
                return NULL;
            }

            // Grava os dados do fornecedor em uma linha formatada, separados por ponto e vírgula
            fprintf(fp, "%d;%s;%s;%s;%s;%s;%s;%d;%f\n",
                    fornecedor->id,
                    fornecedor->nome_fantasia,
                    fornecedor->razao_social,
                    fornecedor->endereco_completo,
                    fornecedor->cnpj,
                    fornecedor->tel,
                    fornecedor->tipo_servico,
                    fornecedor->id_logado,
                    fornecedor->valor
                );

            // Fecha o arquivo após a escrita
            fclose(fp);

            printf("Fornecedor/Parceiro %s salvo em TEXTO!\n", fornecedor->nome_fantasia);

            // Retorna o ponteiro para o fornecedor criado
            return fornecedor;
        }


        // ============================================
        // CASO 3 - SALVAR EM ARQUIVO BINARIO
        // ============================================
        case BINARIO: {
            // Abre o arquivo binário em modo leitura ("rb") para descobrir o último ID
            FILE* fp = fopen("fornecedores.bin", "rb");
            if (fp) {
                Fornecedor_parceiro tmp;
                // Lê fornecedor por fornecedor até o final do arquivo
                while (fread(&tmp, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                    novo_id = tmp.id + 1; // sempre pega o último ID e soma 1

                if (strcmp(tmp.cnpj, fornecedor->cnpj) == 0 ) {
                        printf("\nErro: Ja existe um Fornecedor/Parceiro com o CNPJ '%s' cadastrado.\n",
                               fornecedor->cnpj);
                               printf("Produtora NAO cadastrado\n");
                        fclose(fp);
                        return NULL;
                    }
                }
                fclose(fp);
            }

            // Define o novo ID para o fornecedor atual
            fornecedor->id = novo_id;

            fornecedor->id_logado = get_operador_logado(); // Associo o fornecedor ao operador logado
            // Garante que os campos string terminam com '\0'
            fornecedor->cnpj[sizeof(fornecedor->cnpj) - 1] = '\0';
            fornecedor->tel[sizeof(fornecedor->tel) - 1] = '\0';

            // Abre o arquivo binário em modo append binário ("ab")
            fp = fopen("fornecedores.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir fornecedores.bin");
                return NULL;
            }

            // Escreve o fornecedor no arquivo em formato binário
            if (fwrite(fornecedor, sizeof(Fornecedor_parceiro), 1, fp) != 1) {
                perror("Erro ao gravar fornecedor em fornecedors.bin");
                fclose(fp);
                return NULL;
            }

            // Fecha o arquivo após gravar
            fclose(fp);

            printf("Fornecedor/Parceiro %s salvo em BINARIO!\n", fornecedor->nome_fantasia);

            // Retorna o ponteiro do fornecedor que foi salvo
            return fornecedor;
        }
    }

    // Caso o tipo de armazenamento nao seja reconhecido, retorno NULL
    return NULL;
}

Fornecedor_parceiro* atualizar_fornecedor_parceiro(const char* cnpj_busca, Fornecedor_parceiro* novos_dados, TipoArmazenamento tipo) {

    // Verifico se os parâmetros passados são válidos antes de qualquer coisa
    if (!cnpj_busca || !novos_dados) return NULL;


    //antes de salvar verifica se o tel é valido
                if (validar_tel(novos_dados->tel) == 0) {
                    printf("Erro: telefone invalido. Deve conter 11 digitos.\n");
                    printf("Fornecedor/Parceiro NAO atualizado\n");
                    return NULL;
                }

    // Uso um switch para tratar de acordo com o tipo de armazenamento escolhido
    switch (tipo) {

        // =======================================================
        // CASO 1: Atualização de fornecedor armazenado na MEMÓRIA
        // =======================================================
        case MEMORIA: {
            // Percorro todos os fornecedores armazenados na memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ atual com o que foi informado pelo usuário
                if (strcmp(fornecedores_memoria[i].cnpj, cnpj_busca) == 0) {

                    // Guardo o id_logado original
                    int id_logado_original = fornecedores_memoria[i].id_logado;

                    // Substituo o fornecedor antigo pelos novos dados
                    fornecedores_memoria[i] = *novos_dados;

                    // Mas garanto que o CNPJ e id_logado originais nao sejam alterados
                    strcpy(fornecedores_memoria[i].cnpj, cnpj_busca);
                    fornecedores_memoria[i].id_logado = id_logado_original;

                    // Aviso que o fornecedor foi atualizado
                    printf("Fornecedor/Parceiro %s atualizado em MEMORIA!\n", fornecedores_memoria[i].nome_fantasia);

                    // Retorno o ponteiro para o fornecedor atualizado
                    return &fornecedores_memoria[i];
                }
            }

            // Se chegar aqui, é porque o CNPJ nao foi encontrado
           printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em MEMORIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            return NULL;
        }

        // =======================================================
        // CASO 2: Atualização de fornecedor armazenado em ARQUIVO TEXTO
        // =======================================================
        case TEXTO: {
            // Abro o arquivo original de fornecedores em modo leitura
            FILE* fp = fopen("fornecedores.txt", "r");
            if (!fp) {
                perror("Erro ao abrir fornecedores.txt");
                return NULL;
            }

            // Crio um arquivo temporário onde vou escrever as atualizações
            FILE* temp = fopen("fornecedores_tmp.txt", "w");
            if (!temp) {
                perror("Erro ao criar fornecedores_tmp.txt");
                fclose(fp);
                return NULL;
            }

            char linha[400]; // buffer para ler as linhas do arquivo
            int atualizado = 0; // flag para indicar se algum fornecedor foi atualizado

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                Fornecedor_parceiro f = {0};

                // Quebro a linha em campos separados por ';'
                 sscanf(linha, "%d;%49[^;];%49[^;];%14[^;];%99[^;];%11[^;];%49[^;];%d,%f",
                                        &f.id, f.nome_fantasia, f.razao_social,
                                       f.endereco_completo, f.cnpj, 
                                        f.tel, f.tipo_servico, &f.id_logado, &f.valor);


                // Comparo o CNPJ lido com o buscado
                if (strcmp(f.cnpj, cnpj_busca) == 0) {

                    // Aqui sobrescrevo a linha do fornecedor com os novos dados
                    // mantendo id, CNPJ e id_logado originais
                    fprintf(temp, "%d;%s;%s;%s;%s;%s;%s;%d;%f\n",
                            f.id, 
                            novos_dados->nome_fantasia,
                            novos_dados->razao_social,
                            novos_dados->endereco_completo,
                            f.cnpj,
                            novos_dados->tel,
                            novos_dados->tipo_servico,
                            f.id_logado,
                            novos_dados->valor
                        );
                        

                    atualizado = 1; // marco que o fornecedor foi atualizado
                } else {
                    // Se nao for o fornecedor buscado, só copio a linha normalmente
                    fputs(linha, temp);
                }
            }

            // Fecho os dois arquivos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo novo com as atualizações
            remove("fornecedores.txt");
            rename("fornecedores_tmp.txt", "fornecedores.txt");

            // Exibo mensagem de acordo com o resultado
            if (atualizado) {
                printf("Fornecedor/Parceiro %s atualizado em TEXTO!\n", novos_dados->nome_fantasia);
                return novos_dados;
            } else {
                printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
                return NULL;
            }
        }

        // =======================================================
        // CASO 3: Atualização de fornecedor armazenado em ARQUIVO BINÁRIO
        // =======================================================
        case BINARIO: {
            // Abro o arquivo binário para leitura e escrita
            FILE* fp = fopen("fornecedores.bin", "rb+");
            if (!fp) {
                perror("Erro ao abrir fornecedores.bin");
                return NULL;
            }

            Fornecedor_parceiro f;
            int encontrado = 0; // flag para marcar se encontrei o fornecedor

            // Leio fornecedor por fornecedor no arquivo
            while (fread(&f, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                // Garante que o CPF/CNPJ está terminado corretamente com '\0'
                f.cnpj[sizeof(f.cnpj) - 1] = '\0';

                // Comparo o CPF/CNPJ lido com o buscado
                if (strcmp(f.cnpj, cnpj_busca) == 0) {
                    encontrado = 1;

                    // Volto o cursor do arquivo para reescrever esse mesmo fornecedor
                    fseek(fp, -(long)sizeof(Fornecedor_parceiro), SEEK_CUR);

                    // Copio os dados fixos que nao podem ser alterados
                    novos_dados->id = f.id;
                    strcpy(novos_dados->cnpj, f.cnpj);
                    novos_dados->id_logado = f.id_logado;

                    // Gravo os novos dados no arquivo (sobrescrevendo o fornecedor antigo)
                    if (fwrite(novos_dados, sizeof(Fornecedor_parceiro), 1, fp) != 1) {
                        perror("Erro ao escrever fornecedor atualizado em fornecedores.bin");
                        fclose(fp);
                        return NULL;
                    }

                    // Fecho o arquivo e informo o sucesso
                    fclose(fp);
                    printf("Fornecedor/Parceiro %s atualizado em BINARIO!\n", novos_dados->nome_fantasia);
                    return novos_dados;
                }
            }

            // Fecho o arquivo caso o fornecedor nao tenha sido encontrado
            fclose(fp);

            // Se nao encontrei o fornecedor, aviso o usuário
            if (!encontrado) {
                 printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em BINARIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
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

void buscar_e_exibir_fornecedor_parceiro(const char* cnpj_busca, TipoArmazenamento tipo) {

    // --------------------------------------------------------
    // Primeiro, verifico se o parâmetro CNPJ é válido.
    // Se for NULL (nulo), nao adianta prosseguir — o usuário
    // provavelmente esqueceu de digitar o documento.
    // --------------------------------------------------------
    if (!cnpj_busca) {
        printf("+--------------------------+\n");
        printf("| CNPJ invalido!       |\n");
        printf("+--------------------------+\n");
        return; // interrompo a função aqui
    }

    // --------------------------------------------------------
    // Crio um ponteiro para Forncedor_parceiro que usarei conforme o tipo
    // de armazenamento (pode ser carregado da memória, texto ou binário).
    // --------------------------------------------------------
    Fornecedor_parceiro* fornecedor = NULL;

    switch (tipo) {

        // =====================================================
        // Caso 1: Busca em memória (dados já carregados no vetor)
        // =====================================================
        case MEMORIA: {
            // Percorro o vetor global 'fornecedores_memoria' até a quantidade cadastrada
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ de cada fornecedor com o que foi buscado
                if (strcmp(fornecedores_memoria[i].cnpj, cnpj_busca) == 0) {

                    // Se achei, exibo os dados
                    ver_fornecedor_parceiro(&fornecedores_memoria[i]);
                    return; // Saio da função pois já encontrei
                }
            }

            // Se percorrer todo o vetor e nao encontrar o fornecedoe, informo isso
            printf("+--------------------------------------+\n");
            printf("| Fornecedor/Parceiro nao encontrado!  |\n");
            printf("+--------------------------------------+\n");
            break; // Termina o case MEMORIA
        }

        // =====================================================
        // Caso 2: Busca em arquivo texto (fornecedores.txt)
        // =====================================================
        case TEXTO: {
            // Tento abrir o arquivo texto para leitura
            FILE* fp = fopen("fornecedores.txt", "r");
            if (!fp) {
                // Se nao conseguir abrir, mostro erro do sistema (perror)
                perror("Erro ao abrir fornecedores.txt");
                return;
            }

            // Aloco memória temporária para armazenar os dados lidos do arquivo
            fornecedor = malloc(sizeof(Fornecedor_parceiro));
            char linha[512]; // buffer para ler cada linha do arquivo

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Cada linha contém os dados separados por ';'
                // Uso sscanf para extrair os campos para a estrutura fornecedor
                sscanf(linha, "%d;%49[^;];%49[^;];%99[^;];%14[^;];%11[^;];%49[^;];%d;%f",
                       &fornecedor->id,
                       fornecedor->nome_fantasia,
                       fornecedor->razao_social,
                       fornecedor->endereco_completo,
                       fornecedor->cnpj,
                       fornecedor->tel,
                       fornecedor->tipo_servico,
                       &fornecedor->id_logado,
                       &fornecedor->valor
                    );

                // Comparo o CNPJ lido com o buscado
                if (strcmp(fornecedor->cnpj, cnpj_busca) == 0) {

                    // Se encontrei, exibo os dados do fornecedor
                    ver_fornecedor_parceiro(fornecedor);
                    fclose(fp);
                    free(fornecedor);
                    return; // Já encontrei, então retorno
                }
            }

            // Se chegou aqui, é porque leu o arquivo todo e nao achou o CNPJ
            fclose(fp);
            free(fornecedor);
            fornecedor = NULL; // segurança: aponto para NULL após liberar memória
            //msg para informar o usuario
            printf("+--------------------------------------+\n");
            printf("| Fornecedor/Parceiro nao encontrado!  |\n");
            printf("+--------------------------------------+\n");

            break;
        }

        // =====================================================
        // Caso 3: Busca em arquivo binário (fornecedores.bin)
        // =====================================================
        case BINARIO: {
            // Tento abrir o arquivo binário em modo leitura
            FILE* fp = fopen("fornecedores.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir fornecedores.bin");
                return;
            }

            // Aloco um fornecedor temporário para armazenar os dados lidos do binário
            fornecedor = malloc(sizeof(Fornecedor_parceiro));

            // Leio o arquivo fornecedor por fornecedor
            while (fread(fornecedor, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                // Garante que a string CNPJ termina com '\0'
                fornecedor->cnpj[sizeof(fornecedor->cnpj) - 1] = '\0';

                // Comparo com o CPF/CNPJ buscado
                if (strcmp(fornecedor->cnpj, cnpj_busca) == 0) {
                    // Se encontrei, exibo os dados do fornecedor
                    ver_fornecedor_parceiro(fornecedor);
                    fclose(fp);
                    free(fornecedor);
                    return;
                }
            }

            // Se o loop terminou e nao encontrou o fornecedor, aviso o usuário
            fclose(fp);
            free(fornecedor); //libero a memoria temporaria
            fornecedor = NULL; //retorno null no memoria alocada por precausao

            printf("+--------------------------------------+\n");
            printf("| Fornecedor/Parceiro nao encontrado!  |\n");
            printf("+--------------------------------------+\n");

            break;
        }

        // =====================================================
        // Caso padrão (tipo de armazenamento inválido)
        // =====================================================
        default:
            printf("Tipo de armazenamento invalido!\n");
            return;
    }
}

void deletar_fornecedor_parceiro(const char* cnpj_busca, TipoArmazenamento tipo) {
    // Primeiro verifico se o parâmetro CNPJ recebido é válido
    // Se for nulo, já mostro uma mensagem de erro e saio da função.
    if (!cnpj_busca) {
        printf("+--------------------------+\n");
        printf("|     CNPJ invalido!       |\n");
        printf("+--------------------------+\n");
        return; // interrompo a função aqui
    }

    // Uso o switch para tratar cada tipo de armazenamento (memória, texto ou binário)
    switch (tipo) {

        // =====================================================
        // Caso 1 — Deletar fornecedor armazenado em MEMÓRIA
        // =====================================================
        case MEMORIA: {
            // Percorro o vetor global de fornecedores armazenados em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ digitado com o do fornecedor atual
                if (strcmp(fornecedores_memoria[i].cnpj, cnpj_busca) == 0) {

                    // Se achei, preciso remover o fornecedor do vetor
                    // Para isso, desloco todos os próximos elementos uma posição para trás
                    for (int j = i; j < qtd - 1; j++) {
                        fornecedores_memoria[j] = fornecedores_memoria[j + 1];
                    }

                    // Reduzo a contagem total de fornecedors em memória
                    qtd--;

                    // Exibo mensagem de sucesso e saio da função
                    printf("Fornecedor/Parceiro %s deletado em MEMORIA!\n",fornecedores_memoria[i].nome_fantasia);
                    return;
                }
            }

            // Se cheguei até aqui, o CNPJ não foi encontrado
            printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em MEMORIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            return;
        }

        // =====================================================
        // Caso 2 — Deletar fornecedor em ARQUIVO TEXTO (.txt)
        // =====================================================
        case TEXTO: {
            // Abro o arquivo de fornecedors em modo leitura
            FILE* fp = fopen("fornecedores.txt", "r");
            if (!fp) {
                perror("Erro ao abrir fornecedores.txt");
                return;
            }

            // Crio um arquivo temporário onde vou regravar apenas os fornecedores válidos
            FILE* temp = fopen("temp.txt", "w");
            if (!temp) {
                perror("Erro ao criar temp.txt");
                fclose(fp);
                return;
            }

            char linha[512];    // Buffer usado para ler cada linha do arquivo
            Fornecedor_parceiro fornecedor;    // Struct temporária para armazenar os dados lidos
            int deletado = 0;   // Variável de controle para saber se excluí algum fornecedor

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Tento extrair todos os campos, incluindo tipo_doc e id_logado
                 sscanf(linha, "%d;%49[^;];%49[^;];%99[^;];%14[^;];%11[^;];%49[^;];%d;%f",
                                   &fornecedor.id,
                                   fornecedor.nome_fantasia,
                                   fornecedor.razao_social,
                                   fornecedor.endereco_completo,
                                   fornecedor.cnpj,
                                   fornecedor.tel,
                                   fornecedor.tipo_servico,
                                   &fornecedor.id_logado,
                                   &fornecedor.valor
                                );

                // Verifico se esta linha pertence ao fornecedor que quero deletar
                if (strcmp(fornecedor.cnpj, cnpj_busca) == 0) {
                    // Se achei o fornecedor, simplesmente não escrevo essa linha no arquivo temporário
                    // Isso faz com que o fornecedor seja removido
                    deletado = 1;
                    continue; // Pulo para a próxima linha
                    
                }

                // Caso não seja o fornecedor que quero excluir, regravo a linha normalmente
                fputs(linha, temp);
            }

            // Fecho os arquivos originais e temporários
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo novo (sem o fornecedor deletado)
            remove("fornecedores.txt");
            rename("temp.txt", "fornecedores.txt");

            // Se o fornecedor foi encontrado e deletado, exibo sucesso
            if (deletado) {
                printf("Fornecedor/Parceiro %s deletado em ARQUIVO TEXTO!\n",fornecedor.nome_fantasia);
            } else {
                // Caso contrário, o fornecedor não existia
                printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            }
            return;
        }

        // =====================================================
        // Caso 3 — Deletar fornecedor em ARQUIVO BINÁRIO (.bin)
        // =====================================================
        case BINARIO: {
            // Abro o arquivo binário original em modo leitura
            FILE* fp = fopen("fornecedores.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir fornecedores.bin");
                return;
            }

            // Crio um arquivo temporário onde vou regravar apenas os fornecedores válidos
            FILE* temp = fopen("temp.bin", "wb");
            if (!temp) {
                perror("Erro ao criar temp.bin");
                fclose(fp);
                return;
            }

            Fornecedor_parceiro fornecedor;   // Struct temporária usada para leitura
            int deletado = 0;  // Controle de exclusão bem-sucedida

            // Leio cada fornecedor do arquivo binário
            while (fread(&fornecedor, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                // Garante que o CNPJ termine corretamente
                fornecedor.cnpj[sizeof(fornecedor.cnpj) - 1] = '\0';

                // Comparo o CPF lido com o CPF digitado
                if (strcmp(fornecedor.cnpj, cnpj_busca) == 0) {
                    // Achei o fornecedor
                    // Simplesmente não regravo esse fornecedor no arquivo temporário
                        deletado = 1;
                        continue;
                    
                }

                // Caso não seja o fornecedor a deletar, regravo normalmente no arquivo temporário
                fwrite(&fornecedor, sizeof(Fornecedor_parceiro), 1, temp);
            }

            // Fecho ambos os arquivos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo antigo pelo temporário atualizado
            remove("fornecedores.bin");
            rename("temp.bin", "fornecedores.bin");

            // Exibo a mensagem final apropriada
            if (deletado)
                printf("Fornecedor/Parceiro %s deletado em ARQUIVO BINARIO!\n",fornecedor.nome_fantasia);
            else
                printf("Fornecedor/Parceiro com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em BINARIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);

            return;
        }

                        // =====================================================
                        // Caso padrão — tipo de armazenamento inválido
                        // =====================================================
                        default:
                            printf("Tipo de armazenamento inválido!\n");
                            break;
                    }
                }

// Lista todos os fornecedores do tipo especificado. Retorna array alocado e seta out_count.
// Se não houver registros, retorna NULL e out_count = 0.
Fornecedor_parceiro* listar_todos_fornecedores(TipoArmazenamento tipo, int* out_count) {
    if (!out_count) return NULL;
    *out_count = 0;

    switch (tipo) {
        case MEMORIA: {
            if (qtd == 0) return NULL;
            Fornecedor_parceiro* arr = malloc(sizeof(Fornecedor_parceiro) * qtd);
            if (!arr) return NULL;
            for (int i = 0; i < qtd; i++) arr[i] = fornecedores_memoria[i];
            *out_count = qtd;
            return arr;
        }
        case TEXTO: {
            FILE* fp = fopen("fornecedores.txt", "r");
            if (!fp) return NULL;
            int count = 0;
            char linha[512];
            while (fgets(linha, sizeof(linha), fp)) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            Fornecedor_parceiro* arr = malloc(sizeof(Fornecedor_parceiro) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fgets(linha, sizeof(linha), fp) && idx < count) {
                Fornecedor_parceiro f = {0};
                sscanf(linha, "%d;%49[^;];%49[^;];%99[^;];%14[^;];%11[^;];%49[^\n]",
                    &f.id, f.nome_fantasia, f.razao_social, f.endereco_completo,
                    f.cnpj, f.tel, f.tipo_servico);
                f.cnpj[sizeof(f.cnpj)-1] = '\0';
                arr[idx++] = f;
            }
            fclose(fp);
            *out_count = idx;
            return arr;
        }
        case BINARIO: {
            FILE* fp = fopen("fornecedores.bin", "rb");
            if (!fp) return NULL;
            int count = 0;
            Fornecedor_parceiro tmp;
            while (fread(&tmp, sizeof(Fornecedor_parceiro), 1, fp) == 1) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            Fornecedor_parceiro* arr = malloc(sizeof(Fornecedor_parceiro) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fread(&arr[idx], sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                arr[idx].cnpj[sizeof(arr[idx].cnpj)-1] = '\0';
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
int limpar_fornecedores(TipoArmazenamento tipo) {
    switch (tipo) {
        case MEMORIA:
            qtd = 0;
            // memset(fornecedores_memoria, 0, sizeof(fornecedores_memoria));
            return 1;
        case TEXTO:
            remove("fornecedores.txt");
            return 1;
        case BINARIO:
            remove("fornecedores.bin");
            return 1;
        default:
            return 0;
    }
}

void listar_fornecedores(TipoArmazenamento tipo) {

    printf("\n");
    printf("+ --------------------------------------------------------------- +\n");
    printf("|                      LISTA DE FORNECEDORES                      |\n");
    printf("+ --------------------------------------------------------------- +\n");

    switch (tipo) {

        // ============================================================
        // MODO MEMORIA
        // Dados carregados direto do vetor global fornecedores_memoria
        // ============================================================
        case MEMORIA: {
            int encontrados = 0;

            for (int i = 0; i < qtd; i++) {
                printf("|  ID: %d | Nome Fantasia: %s | Servico: %s | Valor: %.2f\n",
                       fornecedores_memoria[i].id,
                       fornecedores_memoria[i].nome_fantasia,
                       fornecedores_memoria[i].tipo_servico,
                       fornecedores_memoria[i].valor);
                encontrados++;
            }

            if (encontrados == 0)
                printf("|  Nenhum fornecedor cadastrado.\n");

            break;
        }

        // ============================================================
        // MODO TEXTO
        // Aqui eu leio o arquivo fornecedores.txt
        // ============================================================
        case TEXTO: {
            FILE* fp = fopen("fornecedores.txt", "r");
            if (!fp) {
                printf("|  Arquivo fornecedores.txt não encontrado.\n");
                return;
            }

            Fornecedor_parceiro tmp;
            int encontrados = 0;

            // Leitura com fscanf no mesmo formato usado na gravação
            while (fscanf(fp, "%d;%49[^;];%49[^;];%14[^;];%99[^;];%11[^;];%49[^;];%d;%f\n",
                          &tmp.id, tmp.nome_fantasia, tmp.razao_social, tmp.cnpj,
                          tmp.endereco_completo, tmp.tel, tmp.tipo_servico,
                          &tmp.id_logado, &tmp.valor) == 9) {

                printf("|  ID: %d | Nome Fantasia: %s | Serviço: %s | Valor: %.2f\n",
                       tmp.id, tmp.nome_fantasia, tmp.tipo_servico, tmp.valor);
                encontrados++;
            }

            fclose(fp);

            if (encontrados == 0)
                printf("|  Nenhum fornecedor encontrado no arquivo texto.\n");

            break;
        }

        // ============================================================
        // MODO BINARIO
        // Leitura via fread() do arquivo fornecedores.bin
        // ============================================================
        case BINARIO: {
            FILE* fp = fopen("fornecedores.bin", "rb");
            if (!fp) {
                printf("|  Arquivo fornecedores.bin não encontrado.\n");
                return;
            }

            Fornecedor_parceiro tmp;
            int encontrados = 0;

            while (fread(&tmp, sizeof(Fornecedor_parceiro), 1, fp) == 1) {
                printf("|  ID: %d | Nome Fantasia: %s | Serviço: %s | Valor: %.2f\n",
                       tmp.id, tmp.nome_fantasia, tmp.tipo_servico, tmp.valor);
                encontrados++;
            }

            fclose(fp);

            if (encontrados == 0)
                printf("|  Nenhum fornecedor encontrado no arquivo binário.\n");

            break;
        }
    }

    printf("+ --------------------------------------------------------------- +\n");
}


Fornecedor_parceiro* buscar_fornecedor_por_id(TipoArmazenamento tipo, int id) {
    static Fornecedor_parceiro fornecedor;  // variável estática para manter o dado válido após retorno
    FILE* arquivo;

    switch (tipo) {
        // ------------------------------------------------------------
        // Caso 1: busca em memória
        // ------------------------------------------------------------
        case MEMORIA:
            for (int i = 0; i < qtd; i++) {
                if (fornecedores_memoria[i].id == id) {
                    return &fornecedores_memoria[i]; // retorna o endereço do fornecedor encontrado
                }
            }
            return NULL; // não achou

        // ------------------------------------------------------------
        // Caso 2: busca em arquivo texto
        // ------------------------------------------------------------
        case TEXTO:
            arquivo = fopen("fornecedores.txt", "r");
            if (!arquivo) {
                printf("Erro: arquivo fornecedores.txt não encontrado.\n");
                return NULL;
            }

            // leio linha a linha no formato CSV
            while (fscanf(arquivo, "%d;%49[^;];%49[^;];%14[^;];%99[^;];%11[^;];%49[^;];%d;%f\n",
                          &fornecedor.id, fornecedor.nome_fantasia, fornecedor.razao_social,
                          fornecedor.cnpj, fornecedor.endereco_completo, fornecedor.tel,
                          fornecedor.tipo_servico, &fornecedor.id_logado, &fornecedor.valor) == 9) {

                if (fornecedor.id == id) {
                    fclose(arquivo);
                    return &fornecedor;
                }
            }

            fclose(arquivo);
            return NULL;

        // ------------------------------------------------------------
        // Caso 3: busca em arquivo binário
        // ------------------------------------------------------------
        case BINARIO:
            arquivo = fopen("fornecedores.bin", "rb");
            if (!arquivo) {
                printf("Erro: arquivo fornecedores.bin não encontrado.\n");
                return NULL;
            }

            while (fread(&fornecedor, sizeof(Fornecedor_parceiro), 1, arquivo)) {
                if (fornecedor.id == id) {
                    fclose(arquivo);
                    return &fornecedor;
                }
            }

            fclose(arquivo);
            return NULL;
    }

    return NULL; // fallback
}

