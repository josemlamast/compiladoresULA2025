CXX = clang++ -std=c++17 -O0 -g
FLEX = flex
BISON = bison --defines=token.h

OBJ = ast_node_interface.o datatype.o declaration.o expression.o statement.o symbol_table.o parser.o scanner.o

default: main

all: main

main: $(OBJ) main.cpp
	$(CXX) -I. $@.cpp -o $@ $(OBJ)
	
parser.o: parser.c
	$(CXX) -c -I. -std=c++17 parser.c

parser.c: parser.bison
	$(BISON) -v --output parser.c parser.bison

scanner.o: token.h scanner.c
	$(CXX) -c -std=c++17 scanner.c

scanner.c: scanner.flex
	$(FLEX) -o scanner.c scanner.flex

main.o: token.h main.c
	$(CXX) -c -I. -std=c++17 main.c

ast_node_interface.o: ast_node_interface.cpp ast_node_interface.hpp 
	$(CXX) -I. -c $< -o $@

datatype.o: datatype.cpp datatype.hpp 
	$(CXX) -I. -c $< -o $@

declaration.o: declaration.cpp declaration.hpp 
	$(CXX) -I. -c $< -o $@

expression.o: expression.cpp expression.hpp 
	$(CXX) -I. -c $< -o $@

statement.o: statement.cpp statement.hpp 
	$(CXX) -I. -c $< -o $@

symbol_table.o: symbol_table.cpp symbol_table.hpp 
	$(CXX) -I. -c $< -o $@

.PHONY:
clean:
	$(RM) $(OBJ) main
