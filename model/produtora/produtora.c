#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtora.h"
#include "../../view/produtora/produtora_view.h"
#include "../../view/main/main_view.h"
#include "../../model/sessao/sessao.h"


#define MAX_PRODUTORAS 100
static Produtora produtoras_memoria[MAX_PRODUTORAS]; // cada posição guarda uma produtora
static int qtd = 0; // contador de quantas produtoras já estão salvas na memória

// Função que cria e salva uma produtora de acordo com o tipo escolhido
Produtora* criar_produtora(Produtora* produtora, TipoArmazenamento tipo) {
    // Se o ponteiro produtora for nulo, nao há como salvar — retorno NULL
    if (!produtora) return NULL;

    produtora->id_logado = get_operador_logado(); // Associo o produtora ao operador logado

    // Escolho o tipo de armazenamento que o sistema está usando (memória, texto ou binário)
    switch (tipo) {
        

        // ============================================
        // CASO 1 - SALVAR NA MEMORIA
        // ============================================
        case MEMORIA: {
            // Verifico se ainda há espaço no array de produtoras em memória
            if (qtd < MAX_PRODUTORAS) {

                // Salvo o produtora no vetor em memória
                produtoras_memoria[qtd] = *produtora;

                // Crio um ponteiro que aponta para o produtora recém-salvo
                Produtora* salvo = &produtoras_memoria[qtd];

                produtora->id_logado = get_operador_logado(); // garante que está atualizada

                // Incremento a contagem total de produtoras
                qtd++;

                printf("Produtora %s salvo em MEMORIA! \n", produtora->nome_fantasia);

                // Retorno o ponteiro do produtora que acabou de ser salvo
                return salvo;
            } else {
                // Caso o vetor estaja cheio, nao consigo salvar
                printf("Erro: limite de produtoras na memória atingido!\n");
                return NULL;
            }
        }


        // ============================================
        // CASO 2 - SALVAR EM ARQUIVO TEXTO
        // ============================================
        case TEXTO: {
            
            produtora->id_logado = get_operador_logado(); // Associo o produtora ao operador logado

            // Agora abre o arquivo em modo append (“a”) para adicionar o novo produtora no final
            FILE* fp = fopen("produtoras.txt", "a");
            if (!fp) {
                perror("Erro ao abrir produtoras.txt");
                return NULL;
            }

            // Escrevo os dados da produtora no arquivo separados por ponto e vírgula
            // Uso o limite máximo de caracteres baseado no tamanho do array da struct
                 // Isso garante que o campo não ultrapasse o espaço definido
                fprintf(fp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%f;%d\n",
                    produtora->nome_fantasia,
                    produtora->razao_social,
                    produtora->inscricao_estadual,
                    produtora->cnpj,
                    produtora->endereco_completo,
                    produtora->tel,
                    produtora->email,
                    produtora->nome_resp,
                    produtora->tel_resp,
                    produtora->lucro,
                    produtora->id_logado
                );

            // Fecha o arquivo após a escrita
            fclose(fp);


            printf("Produtora %s salvo em TEXTO!\n", produtora->nome_fantasia);

            // Retorna o ponteiro para o produtora criado
            return produtora;
        }


        // ============================================
        // CASO 3 - SALVAR EM ARQUIVO BINARIO
        // ============================================
        case BINARIO: {

            produtora->id_logado = get_operador_logado(); // Associo o produtora ao operador logado
            // Garante que os campos string terminam com '\0'
            produtora->cnpj[sizeof(produtora->cnpj) - 1] = '\0';
            produtora->tel[sizeof(produtora->tel) - 1] = '\0';

            // Abre o arquivo binário em modo append binário ("ab")
            FILE* fp = fopen("produtoras.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir produtoras.bin");
                return NULL;
            }

            // Escreve o produtora no arquivo em formato binário
            if (fwrite(produtora, sizeof(Produtora), 1, fp) != 1) {
                perror("Erro ao gravar produtora em produtoras.bin");
                fclose(fp);
                return NULL;
            }

            // Fecha o arquivo após gravar
            fclose(fp);


            printf("Produtora %s salvo em BINARIO! \n", produtora->nome_fantasia);

            // Retorna o ponteiro do produtora que foi salvo
            return produtora;
        }
    }

    // Caso o tipo de armazenamento nao seja reconhecido, retorno NULL
    return NULL;
}
    
            // Função que atualiza um produtora de acordo com o tipo escolhido
            // recebe o CNPJ do produtora que quero atualizar e o tipo de armazenamento
Produtora* atualizar_produtora(const char* cnpj_busca, Produtora* novos_dados, TipoArmazenamento tipo) {

    // Verifico se os parâmetros passados são válidos antes de qualquer coisa
    if (!cnpj_busca || !novos_dados) return NULL;

    // Pego o ID do operador que está logado no momento (para verificar permissões)
    int operador_atual = get_operador_logado();

    // Uso um switch para tratar de acordo com o tipo de armazenamento escolhido
    switch (tipo) {

        // =======================================================
        // CASO 1: Atualização de produtora armazenado na MEMÓRIA
        // =======================================================
        case MEMORIA: {
            // Percorro todos os produtoras armazenados na memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ atual com o que foi informado pelo usuário
                if (strcmp(produtoras_memoria[i].cnpj, cnpj_busca) == 0) {

                    // Verifico se o operador logado é o mesmo que criou essa produtora
                    if (produtoras_memoria[i].id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para atualizar esta produtora \n");
                        return NULL; // se não for o mesmo, não permito a atualização
                    }

                    // Guardo o id_logado original
                    int id_logado_original = produtoras_memoria[i].id_logado;

                    // Substituo a produtora antigo pelos novos dados
                    produtoras_memoria[i] = *novos_dados;

                    // Mas garanto que o CNPJ e id_logado originais não sejam alterados
                    strcpy(produtoras_memoria[i].cnpj, cnpj_busca);
                    produtoras_memoria[i].id_logado = id_logado_original;

                    // Aviso que o produtora foi atualizada
                    printf("Produtora %s atualizada em MEMORIA!\n", produtoras_memoria[i].nome_fantasia);

                    // Retorno o ponteiro para o produtora atualizada
                    return &produtoras_memoria[i];
                }
            }

            // Se chegar aqui, é porque o CPF/CNPJ não foi encontrado
             printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada em MEMORIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            return NULL;
        }

        // =======================================================
        // CASO 2: Atualização de produtora armazenado em ARQUIVO TEXTO
        // =======================================================
            case TEXTO: {
                // Abro o arquivo principal que contém todas as produtoras cadastradas em modo leitura
                // Esse arquivo armazena cada produtora em uma linha, separando os campos por ';'
                FILE* fp = fopen("produtoras.txt", "r");
                if (!fp) {
                    // Caso o arquivo não consiga ser aberto (não existe ou deu erro no sistema)
                    // uso perror para mostrar o motivo exato do erro.
                    perror("Erro ao abrir produtoras.txt");
                    return NULL; // E retorno NULL, já que não tem como atualizar sem o arquivo
                }

                // Crio um arquivo temporário onde vou reescrever os dados de todas as produtoras.
                // Se a produtora que quero atualizar for encontrada, vou escrever os dados novos nela.
                // Caso contrário, apenas copio as linhas antigas.
                FILE* temp = fopen("produtoras_tmp.txt", "w");
                if (!temp) {
                    // Se não conseguir criar o arquivo temporário, mostro erro, fecho o arquivo principal e saio
                    perror("Erro ao criar produtoras_tmp.txt");
                    fclose(fp);
                    return NULL;
                }

                // Aqui crio um buffer (vetor de caracteres) para armazenar cada linha lida do arquivo
                char linha[400];

                // Variável de controle para saber se alguma produtora foi realmente atualizada
                // (0 = nao, 1 = sim)
                int atualizado = 0;

                // Leio o arquivo de produtoras linha por linha até o final do arquivo
                while (fgets(linha, sizeof(linha), fp)) {

                    // Crio uma struct Produtora vazia, inicializada com zeros
                    // Aqui vou armazenar os dados lidos da linha atual
                    Produtora pro = {0};

                    // Uso sscanf para "quebrar" a linha em partes, separadas por ponto e vírgula (;)
                    // Cada campo da produtora é extraído e colocado na variável correspondente da struct
                    // IMPORTANTE: cada "%..." define o tamanho máximo que pode ser lido em cada campo
                    sscanf(
                        linha,
                        "%49[^;];%49[^;];%14[^;];%19[^;];%99[^;];%14[^;];%49[^;];%49[^;];%14[^;];%f;%d",
                        pro.nome_fantasia,        
                        pro.razao_social,         
                        pro.inscricao_estadual,   
                        pro.cnpj,                 
                        pro.endereco_completo,    
                        pro.tel,                  
                        pro.email,                
                        pro.nome_resp,            
                        pro.tel_resp,             
                        &pro.lucro,               
                        &pro.id_logado            
                    ); 

                    // Verifico se o CNPJ que acabei de ler dessa linha é igual ao CNPJ que o usuário quer atualizar
                    if (strcmp(pro.cnpj, cnpj_busca) == 0) {

                        // Se for o mesmo CNPJ, significa que encontrei a produtora que preciso atualizar

                        // Agora preciso verificar se o operador logado tem permissão para atualizar essa produtora
                        // Isso é feito comparando o id_logado salvo na linha com o id do operador atual
                        if (pro.id_logado != operador_atual) {
                            // Se forem diferentes, o usuário atual não cadastrou essa produtora
                            
                            printf("Erro: voce nao tem permissao para atualizar esta produtora\n");

                            // Fecho os arquivos abertos
                            fclose(fp);
                            fclose(temp);

                            // Apago o arquivo temporário, já que não quero deixar lixo parcial
                            remove("produtora_tmp.txt");

                            // Retorno NULL e saio imediatamente da função, sem fazer nenhuma modificação
                            return NULL;
                        }

                        // Se chegou até aqui, significa que o operador atual tem permissão de atualizar
                        // (ele mesmo cadastrou a produtora anteriormente)

                        // Aqui reescrevo a linha da produtora com os novos dados inseridos pelo usuário
                        // OBS: o CNPJ e o id_logado permanecem iguais, pois não podem ser alterados
                        fprintf(temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%f;%d\n",
                            novos_dados->nome_fantasia,   
                            novos_dados->razao_social,    
                            novos_dados->inscricao_estadual, 
                            pro.cnpj,                     // Mantém o CNPJ original
                            novos_dados->endereco_completo, 
                            novos_dados->tel,             
                            novos_dados->email,  
                            novos_dados->nome_resp,         
                            novos_dados->tel_resp,        
                            novos_dados->lucro,           
                            pro.id_logado                 // Mantém o ID do operador que cadastrou
                        );

                        // Marco que a produtora foi atualizada com sucesso
                        atualizado = 1;
                    } else {
                        // Se o CNPJ da linha atual não for o que estou procurando,
                        // simplesmente copio essa linha pro arquivo temporário sem mudar nada
                        fputs(linha, temp);
                    }
                }

                // Depois de processar todas as linhas, fecho os arquivos
                fclose(fp);
                fclose(temp);

                // Agora substituo o arquivo antigo (produtoras.txt) pelo arquivo temporário com as alterações
                remove("produtoras.txt");
                rename("produtoras_tmp.txt", "produtoras.txt");

                // Exibo mensagens diferentes dependendo do resultado da atualização
                if (atualizado) {
                    // Caso tenha sido atualizado com sucesso, informo o nome da produtora
                    printf("Produtora %s atualizado em TEXTO!\n", novos_dados->nome_fantasia);
                    return novos_dados; // retorno os dados atualizados
                } else {
                    // Caso não tenha encontrado nenhuma produtora com o CNPJ informado
                    // formata o CNPJ na saída para ficar mais legível
                    printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada em TEXTO!\n",
                        cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]
                    );
                    return NULL; // retorno NULL porque não houve atualização
                }
            }

    

        // =======================================================
        // CASO 3: Atualização de produtora armazenado em ARQUIVO BINÁRIO
        // =======================================================
        case BINARIO: {
            // Abro o arquivo "produtoras.bin" em modo de leitura e escrita binária (rb+)
            // Isso me permite tanto ler quanto atualizar (sobrescrever) dados já existentes no arquivo.
            FILE* fp = fopen("produtoras.bin", "rb+");
            if (!fp) {
                // Se não conseguir abrir o arquivo (por exemplo, ele não existe ainda),
                // mostro a mensagem de erro detalhada (perror usa a mensagem do sistema operacional)
                perror("Erro ao abrir produtoras.bin");
                return NULL; // Retorno NULL porque não há como continuar sem o arquivo
            }

            // Crio uma variável 'p' do tipo Produtora.
            // Ela vai servir como um buffer temporário para armazenar cada produtora lida do arquivo.
            Produtora p;

            // Crio uma flag (variável de controle) para saber se encontrei a produtora com o CNPJ buscado.
            // Começa em 0 (falso) e só muda para 1 (verdadeiro) se o CNPJ for encontrado.
            int encontrado = 0;

            // Uso fread para ler o arquivo bloco por bloco (um struct Produtora por vez)
            // fread retorna 1 quando a leitura é bem-sucedida, então esse while vai rodar até o fim do arquivo.
            while (fread(&p, sizeof(Produtora), 1, fp) == 1) {

                // Aqui eu garanto que o campo CNPJ dentro da struct está corretamente terminado com '\0'
                // Isso evita erros ao comparar strings, caso o campo tenha sido gravado sem o terminador.
                p.cnpj[sizeof(p.cnpj) - 1] = '\0';

                // Comparo o CNPJ lido do arquivo com o CNPJ que o usuário digitou (cnpj_busca)
                // Se forem iguais, significa que encontrei a produtora certa.
                if (strcmp(p.cnpj, cnpj_busca) == 0) {
                    // Marco que encontrei a produtora
                    encontrado = 1;

                    // Agora preciso verificar se o operador logado tem permissão para atualizar essa produtora.
                    // Isso é feito comparando o ID salvo no campo id_logado com o ID do operador atual.
                    if (p.id_logado != operador_atual) {
                        // Se forem diferentes, quer dizer que o usuário logado não criou essa produtora.
                        // Então ele NÃO tem permissão para alterá-la.
                        printf("Erro: voce nao tem permissao para atualizar esta produtora\n");

                        // Fecho o arquivo antes de sair da função para liberar o recurso.
                        fclose(fp);

                        // Retorno NULL para indicar que a operação não foi realizada.
                        return NULL;
                    }

                    // Se chegou aqui, significa que o operador logado é o dono da produtora.
                    // Então posso atualizar os dados dela no próprio arquivo.

                    // fseek move o cursor do arquivo para a posição correta para sobrescrever os dados.
                    // Como já li o registro inteiro, preciso voltar exatamente o tamanho de uma Produtora
                    // (sizeof(Produtora)) para regravar por cima da mesma posição.
                    fseek(fp, -(long)sizeof(Produtora), SEEK_CUR);

                    // Aqui mantenho os dados que não podem ser alterados (CNPJ e id_logado)
                    // Copio o CNPJ original e o ID do operador criador para os novos dados.
                    strcpy(novos_dados->cnpj, p.cnpj);
                    novos_dados->id_logado = p.id_logado;

                    // Agora faço a escrita dos novos dados no mesmo local do arquivo.
                    // fwrite grava o conteúdo da struct 'novos_dados' diretamente no arquivo binário.
                    if (fwrite(novos_dados, sizeof(Produtora), 1, fp) != 1) {
                        // Caso a gravação falhe (por exemplo, erro de disco ou permissão),
                        // uso perror para mostrar o motivo.
                        perror("Erro ao escrever produtora atualizada em produtoras.bin");
                        fclose(fp); // Fecho o arquivo mesmo assim.
                        return NULL;
                    }

                    // Se chegou até aqui, significa que a atualização foi concluída com sucesso!
                    // Fecho o arquivo porque não preciso mais manipular o binário.
                    fclose(fp);

                    // Mostro uma mensagem informando que a produtora foi atualizada corretamente.
                    printf("Produtora %s atualizada em BINARIO!\n", novos_dados->nome_fantasia);

                    // Retorno a struct de novos dados atualizada para confirmar o sucesso.
                    return novos_dados;
                }
            }

            // Se o loop terminar e não encontrar o CNPJ, fecho o arquivo de qualquer forma.
            fclose(fp);

            // Agora verifico se a flag "encontrado" permaneceu 0 (ou seja, produtora não localizada)
            if (!encontrado) {
                // Nesse caso, exibo uma mensagem indicando que não encontrei a produtora no arquivo binário.
                // Aqui faço a formatação visual do CNPJ (tipo 12.345.678/0001-99) apenas para deixar a mensagem mais legível.
                printf(
                    "Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em BINARIO",
                    cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                    cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                    cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                    cnpj_busca[9], cnpj_busca[10], cnpj_busca[11],
                    cnpj_busca[12], cnpj_busca[13]
                );
            }

            // Retorno NULL para indicar que não houve atualização (ou não foi encontrada).
            return NULL;
        }
    }
    return NULL;
}


void buscar_e_exibir_produtora(const char* cnpj_busca, TipoArmazenamento tipo) {
    // Verifica se o CPF/CNPJ passado é válido
    if (!cnpj_busca) {
        exibir_mensagem("+--------------------------+\n");
        exibir_mensagem("| CNPJ invalido!       |\n");
        exibir_mensagem("+--------------------------+\n");
        return;
    }

    int operador_atual = get_operador_logado(); // obtém o ID do operador atualmente logado
    Produtora* produtora = NULL; // ponteiro que armazenará a proddutora encontrado

    // Escolhe o tipo de armazenamento
    switch (tipo) {

        // =========================
        // Caso 1: busca em memória
        // =========================
        case MEMORIA: {
            for (int i = 0; i < qtd; i++) {
                // Compara o CNPJ com o buscado
                if (strcmp(produtoras_memoria[i].cnpj, cnpj_busca) == 0) {
                    // Verifica se o produtora pertence ao operador logado
                    if (produtoras_memoria[i].id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para visualizar esta produtora.\n");
                        return; // sai sem exibir nada
                    }

                    // Se o operador for o mesmo, exibe os dados do produtora com a funçao do view
                    ver_produtora(&produtoras_memoria[i]);
                    return;
                }
            }
            break;
        }

        // ==============================
        // Caso 2: busca em arquivo texto
        // ==============================
        case TEXTO: {
            FILE* fp = fopen("produtoras.txt", "r");
            if (!fp) {
                perror("Erro ao abrir produtoras.txt");
                return;
            }

            produtora = malloc(sizeof(Produtora));
            char linha[512];

            while (fgets(linha, sizeof(linha), fp)) {
                // Lê todos os campos, incluindo o id_logado no final
                sscanf(
                        linha,
                        "%49[^;];%49[^;];%14[^;];%19[^;];%99[^;];%14[^;];%49[^;];%49[^;];%14[^;];%f;%d",
                        produtora->nome_fantasia,        
                        produtora->razao_social,         
                        produtora->inscricao_estadual,   
                        produtora->cnpj,                 
                        produtora->endereco_completo,    
                        produtora->tel,                  
                        produtora->email,                
                        produtora->nome_resp,            
                        produtora->tel_resp,             
                        &produtora->lucro,               
                        &produtora->id_logado            
                    ); 

                // Compara CNPJ
                if (strcmp(produtora->cnpj, cnpj_busca) == 0) {
                    // Verifica se o operador atual é o dono do produtora
                    if (produtora->id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para visualizar esta produtora.\n");
                        fclose(fp);
                        free(produtora);
                        return;
                    }

                    // Exibe os dados da produtora permitido atraves da funçao do view
                    ver_produtora(produtora);
                    fclose(fp);
                    free(produtora);
                    return;
                }
            }

            fclose(fp);
            free(produtora);
            produtora = NULL;
            break;
        }

        // ==================================
        // Caso 3: busca em arquivo binário
        // ==================================
        case BINARIO: {
            FILE* fp = fopen("produtoras.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir produtoras.bin");
                return;
            }

            produtora = malloc(sizeof(Produtora));

            // Lê produtora por produtora do arquivo binário
            while (fread(produtora, sizeof(Produtora), 1, fp) == 1) {
                produtora->cnpj[sizeof(produtora->cnpj) - 1] = '\0';

                // Compara CPF/CNPJ
                if (strcmp(produtora->cnpj, cnpj_busca) == 0) {
                    // Verifica se o operador atual é o dono do produtora
                    if (produtora->id_logado != operador_atual) {
                        printf("Erro: voce nao tem permissao para visualizar esta produtora.\n");
                        fclose(fp);
                        free(produtora);
                        return;
                    }

                    // Exibe o produtora encontrado
                    ver_produtora(produtora);
                    fclose(fp);
                    free(produtora);
                    return;
                }
            }

            fclose(fp);
            free(produtora);
            produtora = NULL;
            break;
        }

        default:
            exibir_mensagem("Tipo de armazenamento invalido!\n");
            return;
    

    // Caso o produtora não seja encontrado
    if (!produtora) {
        printf("+--------------------------+\n");
        printf("| Produtora nao encontrado!  |\n");
        printf("+--------------------------+\n");
    }
}
}
//funçao para buscar e exibir diretamente uma produtora pelo CNPJ e tipo de armazenamento
// void buscar_e_exibir_produtora(const char* cnpj_busca, TipoArmazenamento tipo) {
//     // Verifico se o CNPJ passado é nulo (inválido)
//     if (!cnpj_busca) {
//         exibir_mensagem("+--------------------------+\n");
//         exibir_mensagem("| CNPJ inválido!       |\n");
//         exibir_mensagem("+--------------------------+\n");
//         return; // se for inválido, saio da função
//     }

//     Produtora* produtora = NULL; // Ponteiro que vai armazenar a produtora encontrada, se existir

//     // Escolho o tipo de armazenamento usado
//     switch (tipo) {

//         // =========================
//         // Caso 1: busca em memória
//         // =========================
//         case MEMORIA: {
//             // Percorro o array de produtoras em memória
//             for (int i = 0; i < qtd; i++) {
//                 // Comparo o CNPJ de cada produtora com o CNPJ digitado
//                 if (strncmp(produtoras_memoria[i].cnpj, cnpj_busca, sizeof(produtoras_memoria[i].cnpj)) == 0) {
//                     // Se encontrar, guardo o endereço da produtora encontrada
//                     produtora = &produtoras_memoria[i];
//                     // Exibo as informações dessa produtora
//                     ver_produtora(produtora);
//                     break; // paro o loop, pois já encontrei a produtora
//                 }
//             }
//             break;
//         }

//         // ==============================
//         // Caso 2: busca em arquivo texto
//         // ==============================
//         case TEXTO: {
//             // Abro o arquivo texto para leitura
//             FILE* fp = fopen("produtoras.txt", "r");
//             if (!fp) {
//                 // Se não conseguir abrir, mostro o erro e saio
//                 perror("Erro ao abrir produtoras.txt");
//                 return;
//             }

//             // Aloco memória para armazenar temporariamente uma produtora
//             produtora = malloc(sizeof(Produtora));
//             char linha[512]; // buffer para armazenar cada linha do arquivo

//             // Leio o arquivo linha por linha
//             while (fgets(linha, sizeof(linha), fp)) {
//                 // Extraio os campos separados por ponto e vírgula
//                 sscanf(linha, "%49[^;];%99[^;];%13[^;];%14[^;];%99[^;];%11[^;];%49[^;];%49[^;];%11[^;];%f",
//                     produtora->nome_fantasia,
//                     produtora->razao_social,
//                     produtora->inscricao_estadual,
//                     produtora->cnpj,
//                     produtora->endereco_completo,
//                     produtora->tel,
//                     produtora->email,
//                     produtora->nome_resp,
//                     produtora->tel_resp,
//                     &produtora->lucro
//                 );

//                 // Comparo o CNPJ lido com o CNPJ buscado
//                 if (strncmp(produtora->cnpj, cnpj_busca, sizeof(produtora->cnpj)) == 0) {
//                     // Se encontrar, fecho o arquivo e exibo a produtora
//                     fclose(fp);
//                     ver_produtora(produtora);
//                     free(produtora); // libero a memória alocada
//                     return; // retorno, pois já encontrei a produtora
//                 }
//             }

//             // Se chegar aqui, a produtora não foi encontrada
//             fclose(fp);   // fecho o arquivo
//             free(produtora); // libero a memória usada
//             produtora = NULL; // deixo o ponteiro nulo
//             break;
//         }

//         // =================================
//         // Caso 3: busca em arquivo binário
//         // =================================
//         case BINARIO: {
//             // Abro o arquivo binário para leitura
//             FILE* fp = fopen("produtoras.bin", "rb");
//             if (!fp) {
//                 // Se não conseguir abrir, mostro o erro e saio
//                 perror("Erro ao abrir produtoras.bin");
//                 return;
//             }

//             // Aloco memória para armazenar temporariamente uma produtora
//             produtora = malloc(sizeof(Produtora));

//             // Leio o arquivo binário produtora por produtora
//             while (fread(produtora, sizeof(Produtora), 1, fp) == 1) {
//                 // Garante que a string do CNPJ termina em '\0'
//                 produtora->cnpj[sizeof(produtora->cnpj)-1] = '\0';

//                 // Comparo o CNPJ lido com o CNPJ buscado
//                 if (strncmp(produtora->cnpj, cnpj_busca, sizeof(produtora->cnpj)) == 0) {
//                     // Se encontrar, fecho o arquivo e exibo as informações
//                     fclose(fp);
//                     ver_produtora(produtora);
//                     free(produtora); // libero a memória alocada
//                     return; // retorno, pois já encontrei a produtora
//                 }
//             }

//             // Se terminar o arquivo e não encontrar, libero tudo
//             fclose(fp);
//             free(produtora);
//             produtora = NULL;
//             break;
//         }

//         // Caso o tipo de armazenamento não seja reconhecido
//         default:
//             exibir_mensagem("Tipo de armazenamento inválido!\n");
//             return;
//     }

//     // Se nenhum produtora foi encontrado nos casos acima, mostro mensagem padrão
//     if (!produtora) {
//         printf("+--------------------------+\n");
//         printf("| Produtora não encontrada!  |\n");
//         printf("+--------------------------+\n");
//     }
// }


// Função para deletar uma produtora a partir do CNPJ e do tipo de armazenamento
void deletar_produtora(const char* cnpj_busca, TipoArmazenamento tipo) {
    // Verifico se o parâmetro cnpj_busca é nulo antes de continuar
    if (!cnpj_busca) {
        printf("CNPJ inválido!\n");
        return;
    }

    // Uso o switch para tratar cada tipo de armazenamento (memória, texto e binário)
    switch (tipo) {

        // =====================================================
        // Caso 1 — Deletar produtora armazenada em MEMÓRIA
        // =====================================================
        case MEMORIA: {
            int encontrado = 0; // variável para controlar se o produtora foi achado

            // Percorro o vetor global de produtoras armazenados em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ do produtora atual com o CNPJ digitado
                if (strncmp(produtoras_memoria[i].cnpj, cnpj_busca, sizeof(produtoras_memoria[i].cnpj)) == 0) {

                    // Quando encontro o produtora, preciso removê-la deslocando os próximos para trás
                    for (int j = i; j < qtd - 1; j++) {
                        produtoras_memoria[j] = produtoras_memoria[j + 1];
                    }

                    // Reduzo a quantidade total de produtoras armazenados
                    qtd--;

                    // Marco que encontrei o produtora
                    encontrado = 1;

                    printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c deletada em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);
                    break; // saio do laço, já deletei
                }
            }

            // Caso o produtora não tenha sido encontrado, aviso o usuário
            if (!encontrado)
                 printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c nao encontrada em MEMÓRIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);

            break;
        }

        // =====================================================
        // Caso 2 — Deletar produtora de ARQUIVO TEXTO
        // =====================================================
        case TEXTO: {
            // Abro o arquivo de produtoras em modo leitura
            FILE* fp = fopen("produtoras.txt", "r");
            if (!fp) {
                perror("Erro ao abrir produtoras.txt");
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
            int deletado = 0;  // controle se o produtora foi removido
            Produtora produtora;   // struct temporária para armazenar os dados lidos

            // Leio o arquivo original linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Uso sscanf para quebrar os dados lidos separados por ';'
                sscanf(linha, "%49[^;];%99[^;];%13[^;];%14[^;];%99[^;];%11[^;];%49[^;];%49[^;];%11[^;];%f",
                    produtora.nome_fantasia,
                    produtora.razao_social,
                    produtora.inscricao_estadual,
                    produtora.cnpj,
                    produtora.endereco_completo,
                    produtora.tel,
                    produtora.email,
                    produtora.nome_resp,
                    produtora.tel_resp,
                    &produtora.lucro
                );

                // Se o CNPJ não for o que quero excluir, regravo essa linha no arquivo temporário
                if (strncmp(produtora.cnpj, cnpj_busca, sizeof(produtora.cnpj)) != 0) {
                    fprintf(temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%f\n",
                        produtora.nome_fantasia,
                        produtora.razao_social,
                        produtora.inscricao_estadual,
                        produtora.cnpj,
                        produtora.endereco_completo,
                        produtora.tel,
                        produtora.email,
                        produtora.nome_resp,
                        produtora.tel_resp,
                        produtora.lucro
                    );
                } else {
                    // Caso o CNPJ seja o que quero deletar, apenas marco a exclusão
                    deletado = 1;
                }
            }


            // Fecho os arquivos abertos
            fclose(fp);
            fclose(temp);

            // Excluo o arquivo antigo e renomeio o temporário como o novo "produtoras.txt"
            remove("produtoras.txt");
            rename("temp.txt", "produtoras.txt");

            // Exibo mensagem informando o resultado da operação
            if (deletado)
                printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c deletada em ARQUIVO TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);
            else
                printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada no ARQUIVO TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);

            break;
        }

        // =====================================================
        // Caso 3 — Deletar de ARQUIVO BINÁRIO
        // =====================================================
        case BINARIO: {
            // Abro o arquivo binário original em modo leitura binária
            FILE* fp = fopen("produtoras.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir produtoras.bin");
                return;
            }

            // Crio um arquivo temporário para regravar os produtoras que continuarão existindo
            FILE* temp = fopen("temp.bin", "wb");
            if (!temp) {
                perror("Erro ao criar temp.bin");
                fclose(fp);
                return;
            }

            Produtora produtora;   // struct temporária para armazenar a produtora lida
            int deletado = 0;  // controle se alguma produtora foi realmente removida

            // Leio cada produtora do arquivo binário original
            while (fread(&produtora, sizeof(Produtora), 1, fp) == 1) {
                // Garanto que o CNPJ termina com '\0' para evitar erro de comparação
                produtora.cnpj[sizeof(produtora.cnpj) - 1] = '\0';

                // Se o CNPJ não for o que deve ser deletado, gravo essa produtora no arquivo temporário
                if (strncmp(produtora.cnpj, cnpj_busca, sizeof(produtora.cnpj)) != 0) {
                    fwrite(&produtora, sizeof(Produtora), 1, temp);
                } else {
                    // Caso o CNPJ seja o da produtora a excluir, apenas marco que deletei
                    deletado = 1;
                }
            }

            // Fecho os arquivos abertos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo temporário atualizado
            remove("produtoras.bin");
            rename("temp.bin", "produtoras.bin");

            // Exibo mensagens de status ao usuário
            if (deletado)
                 printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c deletada em ARQUIVO BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);
            else
                printf("Produtora com CNPJ  %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada no ARQUIVO BINÁRIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                       cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                       cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                       cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                       cnpj_busca[13]);

            break;
        }

        // =====================================================
        // Caso padrão — tipo de armazenamento inválido
        // =====================================================
        default:
            printf("Tipo de armazenamento inválido!\n");
            break;
    }
}