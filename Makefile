CXX = clang++ -std=c++17 -O0 -g
FLEX = flex
BISON = bison --defines=token.h

OBJ = utils.o expression.o Exceptions.o semantic_analyzer.o parser.o scanner.o main.o

default: main

all: main

main: $(OBJ)
	$(CXX) -I. -o $@ $(OBJ)
	
parser.o: parser.c
	$(CXX) -c -I. -std=c++17 parser.c

parser.c: parser.bison
	$(BISON) -v --output parser.c parser.bison

token.h: parser.bison
	$(BISON) --defines=token.h parser.bison

scanner.o: token.h scanner.c
	$(CXX) -c -std=c++17 scanner.c

scanner.c: scanner.flex
	$(FLEX) -o scanner.c scanner.flex

main.o: token.h main.cpp
	$(CXX) -c -I. -std=c++17 main.cpp


utils.o: utils.cpp utils.hpp 
	$(CXX) -I. -c $< -o $@

Exceptions.o: Exceptions.cpp Exceptions.hpp 
	$(CXX) -I. -c $< -o $@

expression.o: expression.cpp expression.hpp 
	$(CXX) -I. -c $< -o $@

semantic_analyzer.o: semantic_analyzer.cpp semantic_analyzer.hpp 
	$(CXX) -I. -c $< -o $@


.PHONY:
clean:
	$(RM) $(OBJ) main
