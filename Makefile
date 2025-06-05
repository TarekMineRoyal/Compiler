all:
	bison -dtv -o parser.cpp --defines=parser.h parser.y  
	flex -oscanner.cpp ./lexer.l   
	g++ -std=c++17 -o my_compiler ./program.cpp ./parser.cpp ./scanner.cpp ./ast.cpp ./symbol_table.cpp ./semantic_analyzer.cpp -I"D:/Program Files/msys64/usr/include" -static-libgcc -static-libstdc++ -static