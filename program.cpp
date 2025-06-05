// program.cpp
#include "ast.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include <iostream>
#include <fstream>

extern ProgramNode* root_ast_node;
extern int yyparse();
extern FILE* yyin; // For Flex input file

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
    }
    else {
        // Optionally, allow stdin if no file is provided
        std::cout << "No input file specified. Reading from stdin (Ctrl+D or Ctrl+Z to end)." << std::endl;
    }

    yydebug = 0; // Change it to ture for Bison debug output

    std::cout << "Starting parsing..." << std::endl;
    int parse_result = yyparse();

    if (argc > 1 && yyin) {
        fclose(yyin); // Close the file if one was opened
    }

    if (parse_result == 0 && root_ast_node != nullptr) {
        std::cout << "Parsing successful!" << std::endl;

        // --- Semantic Analysis ---
        std::cout << "Starting semantic analysis..." << std::endl;
        SemanticAnalyzer semanticAnalyzer;
        try {
            root_ast_node->accept(semanticAnalyzer); // Start semantic analysis

            if (semanticAnalyzer.hasErrors()) {
                std::cerr << "Semantic analysis failed with the following errors:" << std::endl;
                semanticAnalyzer.printErrors(std::cerr);
                // Decide if you want to print AST or stop
            }
            else {
                std::cout << "Semantic analysis successful!" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Semantic analysis crashed: " << e.what() << std::endl;
        }
        // --- End Semantic Analysis ---


        // Print AST only if no semantic errors or for debugging
        // if (!semanticAnalyzer.hasErrors()) { // Or always print for debugging
        std::cout << "\n--- AST Dump ---" << std::endl;
        root_ast_node->print(std::cout, 0);
        std::cout << "--- End AST Dump ---" << std::endl;
        // }

        delete root_ast_node; // Cleanup AST
        root_ast_node = nullptr;

    }
    else {
        std::cout << "Parsing failed." << std::endl;
        if (root_ast_node) { // Should be null on parse error, but good practice
            delete root_ast_node;
            root_ast_node = nullptr;
        }
    }
    int x;
    std::cin >> x;
    return 0;
}