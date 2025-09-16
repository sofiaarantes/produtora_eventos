# ==============================
# MAKEFILE PARA PROJETO MVC EM C 
# ==============================

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -I./model -I./view -I./controller

# Objetos
OBJ = main.o \
      model/cliente/cliente.o \
      view/cliente/cliente_view.o \
      controller/cliente/cliente_controller.o

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    # Windows
    TARGET = produtora_eventos.exe
    RM = del /Q
    SEP = \\
else
    # Linux
    TARGET = produtora_eventos
    RM = rm -f
    SEP = /
endif

# Regra padrão: compilar tudo e gerar executável
all: pre_clean $(TARGET)

# Limpeza antes de compilar: objetos e executável
pre_clean:
	$(RM) $(TARGET) $(OBJ) 2>nul || true

# Linkagem dos objetos
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilação de cada arquivo .c em .o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

model/cliente/cliente.o: model/cliente/cliente.c model/cliente/cliente.h
	$(CC) $(CFLAGS) -c model/cliente/cliente.c -o model/cliente/cliente.o

view/cliente/cliente_view.o: view/cliente/cliente_view.c view/cliente/cliente_view.h model/cliente/cliente.h
	$(CC) $(CFLAGS) -c view/cliente/cliente_view.c -o view/cliente/cliente_view.o

controller/cliente/cliente_controller.o: controller/cliente/cliente_controller.c controller/cliente/cliente_controller.h model/cliente/cliente.h view/cliente/cliente_view.h
	$(CC) $(CFLAGS) -c controller/cliente/cliente_controller.c -o controller/cliente/cliente_controller.o

# Limpeza manual
clean:
	$(RM) $(OBJ) $(TARGET) 2>nul || true
