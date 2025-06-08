// program.cpp
#include "ast.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "codegenerator.h"
#include <iostream>
#include <fstream>
#include <string>

extern ProgramNode* root_ast_node;
extern int yyparse();
extern FILE* yyin;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "Usage: ./my_compiler <input_file.pas>" << std::endl;
        return 1;
    }

    yydebug = 0; // Set to 1 for parser debug info

    std::cout << "Starting parsing..." << std::endl;
    int parse_result = yyparse();

    if (argc > 1 && yyin) {
        fclose(yyin);
    }

    if (parse_result == 0 && root_ast_node != nullptr) {
        std::cout << "Parsing successful!" << std::endl;

        // --- Semantic Analysis ---
        SemanticAnalyzer semanticAnalyzer;
        root_ast_node->accept(semanticAnalyzer);

        if (semanticAnalyzer.hasErrors()) {
            std::cerr << "\nSemantic analysis failed with errors:" << std::endl;
            semanticAnalyzer.printErrors(std::cerr);
            delete root_ast_node;
            return 1;
        }
        std::cout << "Semantic analysis successful!" << std::endl;

        // --- Code Generation ---
        std::cout << "Starting code generation..." << std::endl;
        CodeGenerator codeGenerator;
        try {
            // Generate the assembly code as a string
            std::string assemblyCode = codeGenerator.generateCode(*root_ast_node, semanticAnalyzer);

            // Write the generated code to an output file
            std::string outputFilename = "output.vm";
            std::ofstream outfile(outputFilename);
            if (!outfile) {
                std::cerr << "Error: Could not open output file " << outputFilename << std::endl;
            }
            else {
                outfile << assemblyCode;
                std::cout << "Code generation successful! Assembly written to " << outputFilename << std::endl;
                std::cout << "Run with: vm.exe " << outputFilename << std::endl;
            }

        }
        catch (const std::runtime_error& e) {
            std::cerr << "Code generation crashed: " << e.what() << std::endl;
            delete root_ast_node;
            return 1;
        }

        delete root_ast_node;

    }
    else {
        std::cout << "Parsing failed." << std::endl;
        if (root_ast_node) delete root_ast_node;
        return 1;
    }

    return 0;
}