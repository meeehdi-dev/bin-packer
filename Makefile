CXX=clang++ -Wall -O3
INC_DIR=include/
OBJ_DIR=obj/
SRC_DIR=src/
BIN_DIR=bin/

CLASSES=main box boxinfo container layer node pack packer vec3usi
OBJS=$(addsuffix .o,$(addprefix $(OBJ_DIR),$(CLASSES)))

$(BIN_DIR)binpacker: $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $(BIN_DIR)binpacker

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) -c -I$(INC_DIR) $< -o $@

clean:
	rm -rv $(OBJ_DIR) $(BIN_DIR)