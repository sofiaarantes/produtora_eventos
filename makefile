# ==============================
# MAKEFILE PORTÁVEL MVC EM C (com wildcard automático)
# ==============================

# Compilador 
CC = gcc
CFLAGS = -Wall -Wextra -I./model -I./view -I./controller -I./view/main

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    TARGET = produtora_eventos.exe
    RM = del /F /Q
    SEP = \\
    NULLDEV = NUL
else
    TARGET = produtora_eventos
    RM = rm -f
    SEP = /
    NULLDEV = /dev/null
endif

# ==============================
# Lista de fontes e objetos (automática, recursiva)
# ==============================
# Todos os arquivos .c na raiz e em qualquer subpasta até 4 níveis
SRC = $(wildcard *.c */*.c */*/*.c */*/*/*.c)

# Objetos correspondentes
OBJ = $(SRC:.c=.o)

# ==============================
# Regra padrão
all: pre_clean $(TARGET)

# Limpeza antes de compilar
pre_clean:
	-$(RM) $(TARGET) $(OBJ) > $(NULLDEV) 2>&1

# Linkagem do executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilação de cada objeto (automática)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza manual
clean:
	-$(RM) $(OBJ) $(TARGET) > $(NULLDEV) 2>&1
