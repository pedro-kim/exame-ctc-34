CC = g++
CFLAGS = -std=c++11
SRC_DIR = src
TEST_DIR = test
INCLUDE_DIR = includes
BUILD_DIR = build
EXECUTABLE = main
TEST_EXECUTABLE = main_test

# Lista de arquivos fonte em src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Lista de arquivos fonte em test
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# Lista de arquivos de cabeçalho
INCLUDES = $(wildcard $(INCLUDE_DIR)/*.h)

# Lista de arquivos objetos gerados a partir dos arquivos fonte (src)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Lista de arquivos objetos gerados a partir dos arquivos fonte (test)
OBJS_TEST = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# Compilar todos os arquivos objetos e criar o executável de testes
$(TEST_EXECUTABLE): $(OBJS_TEST) $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilar todos os arquivos objetos e criar o executável
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilar cada arquivo fonte em um arquivo objeto (src)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

# Compilar cada arquivo fonte em um arquivo objeto (test)
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

# Limpar arquivos intermediários e executável
clean:
	rm -rf $(BUILD_DIR)/*.o $(EXECUTABLE) $(TEST_EXECUTABLE)

# Rodar os testes
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

# Rodar o programa
run: $(EXECUTABLE)
	./$(EXECUTABLE)
