# --- VARIABLES ---
CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Ilib
SRC_DIR  := src
OBJ_DIR  := obj
BIN      := p01_busquedas

SRCS := $(SRC_DIR)/nodo.cc $(SRC_DIR)/mapa.cc busquedas_no_informadas.cc
OBJS := $(OBJ_DIR)/nodo.o $(OBJ_DIR)/mapa.o $(OBJ_DIR)/busquedas_no_informadas.o

# --- REGLAS ---

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar .cc -> .o en carpeta obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla especial para busquedas_no_informadas.cc (está en raíz)
$(OBJ_DIR)/busquedas_no_informadas.o: busquedas_no_informadas.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c busquedas_no_informadas.cc -o $(OBJ_DIR)/busquedas_no_informadas.o

# Crear carpeta obj si no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
