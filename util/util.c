#include "util.h"   
#include <ctype.h>   // importo a biblioteca ctype.h porque vou usar a função isdigit() para checar se um caractere é número
#include <string.h>  // importo string.h porque vou usar strcpy() para copiar a string no final

// essa função recebe uma string e deixa nela somente os dígitos numéricos
void limpar_digitos(char* str) {
    char apenas_digitos[50]; // crio um array temporário que vai guardar só os números
    int j = 0;               // índice para preencher o array apenas_digitos

    // percorro cada caractere da string original
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        // se o caractere atual for um dígito (0-9), eu salvo no array apenas_digitos
        if (isdigit((unsigned char)str[i])) {
            apenas_digitos[j++] = str[i];
        }
    }

    // adiciono o terminador de string no final
    apenas_digitos[j] = '\0';

    // copio de volta os dígitos filtrados para a string original
    strcpy(str, apenas_digitos);
}
