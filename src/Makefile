runner: main.cpp parser.h parser.cpp pattern.h pattern.cpp lexer.h lexer.cpp global.h global.cpp
	g++ -std=c++11 -O2 parser.cpp regex.cpp tokenizer.cpp pattern.cpp lexer.cpp global.cpp main.cpp -o AQL

global_test: global_test.cpp global.h global.cpp
	g++ -std=c++11 -O2 global.cpp global_test.cpp -o unit_test/global_test

tokenizer_test: tokenizer_test.cpp tokenizer.h tokenizer.cpp global.h global.cpp
	g++ -std=c++11 -O2 tokenizer.cpp global.cpp tokenizer_test.cpp -o unit_test/tokenizer_test

lexer_test: lexer_test.cpp lexer.h lexer.cpp global.h global.cpp
	g++ -std=c++11 -O2 lexer.cpp global.cpp lexer_test.cpp -o unit_test/lexer_test

parser_test: parser_test.cpp parser.h parser.cpp pattern.h pattern.cpp lexer.h lexer.cpp global.h global.cpp
	g++ -std=c++11 -O2 parser.cpp regex.cpp tokenizer.cpp pattern.cpp lexer.cpp global.cpp parser_test.cpp -o unit_test/parser_test