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
    // Abro o arquivo em modo append para não sobrescrever os clientes anteriores
    FILE* fp = fopen("clientes.txt", "a");
    if (!fp) {
        perror("Erro ao abrir clientes.txt");
        return NULL;
    }

    // Escrevo os dados do cliente no arquivo separados por ponto e vírgula
    // Uso o limite máximo de caracteres baseado no tamanho do array da struct
    // Isso garante que o campo não ultrapasse o espaço definido
    fprintf(fp, "%d;%.49s;%d;%.99s;%.11s;%.11s;%.49s;%.49s\n",
        cliente->id,                
        cliente->nome,              
        cliente->idade,             
        cliente->endereco_completo, 
        cliente->cpf_cnpj,          
        cliente->tel,               
        cliente->email,             
        cliente->nome_contato       
    );

    fclose(fp); // Fecho o arquivo
    printf("Cliente %s salvo em TEXTO!\n", cliente->nome);
    return cliente; // Retorno o cliente que foi salvo
}
        // Caso 3: salvar cliente em arquivo binário
case BINARIO: {
    // garante que CPF e telefone terminam com '\0'
    cliente->cpf_cnpj[sizeof(cliente->cpf_cnpj) - 1] = '\0';
    cliente->tel[sizeof(cliente->tel) - 1] = '\0';

    // abre o arquivo em modo append binário
    FILE* fp = fopen("clientes.bin", "ab");
    if (!fp) {
        perror("Erro ao abrir clientes.bin");
        return NULL;
    }

    // grava a struct inteira no arquivo
    if (fwrite(cliente, sizeof(Cliente), 1, fp) != 1) {
        perror("Erro ao gravar cliente em clientes.bin");
        fclose(fp);
        return NULL;
    }

    fclose(fp);

    printf("Cliente %s com CPF:%s salvo em BINÁRIO!\n", cliente->nome, cliente->cpf_cnpj);
    return cliente;
}


     
    }
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

                                printf("Cliente %s atualizado em MEMÓRIA!\n", clientes_memoria[i].nome);
                                return &clientes_memoria[i];
                            }
                        }
                        printf("Cliente com CPF %s não encontrado em MEMÓRIA!\n", cpf_cnpj_busca);
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
                                    printf("Cliente com CPF %s atualizado em TEXTO!\n", cpf_cnpj_busca);
                                    return novos_dados; // retorna os novos dados do cliente
                                } else {
                                    printf("Cliente com CPF %s não encontrado em TEXTO!\n", cpf_cnpj_busca);
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
                        if (fseek(fp, -sizeof(Cliente), SEEK_CUR) != 0) {
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
                        printf("Cliente com CPF %s atualizado em BINÁRIO!\n", c.cpf_cnpj);

                        // Retorno o ponteiro para os novos dados
                        return novos_dados;
                    }
                }

                // Se chegar aqui, significa que não encontrei o CPF no arquivo
                fclose(fp);
                printf("Cliente com CPF %s não encontrado em BINÁRIO!\n", cpf_cnpj_busca);
                return NULL;
            }

        }
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
    for ( i = 0; i < get_qtd_clientes(); i++) {
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
