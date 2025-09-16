# ==============================
# MAKEFILE PARA PROJETO MVC EM C
# ==============================

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -I./model -I./view -I./controller

# Objetos
OBJ = main.o \
      model/cliente.o \
      view/cliente_view.o \
      controller/cliente_controller.o

# Executável
TARGET = seed_Alfg2.exe

# Regra padrão: compilar tudo e gerar executável
all: pre_clean $(TARGET)

# Limpeza antes de compilar: .o e executável
pre_clean:
	if exist $(TARGET) del $(TARGET)
	if exist main.o del main.o
	if exist model\cliente.o del model\cliente.o
	if exist view\cliente_view.o del view\cliente_view.o
	if exist controller\cliente_controller.o del controller\cliente_controller.o

# Linkagem dos objetos
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilação de cada arquivo .c em .o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

model/cliente.o: model/cliente.c model/cliente.h
	$(CC) $(CFLAGS) -c model/cliente.c -o model/cliente.o

view/cliente_view.o: view/cliente_view.c view/cliente_view.h model/cliente.h
	$(CC) $(CFLAGS) -c view/cliente_view.c -o view/cliente_view.o

controller/cliente_controller.o: controller/cliente_controller.c controller/cliente_controller.h model/cliente.h view/cliente_view.h
	$(CC) $(CFLAGS) -c controller/cliente_controller.c -o controller/cliente_controller.o

# Limpeza completa (manual)
clean:
	del $(OBJ) $(TARGET)
