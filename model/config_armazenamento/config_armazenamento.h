#ifndef CONFIG_ARMAZENAMENTO_CONTROLLER_H
#define CONFIG_ARMAZENAMENTO_CONTROLLER_H

// Qual tipo de armazenamento o usuário escolheu
typedef enum {
    MEMORIA = 1,
    TEXTO,
    BINARIO
} TipoArmazenamento;

// Seta qual tipo de armazenamento o sistema usa
void set_armazenamento(TipoArmazenamento tipo);

// Retorna qual tipo de armazenamento o sistema usa
TipoArmazenamento get_armazenamento();

#endif
