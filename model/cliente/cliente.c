#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

#define MAX_CLIENTES 100
static Cliente clientes_memoria[MAX_CLIENTES]; // cada posição guarda um cliente
static int qtd = 0; // contador de quantos clientes já estão salvos na memória

// Função que cria e salva um cliente de acordo com o tipo escolhido
Cliente* criar_cliente(Cliente* cliente, TipoArmazenamento tipo) {
    if (!cliente) return NULL; // se o ponteiro for nulo, não posso salvar

    switch (tipo) {

        // Caso 1: salvar na memória
        case MEMORIA: {
            if (qtd < MAX_CLIENTES) {
                // copio todos os dados do cliente passado para o array de memória
                clientes_memoria[qtd] = *cliente;

                // crio um ponteiro que aponta para o cliente que acabei de salvar
                Cliente* salvo = &clientes_memoria[qtd];

                qtd++; // aumento o contador de clientes na memória

                printf("Cliente %s salvo em MEMÓRIA!\n", salvo->nome);
                return salvo; // retorno o endereço do cliente salvo
            } else {
                printf("Erro: limite de clientes na memória atingido!\n");
                return NULL;
            }
        }

        // Caso 2: salvar em arquivo texto
        case TEXTO: {
            // abro o arquivo em modo append para não sobrescrever os clientes anteriores
            FILE* fp = fopen("clientes.txt", "a");
            if (!fp) {
                perror("Erro ao abrir clientes.txt");
                return NULL;
            }

            // escrevo os dados do cliente no arquivo separados por ponto e vírgula
            fprintf(fp, "%d;%s;%d;%s;%s;%s;%s;%s\n",
                cliente->id,
                cliente->nome,
                cliente->idade,
                cliente->endereco_completo,
                cliente->cpf_cnpj,
                cliente->tel,
                cliente->email,
                cliente->nome_contato
            );

            fclose(fp); // fecho o arquivo
            printf("Cliente %s salvo em TEXTO!\n", cliente->nome);
            return cliente; // retorno o cliente que foi salvo
        }

        // Caso 3: salvar em arquivo binário
        case BINARIO: {
            // abro o arquivo em modo append binário
            FILE* fp = fopen("clientes.bin", "ab");
            if (!fp) {
                perror("Erro ao abrir clientes.bin");
                return NULL;
            }

            // gravo a struct inteira diretamente no arquivo
            fwrite(cliente, sizeof(Cliente), 1, fp);

            fclose(fp); // fecho o arquivo
            printf("Cliente %s salvo em BINÁRIO!\n", cliente->nome);
            return cliente; // retorno o cliente que foi salvo
        }

        // Caso inválido
        default:
            printf("Tipo de armazenamento inválido!\n");
            return NULL;
    }
}
//-------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

// ==============================
// Atualiza cliente que está na MEMÓRIA
// ==============================
void atualizar_cliente_memoria(Cliente* cliente_mem, 
                               const char* nome, int idade,
                               const char* endereco_completo, 
                               const char* tel,
                               const char* email, 
                               const char* nome_contato) {
    // Primeira coisa: verificar se o ponteiro é válido
    // Se não tiver cliente para atualizar, não faço nada
    if (!cliente_mem) return;

    // Atualizo o nome do cliente
    // Uso strncpy para evitar que o nome ultrapasse o tamanho do campo
    strncpy(cliente_mem->nome, nome, sizeof(cliente_mem->nome) - 1);
    cliente_mem->nome[sizeof(cliente_mem->nome) - 1] = '\0'; // garantido terminação nula

    // Atualizo a idade do cliente
    cliente_mem->idade = idade;

    // Atualizo o endereço completo
    strncpy(cliente_mem->endereco_completo, endereco_completo, sizeof(cliente_mem->endereco_completo) - 1);
    cliente_mem->endereco_completo[sizeof(cliente_mem->endereco_completo) - 1] = '\0';

    // Atualizo o telefone
    strncpy(cliente_mem->tel, tel, sizeof(cliente_mem->tel) - 1);
    cliente_mem->tel[sizeof(cliente_mem->tel) - 1] = '\0';

    // Atualizo o e-mail
    strncpy(cliente_mem->email, email, sizeof(cliente_mem->email) - 1);
    cliente_mem->email[sizeof(cliente_mem->email) - 1] = '\0';

    // Atualizo o nome do contato
    strncpy(cliente_mem->nome_contato, nome_contato, sizeof(cliente_mem->nome_contato) - 1);
    cliente_mem->nome_contato[sizeof(cliente_mem->nome_contato) - 1] = '\0';

    // Mensagem de confirmação: o cliente na memória já foi atualizado
    printf("Cliente %s atualizado na MEMÓRIA!\n", cliente_mem->nome);
}

// ==============================
// Atualiza cliente que está no ARQUIVO TEXTO
// ==============================
void atualizar_cliente_texto(Cliente* cliente_txt, 
                             const char* nome, int idade,
                             const char* endereco_completo, 
                             const char* tel,
                             const char* email, 
                             const char* nome_contato) {
    // Primeiro: checar se o ponteiro é válido
    if (!cliente_txt) return;

    // Vou abrir o arquivo original para ler todos os clientes
    FILE* fp = fopen("clientes.txt", "r");
    // E criar um arquivo temporário onde vou escrever os clientes atualizados
    FILE* temp = fopen("clientes_temp.txt", "w");
    if (!fp || !temp) {
        perror("Erro ao abrir arquivo de clientes");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return; // se deu erro, saio
    }

    // Leio cada cliente do arquivo original
    Cliente c;
    while (fscanf(fp, "%d;%49[^;];%d;%99[^;];%19[^;];%14[^;];%49[^;];%49[^\n]\n",
                  &c.id, c.nome, &c.idade, c.endereco_completo, c.cpf_cnpj,
                  c.tel, c.email, c.nome_contato) != EOF) {

        // Se for o cliente que quero atualizar (comparando pelo CPF/CNPJ)
        if (strcmp(c.cpf_cnpj, cliente_txt->cpf_cnpj) == 0) {
            // Atualizo todos os campos permitidos, mas NÃO mexo no CPF/CNPJ
            strncpy(c.nome, nome, sizeof(c.nome) - 1);
            c.nome[sizeof(c.nome) - 1] = '\0';
            c.idade = idade;
            strncpy(c.endereco_completo, endereco_completo, sizeof(c.endereco_completo) - 1);
            c.endereco_completo[sizeof(c.endereco_completo) - 1] = '\0';
            strncpy(c.tel, tel, sizeof(c.tel) - 1);
            c.tel[sizeof(c.tel) - 1] = '\0';
            strncpy(c.email, email, sizeof(c.email) - 1);
            c.email[sizeof(c.email) - 1] = '\0';
            strncpy(c.nome_contato, nome_contato, sizeof(c.nome_contato) - 1);
            c.nome_contato[sizeof(c.nome_contato) - 1] = '\0';
        }

        // Escrevo o cliente (atualizado ou não) no arquivo temporário
        fprintf(temp, "%d;%s;%d;%s;%s;%s;%s;%s\n",
                c.id, c.nome, c.idade, c.endereco_completo, c.cpf_cnpj,
                c.tel, c.email, c.nome_contato);
    }

    // Fecho os arquivos
    fclose(fp);
    fclose(temp);

    // Apago o arquivo original e renomeio o temporário para o nome original
    remove("clientes.txt");
    rename("clientes_temp.txt", "clientes.txt");

    // Mensagem de confirmação: o cliente no arquivo texto já foi atualizado
    printf("Cliente %s atualizado no ARQUIVO TEXTO!\n", nome);
}

// ==============================
// Atualiza cliente que está no ARQUIVO BINÁRIO
// ==============================
void atualizar_cliente_binario(Cliente* cliente_bin, 
                               const char* nome, int idade,
                               const char* endereco_completo, 
                               const char* tel,
                               const char* email, 
                               const char* nome_contato) {
    // Verifico se o ponteiro do cliente é válido
    if (!cliente_bin) return;

    // Abro o arquivo binário em leitura/escrita
    FILE* fp = fopen("clientes.bin", "r+b");
    if (!fp) {
        perror("Erro ao abrir clientes.bin");
        return;
    }

    Cliente c;
    // Leio cada cliente do arquivo
    while (fread(&c, sizeof(Cliente), 1, fp) == 1) {
        // Verifico se é o cliente que quero atualizar pelo CPF/CNPJ
        if (strcmp(c.cpf_cnpj, cliente_bin->cpf_cnpj) == 0) {
            // Atualizo os campos permitidos
            strncpy(c.nome, nome, sizeof(c.nome) - 1);
            c.nome[sizeof(c.nome) - 1] = '\0';
            c.idade = idade;
            strncpy(c.endereco_completo, endereco_completo, sizeof(c.endereco_completo) - 1);
            c.endereco_completo[sizeof(c.endereco_completo) - 1] = '\0';
            strncpy(c.tel, tel, sizeof(c.tel) - 1);
            c.tel[sizeof(c.tel) - 1] = '\0';
            strncpy(c.email, email, sizeof(c.email) - 1);
            c.email[sizeof(c.email) - 1] = '\0';
            strncpy(c.nome_contato, nome_contato, sizeof(c.nome_contato) - 1);
            c.nome_contato[sizeof(c.nome_contato) - 1] = '\0';

            // Volto o ponteiro do arquivo para sobrescrever o cliente antigo
            fseek(fp, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, fp);

            // Mensagem de confirmação
            printf("Cliente %s atualizado no ARQUIVO BINÁRIO!\n", nome);
            break; // saio do loop porque já atualizei o cliente
        }
    }

    fclose(fp); // fecho o arquivo
}

//------------------------------------

// Remove o cliente (libera memória)
void deletar_cliente(Cliente* cliente) {
    if (cliente) free(cliente);
}
//------------------------------------

// Retorna quantos clientes já estão salvos em memória
// Aqui eu só quero saber a quantidade de clientes que já cadastrei na memória
int get_qtd_clientes() { 
    return qtd; // retorno o valor do contador, que tá sendo incrementado toda vez que salvo um cliente
} 

// ==============================
// BUSCA CLIENTE NA MEMÓRIA
// ==============================
Cliente* buscar_cliente_por_cpf_memoria(const char* cpf_cnpj_busca) {
    // Aqui eu quero achar o cliente que está salvo na memória RAM
    // Percorro o array de clientes na memória
    int i;
    for ( i = 0; i < get_qtd_clientes; i++) {
        // comparo o CPF/CNPJ do cliente atual com o que o usuário digitou
        if (strcmp(clientes_memoria[i].cpf_cnpj, cpf_cnpj_busca) == 0) {
            // Achei o cliente que bate com o CPF/CNPJ
            // Vou retornar o ponteiro para ele para poder atualizar depois
            return &clientes_memoria[i];
        }
    }
    // Se percorrer todo o array e não achar o cliente, retorno NULL
    return NULL;
}

// ==============================
// BUSCA CLIENTE EM ARQUIVO TEXTO
// ==============================
Cliente* buscar_cliente_por_cpf_texto(const char* cpf_cnpj_busca) {
    // Preciso abrir o arquivo de texto que contém todos os clientes
    FILE* fp = fopen("clientes.txt", "r");
    if (!fp) {
        // Se não conseguir abrir, mostro o erro e retorno NULL
        perror("Erro ao abrir clientes.txt");
        return NULL;
    }

    // Vou ler os clientes um por um do arquivo
    // Uso static Cliente cliente_tmp para que o ponteiro continue válido
    static Cliente cliente_tmp;

    // Leio linha por linha do arquivo e separo os campos pelo ';'
    while (fscanf(fp, "%d;%49[^;];%d;%99[^;];%19[^;];%14[^;];%49[^;];%49[^\n]\n",
                  &cliente_tmp.id,
                  cliente_tmp.nome,
                  &cliente_tmp.idade,
                  cliente_tmp.endereco_completo,
                  cliente_tmp.cpf_cnpj,
                  cliente_tmp.tel,
                  cliente_tmp.email,
                  cliente_tmp.nome_contato) == 8) {

        // Comparo o CPF/CNPJ lido com o que o usuário digitou
        if (strcmp(cliente_tmp.cpf_cnpj, cpf_cnpj_busca) == 0) {
            // Achei o cliente! Fecho o arquivo e retorno o ponteiro para ele
            fclose(fp);
            return &cliente_tmp;
        }
    }

    // Se percorrer tudo e não achar, fecho o arquivo e retorno NULL
    fclose(fp);
    return NULL;
}

// ==============================
// BUSCA CLIENTE EM ARQUIVO BINÁRIO
// ==============================
Cliente* buscar_cliente_por_cpf_binario(const char* cpf_cnpj_busca) {
    // Abrindo o arquivo binário que contém os clientes
    FILE* fp = fopen("clientes.bin", "rb");
    if (!fp) {
        // Se não abrir, mostro erro e retorno NULL
        perror("Erro ao abrir clientes.bin");
        return NULL;
    }

    // Leio um registro de cada vez
    static Cliente cliente_tmp; // static para manter o ponteiro válido fora da função
    while (fread(&cliente_tmp, sizeof(Cliente), 1, fp) == 1) {
        // Comparo o CPF/CNPJ do cliente lido com o informado
        if (strcmp(cliente_tmp.cpf_cnpj, cpf_cnpj_busca) == 0) {
            // Achei o cliente! Fecho o arquivo e retorno o ponteiro
            fclose(fp);
            return &cliente_tmp;
        }
    }

    // Se não encontrar, fecho o arquivo e retorno NULL
    fclose(fp);
    return NULL;
}
