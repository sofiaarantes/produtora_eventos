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
    if (!cliente) return NULL; // se o ponteiro for nulo, não posso salvar

    // Garante que o tipo de documento está correto (recalcula sempre)
    cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);
    if (cliente->tipo_doc == TIPO_DESCONHECIDO) {
        printf("Erro: documento invalido (%s). Deve ter 11 (CPF) ou 14 (CNPJ) digitos.\n", cliente->cpf_cnpj);
        printf("Cliente NAO salvo.\n");
        return NULL;
    }

    int novo_id = 1; // Id começa em 1 por padrão

    switch (tipo) {

        // =============================
        // CASO 1 - MEMÓRIA
        // =============================
        case MEMORIA: {
            if (qtd < MAX_CLIENTES) {
                if (qtd > 0)
                    novo_id = clientes_memoria[qtd - 1].id + 1;

                cliente->id = novo_id;

                // Recalcula e garante o tipo_doc ANTES de salvar
                cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);

                clientes_memoria[qtd] = *cliente;
                Cliente* salvo = &clientes_memoria[qtd];
                qtd++;

                const char* tipoTexto = (cliente->tipo_doc == TIPO_CPF) ? "CPF" :
                                        (cliente->tipo_doc == TIPO_CNPJ) ? "CNPJ" : "DESCONHECIDO";
                printf("Cliente %s salvo em MEMORIA! Tipo de documento: %s\n", cliente->nome, tipoTexto);
                return salvo;
            } else {
                printf("Erro: limite de clientes na memória atingido!\n");
                return NULL;
            }
        }

        // =============================
        // CASO 2 - ARQUIVO TEXTO
        // =============================
         // =============================
// CASO 2 - ARQUIVO TEXTO
// =============================
case TEXTO: {
    FILE* fp = fopen("clientes.txt", "r");
    if (fp) {
        char linha[512];
        Cliente tmp;

        // lê linha por linha (seguro, não trava)
        while (fgets(linha, sizeof(linha), fp)) {
            // tenta ler até 9 campos (último é tipo_doc)
            int lidos = sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%11[^;];%49[^;];%49[^;];%d",
                               &tmp.id, tmp.nome, &tmp.idade,
                               tmp.endereco_completo, tmp.cpf_cnpj,
                               tmp.tel, tmp.email, tmp.nome_contato,
                               (int*)&tmp.tipo_doc);

            if (lidos >= 1) {
                novo_id = tmp.id + 1;
            }
        }
        fclose(fp);
    }

    cliente->id = novo_id;

    // Recalcula tipo_doc (garante consistência)
    cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);

    // abre o arquivo em modo append
    fp = fopen("clientes.txt", "a");
    if (!fp) {
        perror("Erro ao abrir clientes.txt");
        return NULL;
    }

    // grava os dados formatados
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

    fclose(fp);

    const char* tipoTexto = (cliente->tipo_doc == TIPO_CPF) ? "CPF" :
                            (cliente->tipo_doc == TIPO_CNPJ) ? "CNPJ" : "DESCONHECIDO";
    printf("Cliente %s salvo em TEXTO! Tipo de documento: %s\n", cliente->nome, tipoTexto);
    return cliente;
}

        // =============================
        // CASO 3 - ARQUIVO BINÁRIO
        // =============================
        case BINARIO: {
            FILE* fp = fopen("clientes.bin", "rb");
            if (fp) {
                Cliente tmp;
                while (fread(&tmp, sizeof(Cliente), 1, fp) == 1) {
                    novo_id = tmp.id + 1;
                }
                fclose(fp);
            }
            cliente->id = novo_id;

            // Recalcula tipo_doc ANTES de gravar
            cliente->tipo_doc = identificar_documento(cliente->cpf_cnpj);

            cliente->cpf_cnpj[sizeof(cliente->cpf_cnpj) - 1] = '\0';
            cliente->tel[sizeof(cliente->tel) - 1] = '\0';

            fp = fopen("clientes.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return NULL;
            }

            if (fwrite(cliente, sizeof(Cliente), 1, fp) != 1) {
                perror("Erro ao gravar cliente em clientes.bin");
                fclose(fp);
                return NULL;
            }

            fclose(fp);

            const char* tipoTexto = (cliente->tipo_doc == TIPO_CPF) ? "CPF" :
                                    (cliente->tipo_doc == TIPO_CNPJ) ? "CNPJ" : "DESCONHECIDO";
            printf("Cliente %s salvo em BINÁRIO! Tipo de documento: %s\n", cliente->nome, tipoTexto);
            return cliente;
        }
    }

    return NULL;
}

//-------------------------------------
 // Função que atualiza um cliente de acordo com o tipo escolhido
            // recebe o CPF/CNPJ do cliente que quero atualizar e o tipo de armazenamento
            Cliente* atualizar_cliente(const char* cpf_cnpj_busca, Cliente* novos_dados, TipoArmazenamento tipo) {
                if (!cpf_cnpj_busca || !novos_dados) return NULL;

                switch (tipo) {

                    // Caso 1: atualizar na memória
                    case MEMORIA: {
                        int i;
                        for ( i = 0; i < qtd; i++) {
                            if (strcmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca) == 0) {
                                clientes_memoria[i] = *novos_dados; // sobrescreve os dados
                                strcpy(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca); 
                                // garantia de não mudar o CPF

                                printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c atualizado em MEMÓRIA!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                                cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                                cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                                cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
                                return &clientes_memoria[i];
                            }
                           
                        }
                        printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c não encontrado em MEMÓRIA!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                        cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                        cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                        cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
                                            return NULL;
                    }

                    // Caso 2: atualizar cliente em arquivo texto
                            case TEXTO: {
                                // Abro o arquivo original em modo leitura
                                FILE* fp = fopen("clientes.txt", "r");
                                if (!fp) {
                                    perror("Erro ao abrir clientes.txt");
                                    return NULL; // não conseguimos abrir, retorna NULL
                                }

                                // Crio um arquivo temporário onde vamos salvar os dados atualizados
                                FILE* temp = fopen("clientes_tmp.txt", "w");
                                if (!temp) {
                                    perror("Erro ao criar clientes_tmp.txt");
                                    fclose(fp);
                                    return NULL; // falha ao criar temporário
                                }

                                char linha[300];         // buffer para ler cada linha do arquivo original
                                int atualizado = 0;      // flag que indica se encontramos o cliente e atualizamos

                                // Percorro cada linha do arquivo original
                                while (fgets(linha, sizeof(linha), fp)) {
                                    Cliente c = {0}; // zera todos os campos antes de ler
                                    // Leitura dos campos da linha, separados por ';'
                                    sscanf(linha, "%d;%49[^;];%d;%99[^;];%19[^;];%14[^;];%49[^;];%49[^\n]",
                                        &c.id, c.nome, &c.idade, c.endereco_completo,
                                        c.cpf_cnpj, c.tel, c.email, c.nome_contato);

                                    // Removo possíveis '\n' ou '\r' de todos os campos lidos
                                    

                                    // Se o CPF lido for igual ao CPF que queremos atualizar
                                    if (strcmp(c.cpf_cnpj, cpf_cnpj_busca) == 0) {
                                        // Gravo os novos dados no arquivo temporário, mantendo o mesmo CPF
                                        fprintf(temp, "%d;%s;%d;%s;%s;%s;%s;%s\n",
                                            c.id,
                                            novos_dados->nome,
                                            novos_dados->idade,
                                            novos_dados->endereco_completo,
                                            c.cpf_cnpj,             // CPF não muda
                                            novos_dados->tel,
                                            novos_dados->email,
                                            novos_dados->nome_contato
                                        );
                                        atualizado = 1; // marca que o cliente foi atualizado
                                    } else {
                                        // Se não for o cliente que queremos atualizar, apenas regravo a linha original
                                        fputs(linha, temp);
                                    }
                                }

                                // Fecho os arquivos
                                fclose(fp);
                                fclose(temp);

                                // Substituo o arquivo original pelo temporário
                                remove("clientes.txt");
                                rename("clientes_tmp.txt", "clientes.txt");

                                // Mensagem de sucesso ou falha
                                if (atualizado) {
                                   printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c atualizado em TEXTO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                                    cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                                    cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                                    cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
                                    return novos_dados; // retorna os novos dados do cliente
                                } else {
                                    printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c não encontrado em TEXTO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                                    cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                                    cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                                    cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
                                    return NULL; // cliente não encontrado
                                }
                            }
  
                            case BINARIO: {
                                // Abro o arquivo 'clientes.bin' em modo leitura + escrita binária
                                FILE *fp = fopen("clientes.bin", "rb+");
                                if (!fp) {
                                    // Se não conseguir abrir, mostro o erro do sistema e retorno NULL
                                    perror("Erro ao abrir clientes.bin");
                                    return NULL;
                                }

                                Cliente c; // Estrutura temporária para ler os clientes do arquivo

                                // Percorro todo o arquivo cliente por cliente
                                while (fread(&c, sizeof(Cliente), 1, fp) == 1) {
                                    // Garante que o CPF lido do arquivo termina com '\0', evitando lixo
                                    c.cpf_cnpj[sizeof(c.cpf_cnpj)-1] = '\0';

                                    // Comparo o CPF lido com o CPF que queremos atualizar
                                    // Uso strncmp com o tamanho total do campo (12) para incluir o '\0'
                                    if (strncmp(c.cpf_cnpj, cpf_cnpj_busca, sizeof(c.cpf_cnpj)) == 0) {

                                        // Volto o ponteiro do arquivo para o início deste registro
                                        // para sobrescrever com os novos dados
                                        if (fseek(fp, -(long)sizeof(Cliente), SEEK_CUR) != 0) {
                                            perror("Erro no fseek durante atualização");
                                            fclose(fp);
                                            return NULL;
                                        }

                                        // Mantém o CPF original do cliente, pois não pode ser alterado
                                        strncpy(novos_dados->cpf_cnpj, c.cpf_cnpj, sizeof(novos_dados->cpf_cnpj));
                                        novos_dados->cpf_cnpj[sizeof(novos_dados->cpf_cnpj)-1] = '\0';

                                        // Gravo a struct atualizada no arquivo no lugar do antigo registro
                                        if (fwrite(novos_dados, sizeof(Cliente), 1, fp) != 1) {
                                            perror("Erro ao sobrescrever cliente em clientes.bin");
                                            fclose(fp);
                                            return NULL;
                                        }

                                        // Fecho o arquivo e informo que a atualização deu certo
                                        fclose(fp);
                                        printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c atualizado em BINÁRIO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                                        cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                                        cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                                        cpf_cnpj_busca[9], cpf_cnpj_busca[10]);

                                        // Retorno o ponteiro para os novos dados
                                        return novos_dados;
                                    }
                                }

                                // Se chegar aqui, significa que não encontrei o CPF no arquivo
                                fclose(fp);
                                printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c não encontrado em BINÁRIO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                                    cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                                    cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                                    cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
                                return NULL;
                            }

        }
        return NULL; // Caso inválido
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
    // Verifico se o CPF/CNPJ passado é nulo (inválido)
    if (!cpf_cnpj_busca) {
        exibir_mensagem("+--------------------------+\n");
        exibir_mensagem("| CPF/CNPJ inválido!       |\n");
        exibir_mensagem("+--------------------------+\n");
        return; // se for inválido, saio da função
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
                // Se não conseguir abrir, mostro o erro e saio
                perror("Erro ao abrir clientes.txt");
                return;
            }

            // Aloco memória para armazenar temporariamente um cliente
            cliente = malloc(sizeof(Cliente));
            char linha[512]; // buffer para armazenar cada linha do arquivo

            // Leio o arquivo linha por linha
            while (fgets(linha, sizeof(linha), fp)) {
                // Extraio os campos separados por ponto e vírgula
                sscanf(linha, "%d;%49[^;];%d;%99[^;];%11[^;];%11[^;];%49[^;];%49[^\n]",
                    &cliente->id,
                    cliente->nome,
                    &cliente->idade,
                    cliente->endereco_completo,
                    cliente->cpf_cnpj,
                    cliente->tel,
                    cliente->email,
                    cliente->nome_contato
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

            // Se chegar aqui, o cliente não foi encontrado
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
                // Se não conseguir abrir, mostro o erro e saio
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

            // Se terminar o arquivo e não encontrar, libero tudo
            fclose(fp);
            free(cliente);
            cliente = NULL;
            break;
        }

        // Caso o tipo de armazenamento não seja reconhecido
        default:
            exibir_mensagem("Tipo de armazenamento inválido!\n");
            return;
    }

    // Se nenhum cliente foi encontrado nos casos acima, mostro mensagem padrão
    if (!cliente) {
        printf("+--------------------------+\n");
        printf("| Cliente não encontrado!  |\n");
        printf("+--------------------------+\n");
    }
}
// Função para deletar um cliente a partir do CPF/CNPJ e do tipo de armazenamento
void deletar_cliente(const char* cpf_cnpj_busca, TipoArmazenamento tipo) {
    // Verifico se o parâmetro cpf_cnpj_busca é nulo antes de continuar
    if (!cpf_cnpj_busca) {
        printf("CPF/CNPJ inválido!\n");
        return;
    }

    // Uso o switch para tratar cada tipo de armazenamento (memória, texto e binário)
    switch (tipo) {

        // =====================================================
        // Caso 1 — Deletar cliente armazenado em MEMÓRIA
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

                    printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c deletado em MEMÓRIA!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                       cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                       cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                       cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
                    break; // saio do laço, já deletei
                }
            }

            // Caso o cliente não tenha sido encontrado, aviso o usuário
            if (!encontrado)
                 printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c nao encontrado em MEMÓRIA!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                       cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                       cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                       cpf_cnpj_busca[9], cpf_cnpj_busca[10]);

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

                // Se o CPF não for o que quero excluir, regravo essa linha no arquivo temporário
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
                printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c deletado em ARQUIVO TEXTO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                       cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                       cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                       cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
            else
                printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c não encontrado no ARQUIVO TEXTO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                       cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                       cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                       cpf_cnpj_busca[9], cpf_cnpj_busca[10]);

            break;
        }

        // =====================================================
        // Caso 3 — Deletar cliente de ARQUIVO BINÁRIO
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

                // Se o CPF não for o que deve ser deletado, gravo esse cliente no arquivo temporário
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
                 printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c deletado em ARQUIVO BINÁRIO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                       cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                       cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                       cpf_cnpj_busca[9], cpf_cnpj_busca[10]);
            else
                printf("Cliente com CPF %c%c%c.%c%c%c.%c%c%c-%c%c não encontrado no ARQUIVO BINÁRIO!\n", cpf_cnpj_busca[0], cpf_cnpj_busca[1], cpf_cnpj_busca[2],
                       cpf_cnpj_busca[3], cpf_cnpj_busca[4], cpf_cnpj_busca[5],
                       cpf_cnpj_busca[6], cpf_cnpj_busca[7], cpf_cnpj_busca[8],
                       cpf_cnpj_busca[9], cpf_cnpj_busca[10]);

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

TipoDocumento identificar_documento(const char *doc) {
    int len = 0;

    // Conta apenas os dígitos
    for (int i = 0; doc[i] != '\0'; i++) {
        if (isdigit((unsigned char)doc[i])) {
            len++;
        }
    }

    // Garante que não ultrapasse o tamanho máximo esperado
    if (len == 11) {
        return TIPO_CPF;
    } else if (len == 14) {
        return TIPO_CNPJ;
    } else {
        return TIPO_DESCONHECIDO;
    }
}