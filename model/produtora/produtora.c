#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtora.h"
#include "../../view/produtora/produtora_view.h"
#include "../../view/main/main_view.h"
#include "../../model/sessao/sessao.h"
#include "../../util/util.h"


#define MAX_PRODUTORAS 100
static Produtora produtoras_memoria[MAX_PRODUTORAS]; // cada posição guarda uma produtora
static int qtd = 0; // contador de quantas produtoras já estão salvas na memória


Produtora* criar_produtora(Produtora* produtora, TipoArmazenamento tipo) {
    // -------------------------------------------------------------------------
    // Primeiro eu verifico se o ponteiro passado é válido.
    // Se o ponteiro for NULL, significa que nem existe estrutura pra salvar.
    // Então eu retorno NULL logo de cara.
    // -------------------------------------------------------------------------
    if (!produtora) return NULL;

    // -------------------------------------------------------------------------
    // Agora eu começo as validações básicas.
    // Antes de salvar qualquer coisa, eu garanto que os dados principais
    // da produtora estão dentro dos padrões corretos.
    // -------------------------------------------------------------------------

    // O e-mail precisa obrigatoriamente conter "@gmail"
    if (validar_email(produtora->email) == 0) {
        printf("Erro: e-mail inválido. Deve conter '@gmail'.\n");
        printf("Produtora NÃO cadastrada.\n");
        return NULL;
    }

    // Telefone principal: deve ter 11 dígitos
    if (validar_tel(produtora->tel) == 0) {
        printf("Erro: telefone inválido. Deve conter 11 dígitos.\n");
        printf("Produtora NÃO cadastrada.\n");
        return NULL;
    }

    // Telefone do responsável: também deve ter 11 dígitos
    if (validar_tel(produtora->tel_resp) == 0) {
        printf("Erro: telefone do responsável inválido. Deve conter 11 dígitos.\n");
        printf("Produtora NÃO cadastrada.\n");
        return NULL;
    }

    // CNPJ: deve ter exatamente 14 dígitos
    if (validar_cnpj(produtora->cnpj) == 0) {
        printf("Erro: CNPJ inválido. Deve conter 14 dígitos.\n");
        printf("Produtora NÃO cadastrada.\n");
        return NULL;
    }

    // Inscrição estadual: precisa ter 13 dígitos
    if (validar_inscr(produtora->inscricao_estadual) == 0) {
        printf("Erro: Inscrição estadual inválida. Deve conter 13 dígitos.\n");
        printf("Produtora NÃO cadastrada.\n");
        return NULL;
    }

    // -------------------------------------------------------------------------
    // Aqui eu verifico se já existe alguma produtora cadastrada no tipo de
    // armazenamento atual (memória, texto ou binário).
    // Só pode existir UMA produtora em cada tipo de armazenamento.
    // -------------------------------------------------------------------------
    switch (tipo) {

        // ============================
        // CASO 1 - Memória
        // ============================
        case MEMORIA: {
            // Eu verifico se já existe alguma produtora no vetor.
            // Se qtd > 0, quer dizer que já tem uma cadastrada.
            if (qtd > 0) {
                printf("Erro: Já existe uma produtora cadastrada no sistema!\n");
                return NULL;
            }
            break;
        }

        // ============================
        // CASO 2 - Texto
        // ============================
        case TEXTO: {
            // Abro o arquivo de produtoras em modo leitura.
            FILE *fp_txt = fopen("produtoras.txt", "r");
            if (fp_txt) {
                // Aqui eu uso fgetc para tentar ler o primeiro caractere do arquivo.
                // fgetc() retorna EOF se o arquivo estiver vazio.
                // Se NÃO for EOF, significa que o arquivo tem conteúdo (já tem produtora salva).
                if (fgetc(fp_txt) != EOF) {
                    printf("Erro: Já existe uma produtora cadastrada no sistema!\n");
                    fclose(fp_txt);
                    return NULL;
                }
                // Fecho o arquivo depois de verificar.
                fclose(fp_txt);
            }
            break;
        }

        // ============================
        // CASO 3 - Binário
        // ============================
        case BINARIO: {
            // Abro o arquivo binário só pra leitura.
            FILE *fp_bin = fopen("produtoras.bin", "rb");
            if (fp_bin) {
                // Mesma lógica do texto: se o arquivo não estiver vazio,
                // fgetc vai ler algum byte antes de retornar EOF.
                if (fgetc(fp_bin) != EOF) {
                    printf("Erro: Já existe uma produtora cadastrada no sistema!\n");
                    fclose(fp_bin);
                    return NULL;
                }
                fclose(fp_bin);
            }
            break;
        }

        // Caso o tipo passado não seja nenhum dos válidos.
        default:
            printf("Erro: Tipo de armazenamento inválido!\n");
            return NULL;
    }

    // -------------------------------------------------------------------------
    // Aqui eu associo a produtora ao operador logado.
    // Assim eu sei qual usuário criou essa produtora no sistema.
    // -------------------------------------------------------------------------
    produtora->id_logado = get_operador_logado();

    // -------------------------------------------------------------------------
    // Agora vem a parte de salvar de fato a produtora no armazenamento escolhido.
    // -------------------------------------------------------------------------
    switch (tipo) {

        // ===============================================================
        // SALVAR EM MEMÓRIA
        // ===============================================================
        case MEMORIA: {
            // Verifico se ainda há espaço no vetor.
            if (qtd < MAX_PRODUTORAS) {
                // Copio os dados da produtora pro vetor global.
                produtoras_memoria[qtd] = *produtora;

                // Crio um ponteiro pro item recém-salvo (posição qtd do vetor).
                Produtora* salvo = &produtoras_memoria[qtd];

                // Aumento a contagem de produtoras salvas em memória.
                qtd++;

                // Mensagem de confirmação
                printf("Produtora %s salva em MEMÓRIA!\n", produtora->nome_fantasia);

                // Retorno o ponteiro pra produtora salva.
                return salvo;
            } else {
                printf("Erro: limite de produtoras na memória atingido!\n");
                return NULL;
            }
        }

        // ===============================================================
        // SALVAR EM ARQUIVO TEXTO
        // ===============================================================
        case TEXTO: {
            // Abro o arquivo no modo append (acrescentar no final).
            FILE* fp = fopen("produtoras.txt", "a");
            if (!fp) {
                perror("Erro ao abrir produtoras.txt");
                return NULL;
            }

            // Escrevo todos os dados da produtora separados por ponto e vírgula.
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

            // Fecho o arquivo.
            fclose(fp);

            // Aviso o usuário que deu certo.
            printf("Produtora %s salva em TEXTO!\n", produtora->nome_fantasia);

            return produtora;
        }

        // ===============================================================
        // SALVAR EM ARQUIVO BINÁRIO
        // ===============================================================
        case BINARIO: {
            // Abro o arquivo binário no modo append ("ab").
            FILE* fp = fopen("produtoras.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir produtoras.bin");
                return NULL;
            }

            // Escrevo a estrutura completa no arquivo binário.
            fwrite(produtora, sizeof(Produtora), 1, fp);

            // Fecho o arquivo.
            fclose(fp);

            // Confirmo o salvamento pro usuário.
            printf("Produtora %s salva em BINÁRIO!\n", produtora->nome_fantasia);

            return produtora;
        }

        // ===============================================================
        // CASO INVÁLIDO
        // ===============================================================
        default:
            printf("Erro: Tipo de armazenamento inválido!\n");
            return NULL;
    }
}

    
Produtora* atualizar_produtora(const char* cnpj_busca, Produtora* novos_dados, TipoArmazenamento tipo) {

    // Verifico se os parâmetros passados são válidos antes de qualquer coisa
    if (!cnpj_busca || !novos_dados) return NULL;

    //antes de salvar verifica se o email é valido
                if (validar_email(novos_dados->email) == 0) {
                    printf("Erro: e-mail invalido. Deve conter '@gmail'.\n");
                    printf("Produtora NAO atualizada\n");
                    return NULL;
                }
     //antes de salvar verifica se o tel é valido
                if (validar_tel(novos_dados->tel) == 0) {
                    printf("Erro: telefone invalido. Deve conter 11 digitos.\n");
                    printf("Produtora NAO atualizada\n");
                    return NULL;
                }
    //antes de salvar verifica se o tel resp é valido
                if (validar_tel(novos_dados->tel_resp) == 0) {
                    printf("Erro: telefone do responsavel invalido. Deve conter 11 digitos.\n");
                    printf("Produtora NAO atualizada\n");
                    return NULL;
                }
    //antes de salvar verifica se a inscriçao estadual é valido
                if (validar_inscr(novos_dados->inscricao_estadual) == 0) {
                    printf("Erro: Inscricao estadual invalida. Deve conter 13 digitos.\n");
                    printf("Produtora NAO atualizada\n");
                    return NULL;
                }


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
    // Primeiro, verifico se o parâmetro "cnpj_busca" é válido (ou seja, se não é NULL)
    if (!cnpj_busca) {
        // Se o CNPJ for inválido (por exemplo, o ponteiro for nulo),
        // exibo uma mensagem de erro formatada para o usuário.
        printf("+--------------------------+\n");
        printf("| CNPJ invalido!           |\n");
        printf("+--------------------------+\n");
        return; // E saio da função imediatamente
    }

    // Crio um ponteiro para armazenar a produtora que será encontrada durante a busca.
    // Começa como NULL, indicando que ainda não encontrei nada.
    Produtora* produtora = NULL;

    // ================================
    // ESCOLHA DO TIPO DE ARMAZENAMENTO
    // ================================
    // Dependendo do tipo de armazenamento escolhido (MEMORIA, TEXTO ou BINARIO),
    // o método de busca muda completamente.
    switch (tipo) {

        // ==================================================
        // CASO 1: BUSCA EM MEMÓRIA (dados carregados no RAM)
        // ==================================================
        case MEMORIA: {
            // Nesse caso, as produtoras estão armazenadas em um vetor global "produtoras_memoria".
            // A variável "qtd" representa o total de produtoras carregadas.
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ da produtora atual com o CNPJ buscado pelo usuário.
                if (strcmp(produtoras_memoria[i].cnpj, cnpj_busca) == 0) {
                    // Se o CNPJ for igual, encontrei a produtora.
                    // função para exibir os dados formatados da produtora encontrada.
                    ver_produtora(&produtoras_memoria[i]);
                    return; // Encerro a função, pois já exibi a produtora
                }
            }

            // Se o loop terminar sem encontrar nenhum CNPJ igual, apenas saio do case.
            printf("+------------------------------+\n");
            printf("| Produtora nao encontrada!    |\n");
            printf("+------------------------------+\n");
            break;
        }

        // ==========================================================
        // CASO 2: BUSCA EM ARQUIVO TEXTO (produtoras.txt)
        // ==========================================================
        case TEXTO: {
            // Abro o arquivo texto onde as produtoras estão salvas.
            FILE* fp = fopen("produtoras.txt", "r");
            if (!fp) {
                // Se não conseguir abrir o arquivo (por exemplo, ele não existe),
                // uso perror() para mostrar uma mensagem de erro com o motivo exato.
                perror("Erro ao abrir produtoras.txt");
                return;
            }

            // Aloco dinamicamente memória para armazenar temporariamente uma produtora.
            produtora = malloc(sizeof(Produtora));

            // Crio um buffer para armazenar cada linha lida do arquivo.
            char linha[512];

            // Leio o arquivo linha por linha até o final.
            while (fgets(linha, sizeof(linha), fp)) {
                // A função sscanf serve para extrair os campos da produtora a partir da linha.
                // Cada campo é separado por ponto e vírgula ';'
                sscanf(
                    linha,
                    "%49[^;];%49[^;];%14[^;];%19[^;];%99[^;];%14[^;];%49[^;];%49[^;];%14[^;];%f;%d",
                    produtora->nome_fantasia,        // Nome fantasia
                    produtora->razao_social,         // Razão social
                    produtora->inscricao_estadual,   // Inscrição estadual
                    produtora->cnpj,                 // CNPJ
                    produtora->endereco_completo,    // Endereço
                    produtora->tel,                  // Telefone
                    produtora->email,                // E-mail
                    produtora->nome_resp,            // Nome do responsável
                    produtora->tel_resp,             // Telefone do responsável
                    &produtora->lucro,               // Lucro
                    &produtora->id_logado            // ID do operador que cadastrou
                );

                // Agora comparo o CNPJ lido com o CNPJ que o usuário quer buscar.
                if (strcmp(produtora->cnpj, cnpj_busca) == 0) {
                    // Se for igual, encontrei a produtora no arquivo.

                    // função para exibir os dados da produtora encontrada.
                    ver_produtora(produtora);

                    // Fecho o arquivo e libero a memória usada.
                    fclose(fp);
                    free(produtora);
                    return; // Saio da função, pois já exibi a produtora
                }
            }

            // Se cheguei até aqui, é porque percorri todo o arquivo sem encontrar a produtora.
            fclose(fp);
            free(produtora);
            produtora = NULL; // deixo o ponteiro como NULL para indicar que não achei nada
            printf("+------------------------------+\n");
            printf("| Produtora nao encontrada!    |\n");
            printf("+------------------------------+\n");
            break;

            
        }

        // ==========================================================
        // CASO 3: BUSCA EM ARQUIVO BINÁRIO (produtoras.bin)
        // ==========================================================
        case BINARIO: {
            // Abro o arquivo binário apenas para leitura ("rb").
            // Aqui cada produtora foi salva diretamente como uma struct.
            FILE* fp = fopen("produtoras.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir produtoras.bin");
                return;
            }

            // Aloco memória para armazenar uma produtora temporariamente durante a leitura.
            produtora = malloc(sizeof(Produtora));

            // Leio uma produtora por vez do arquivo binário.
            while (fread(produtora, sizeof(Produtora), 1, fp) == 1) {
                // Garante que o campo de CNPJ termina com '\0', evitando erros de comparação.
                produtora->cnpj[sizeof(produtora->cnpj) - 1] = '\0';

                // Comparo o CNPJ da produtora lida com o buscado.
                if (strcmp(produtora->cnpj, cnpj_busca) == 0) {
                    // Se forem iguais, encontrei a produtora desejada.
                    // Exibo os dados formatados da produtora encontrada.
                    ver_produtora(produtora);

                    // Fecho o arquivo e libero a memória.
                    fclose(fp);
                    free(produtora);
                    return;
                }
            }

            // Caso percorra todo o arquivo e não encontre o CNPJ, fecho e libero tudo.
            fclose(fp);
            free(produtora); //limpo a memoria alocada
            produtora = NULL; //atribuo null ao ponteiro por precausao
            printf("+------------------------------+\n");
            printf("| Produtora nao encontrada!    |\n");
            printf("+------------------------------+\n");
            break;
        }

        // ==========================================================
        // CASO PADRÃO: tipo de armazenamento inválido
        // ==========================================================
        default:
            exibir_mensagem("Tipo de armazenamento invalido!\n");
            return;
    }
}

void deletar_produtora(const char* cnpj_busca, TipoArmazenamento tipo) {
    // Primeiro verifico se o parâmetro CNPJ recebido é válido
    // Se for nulo, já mostro uma mensagem de erro e saio da função.
    if (!cnpj_busca) {
        printf("+--------------------------+\n");
        printf("| CNPJ invalido!           |\n");
        printf("+--------------------------+\n");
        return;
    }

  

    // Uso o switch para tratar cada tipo de armazenamento (memória, texto ou binário)
    switch (tipo) {

        // =====================================================
        // Caso 1 — Deletar armazenado em MEMÓRIA
        // =====================================================
        case MEMORIA: {
            // Percorro o vetor global de produtoras armazenados em memória
            for (int i = 0; i < qtd; i++) {
                // Comparo o CNPJ digitado com o do produtora atual
                if (strcmp(produtoras_memoria[i].cnpj, cnpj_busca) == 0) {
                    // Achei o produtora, agora preciso remover o produtora do vetor
                    // Para isso, desloco todos os próximos elementos uma posição para trás
                    for (int j = i; j < qtd - 1; j++) {
                        produtoras_memoria[j] = produtoras_memoria[j + 1];
                    }

                    // Reduzo a contagem total de produtoras em memória
                    qtd--;

                    // Exibo mensagem de sucesso e saio da função
                    printf("Produtora %s deletada em MEMORIA!\n", produtoras_memoria[i].nome_fantasia);
                    return;
                }
            }

            // Se cheguei até aqui, o CNPJ não foi encontrado
            printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada em MEMORIA!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            return;
        }

        // =====================================================
        // Caso 2 — Deletar produtora em ARQUIVO TEXTO (.txt)
        // =====================================================
        case TEXTO: {
            // Abro o arquivo de produtoras em modo leitura
            FILE* fp = fopen("produtoras.txt", "r");
            if (!fp) {
                perror("Erro ao abrir produtoras.txt");
                return;
            }

            // Crio um arquivo temporário onde vou regravar apenas os produtoras válidos
            FILE* temp = fopen("temp.txt", "w");
            if (!temp) {
                perror("Erro ao criar temp.txt");
                fclose(fp);
                return;
            }

            char linha[512];    // Buffer usado para ler cada linha do arquivo
            Produtora produtora;    // Struct temporária para armazenar os dados lidos
            int deletado = 0;   // Variável de controle para saber se excluí algum produtora

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Tento extrair todos os campos
                 sscanf(
                        linha,
                        "%49[^;];%49[^;];%14[^;];%19[^;];%99[^;];%14[^;];%49[^;];%49[^;];%14[^;];%f;%d",
                        produtora.nome_fantasia,        
                        produtora.razao_social,         
                        produtora.inscricao_estadual,   
                        produtora.cnpj,                 
                        produtora.endereco_completo,    
                        produtora.tel,                  
                        produtora.email,                
                        produtora.nome_resp,            
                        produtora.tel_resp,             
                        &produtora.lucro,               
                        &produtora.id_logado            
                    ); 

                // Verifico se esta linha pertence ao produtora que quero deletar
                if (strcmp(produtora.cnpj, cnpj_busca) == 0) {
                    // Se achei o produtora
                    // simplesmente não escrevo essa linha no arquivo temporário
                        // Isso faz com que o produtora seja removido
                        deletado = 1;
                        continue; // Pulo para a próxima linha
                    
                }

                // Caso não seja o produtora que quero excluir, regravo a linha normalmente
                fputs(linha, temp);
            }

            // Fecho os arquivos originais e temporários
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo original pelo novo (sem o produtora deletado)
            remove("produtoras.txt");
            rename("temp.txt", "produtoras.txt");

            // Se o produtora foi encontrado e deletado, exibo sucesso
            if (deletado) {
                printf("Produtora %s deletada em ARQUIVO TEXTO!\n",produtora.nome_fantasia);
            } else {
                // Caso contrário, o produtora não existia
               printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada em TEXTO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
                        cnpj_busca[3], cnpj_busca[4], cnpj_busca[5],
                        cnpj_busca[6], cnpj_busca[7], cnpj_busca[8],
                        cnpj_busca[9], cnpj_busca[10], cnpj_busca[11], cnpj_busca[12],
                        cnpj_busca[13]);
            }
            return;
        }

        // =====================================================
        // Caso 3 — Deletar em ARQUIVO BINÁRIO (.bin)
        // =====================================================
        case BINARIO: {
            // Abro o arquivo binário original em modo leitura
            FILE* fp = fopen("produtoras.bin", "rb");
            if (!fp) {
                perror("Erro ao abrir produtoras.bin");
                return;
            }

            // Crio um arquivo temporário onde vou regravar apenas os produtoras válidos
            FILE* temp = fopen("temp.bin", "wb");
            if (!temp) {
                perror("Erro ao criar temp.bin");
                fclose(fp);
                return;
            }

            Produtora produtora;   // Struct temporária usada para leitura
            int deletado = 0;  // Controle de exclusão bem-sucedida

            // Leio cada produtora do arquivo binário
            while (fread(&produtora, sizeof(Produtora), 1, fp) == 1) {
                // Garante que o CPF termine corretamente
                produtora.cnpj[sizeof(produtora.cnpj) - 1] = '\0';

                // Comparo o CPF lido com o CPF digitado
                if (strcmp(produtora.cnpj, cnpj_busca) == 0) {
                    // Achei o produtora, agora preciso remover o produtora do vetor
                    // não escrevo esse produtora no arquivo temporário
                    deletado = 1;
                    continue;

            }

                // Caso não seja o produtora a deletar, regravo normalmente no arquivo temporário
                fwrite(&produtora, sizeof(Produtora), 1, temp);
            }

            // Fecho ambos os arquivos
            fclose(fp);
            fclose(temp);

            // Substituo o arquivo antigo pelo temporário atualizado
            remove("produtoras.bin");
            rename("temp.bin", "produtoras.bin");

            // Exibo a mensagem final apropriada
            if (deletado)
                printf("Produtora %s deletada em ARQUIVO BINARIO!\n",produtora.nome_fantasia);
            else
               printf("Produtora com CNPJ %c%c.%c%c%c.%c%c%c/%c%c%c%c-%c%c não encontrada em BINARIO!\n", cnpj_busca[0], cnpj_busca[1], cnpj_busca[2],
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
            printf("Tipo de armazenamento invalido!\n");
            return;
    }
}

// Lista todos as produtoras do tipo especificado. Retorna array alocado e seta out_count.
// Se não houver registros, retorna NULL e out_count = 0.
Produtora* listar_todos_produtoras(TipoArmazenamento tipo, int* out_count) {
    if (!out_count) return NULL;
    *out_count = 0;

    switch (tipo) {
        case MEMORIA: {
            if (qtd == 0) return NULL;
            Produtora* arr = malloc(sizeof(Produtora) * qtd);
            if (!arr) return NULL;
            for (int i = 0; i < qtd; i++) arr[i] = produtoras_memoria[i];
            *out_count = qtd;
            return arr;
        }
        case TEXTO: {
            FILE* fp = fopen("produtoras.txt", "r");
            if (!fp) return NULL;
            int count = 0;
            char linha[512];
            while (fgets(linha, sizeof(linha), fp)) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            Produtora* arr = malloc(sizeof(Produtora) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fgets(linha, sizeof(linha), fp) && idx < count) {
                Produtora p = {0};
                sscanf(linha, "%49[^;];%49[^;];%13[^;];%14[^;];%99[^;];%11[^;];%49[^;];%49[^;];%11[^;];%f",
                       p.nome_fantasia, p.razao_social, p.inscricao_estadual,
                       p.cnpj, p.endereco_completo, p.tel, p.email, p.nome_resp, p.tel_resp, &p.lucro);
                p.cnpj[sizeof(p.cnpj)-1] = '\0';
                arr[idx++] = p;
            }
            fclose(fp);
            *out_count = idx;
            return arr;
        }
        case BINARIO: {
            FILE* fp = fopen("produtoras.bin", "rb");
            if (!fp) return NULL;
            int count = 0;
            Produtora tmp;
            while (fread(&tmp, sizeof(Produtora), 1, fp) == 1) count++;
            if (count == 0) { fclose(fp); return NULL; }
            rewind(fp);
            Produtora* arr = malloc(sizeof(Produtora) * count);
            if (!arr) { fclose(fp); return NULL; }
            int idx = 0;
            while (fread(&arr[idx], sizeof(Produtora), 1, fp) == 1) {
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

// Remove todas as produtoras do armazenamento especificado. Retorna 1 em sucesso, 0 caso contrário.
int limpar_produtoras(TipoArmazenamento tipo) {
    switch (tipo) {
        case MEMORIA:
            qtd = 0;
            // opcional: memset(produtoras_memoria, 0, sizeof(produtoras_memoria));
            return 1;
        case TEXTO:
            remove("produtoras.txt");
            return 1;
        case BINARIO:
            remove("produtoras.bin");
            return 1;
        default:
            return 0;
    }
}