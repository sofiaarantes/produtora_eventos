# ==============================
# MAKEFILE PORTÁVEL MVC EM C
# ==============================

# Compilador
CC = gcc
CFLAGS = -Wall -Wextra -I./model -I./view -I./controller

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

# Lista de objetos
OBJ = main.o \
      model$(SEP)cliente$(SEP)cliente.o \
      view$(SEP)cliente$(SEP)cliente_view.o \
      controller$(SEP)cliente$(SEP)cliente_controller.o

# Regra padrão
all: pre_clean $(TARGET)

# Limpeza antes de compilar
pre_clean:
	-$(RM) $(TARGET) $(OBJ) > $(NULLDEV) 2>&1

# Linkagem do executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilação de cada objeto
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

model$(SEP)cliente$(SEP)cliente.o: model$(SEP)cliente$(SEP)cliente.c model$(SEP)cliente$(SEP)cliente.h
	$(CC) $(CFLAGS) -c model$(SEP)cliente$(SEP)cliente.c -o model$(SEP)cliente$(SEP)cliente.o

view$(SEP)cliente$(SEP)cliente_view.o: view$(SEP)cliente$(SEP)cliente_view.c view$(SEP)cliente$(SEP)cliente_view.h model$(SEP)cliente$(SEP)cliente.h
	$(CC) $(CFLAGS) -c view$(SEP)cliente$(SEP)cliente_view.c -o view$(SEP)cliente$(SEP)cliente_view.o

controller$(SEP)cliente$(SEP)cliente_controller.o: controller$(SEP)cliente$(SEP)cliente_controller.c controller$(SEP)cliente$(SEP)cliente_controller.h model$(SEP)cliente$(SEP)cliente.h view$(SEP)cliente$(SEP)cliente_view.h
	$(CC) $(CFLAGS) -c controller$(SEP)cliente$(SEP)cliente_controller.c -o controller$(SEP)cliente$(SEP)cliente_controller.o

# Limpeza manual
clean:
	-$(RM) $(OBJ) $(TARGET) > $(NULLDEV) 2>&1