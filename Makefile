# Variáveis para o compilador e as opções de compilação
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Nome do arquivo executável e caminhos de diretórios
EXECUTABLE = tp2.out
EXEC_PATH = bin/$(EXECUTABLE)
OBJ_PATH = obj
SRC_PATH = src
INC_PATH = include
TEST_EXECUTABLE = test_all.out
TEST_EXEC_PATH = bin/$(TEST_EXECUTABLE)

# Arquivos de código-fonte e objetos
SOURCES = $(wildcard $(SRC_PATH)/*.c)    # Todos os .c no diretório src/
TEST_SOURCES = test/test_all.c $(filter-out src/main.c, $(SOURCES)) # Teste sem main.c
OBJECTS = $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SOURCES)) # Objetos gerados

# A primeira regra é a que será executada quando não se especifica uma regra
all: $(EXEC_PATH)

# Como construir o executável final
$(EXEC_PATH): $(OBJECTS)
	@mkdir -p bin
	$(CC) $(OBJECTS) -o $@

# Como construir cada arquivo objeto
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(OBJ_PATH) bin

# Regra para executar o programa principal
run: $(EXEC_PATH)
	./$(EXEC_PATH)

# Regra para os testes
test: $(TEST_EXEC_PATH)
	@./$(TEST_EXEC_PATH)

# Como construir o executável de testes
$(TEST_EXEC_PATH): $(TEST_SOURCES)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@
