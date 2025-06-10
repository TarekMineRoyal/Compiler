#include "ast.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "codegenerator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem> // For creating directories (C++17)

// External variables from Flex/Bison
extern ProgramNode* root_ast_node;
extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int lin;
extern int col;
extern YYSTYPE yylval;

// Global error flag, set by lexer or parser on error
bool compilation_has_error = false;

// Helper function to get token names (must be declared)
const char* token_to_string(int token, const YYSTYPE& lval);

// Helper function to get the base name of a file path
std::string get_base_filename(const std::string& path) {
    size_t last_slash = path.find_last_of("/\\");
    std::string filename = (last_slash == std::string::npos) ? path : path.substr(last_slash + 1);
    size_t last_dot = filename.find_last_of('.');
    return (last_dot == std::string::npos) ? filename : filename.substr(0, last_dot);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./my_compiler <input_file.pas>" << std::endl;
        return 1;
    }

    std::string input_filename(argv[1]);
    std::string base_name = get_base_filename(input_filename);
    std::string output_dir = "output";

    // --- Create output directory ---
    try {
        if (!std::filesystem::exists(output_dir)) {
            std::filesystem::create_directory(output_dir);
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error creating output directory: " << e.what() << std::endl;
        return 1;
    }

    // --- Open input file ---
    yyin = fopen(input_filename.c_str(), "r");
    if (!yyin) {
        std::cerr << "Error: Could not open file " << input_filename << std::endl;
        return 1;
    }

    // =============================================
    // PHASE 1: LEXICAL ANALYSIS
    // =============================================
    std::cout << "Phase 1: Lexical Analysis..." << std::endl;
    std::string tokens_filepath = output_dir + "/" + base_name + ".tokens.txt";
    std::ofstream tokens_file(tokens_filepath);
    if (!tokens_file) {
        std::cerr << "Error: Could not open tokens output file." << std::endl;
        fclose(yyin);
        return 1;
    }

    int token;
    tokens_file << "--- Token Stream ---" << std::endl;
    while ((token = yylex()) != 0) {
        tokens_file << "Token: " << token_to_string(token, yylval)
            << " (ID: " << token << ")"
            << " at (L:" << lin << ", C:" << col << ")";
        if (token == IDENT) tokens_file << " - Value: " << yylval.rawIdent->name;
        if (token == NUM) tokens_file << " - Value: " << yylval.rawNum->value;
        if (token == REAL_LITERAL) tokens_file << " - Value: " << yylval.rawRealLit->value;
        if (token == STRING_LITERAL) tokens_file << " - Value: \"" << yylval.str_val << "\"";
        tokens_file << std::endl;
    }
    tokens_file.close();

    if (compilation_has_error) {
        std::cerr << "Lexical analysis failed." << std::endl;
        fclose(yyin);
        return 1;
    }
    std::cout << "Lexical analysis successful. Token list written to " << tokens_filepath << std::endl;

    // Reset file and lexer for parsing phase
    rewind(yyin);
    lin = 1;
    col = 1;

    // =============================================
    // PHASE 2: SYNTAX ANALYSIS (PARSING)
    // =============================================
    std::cout << "\nPhase 2: Syntax Analysis..." << std::endl;
    int parse_result = yyparse();

    if (parse_result != 0 || compilation_has_error || root_ast_node == nullptr) {
        std::cerr << "Syntax analysis failed." << std::endl;
        fclose(yyin);
        return 1;
    }
    std::cout << "Parsing successful!" << std::endl;

    std::string ast_filepath = output_dir + "/" + base_name + ".ast.txt";
    std::ofstream ast_file(ast_filepath);
    root_ast_node->print(ast_file);
    ast_file.close();
    std::cout << "AST dump written to " << ast_filepath << std::endl;


    // =============================================
    // PHASE 3: SEMANTIC ANALYSIS
    // =============================================
    std::cout << "\nPhase 3: Semantic Analysis..." << std::endl;
    SemanticAnalyzer semanticAnalyzer;
    root_ast_node->accept(semanticAnalyzer);

    std::string semantics_filepath = output_dir + "/" + base_name + ".semantic_analysis.log";
    std::ofstream semantics_file(semantics_filepath);

    if (semanticAnalyzer.hasErrors()) {
        std::cerr << "Semantic analysis failed. See log for details." << std::endl;
        semantics_file << "--- SEMANTIC ERRORS ---" << std::endl;
        semanticAnalyzer.printErrors(semantics_file);
        delete root_ast_node;
        fclose(yyin);
        semantics_file.close();
        return 1;
    }
    std::cout << "Semantic analysis successful!" << std::endl;
    semantics_file << "Semantic analysis successful. No errors found." << std::endl;
    semantics_file.close();


    // =============================================
    // PHASE 4: CODE GENERATION
    // =============================================
    std::cout << "\nPhase 4: Code Generation..." << std::endl;
    CodeGenerator codeGenerator;
    std::string assemblyCode;
    try {
        assemblyCode = codeGenerator.generateCode(*root_ast_node, semanticAnalyzer);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Code generation crashed: " << e.what() << std::endl;
        delete root_ast_node;
        fclose(yyin);
        return 1;
    }

    std::string vm_filepath = output_dir + "/" + base_name + ".assembly.vm";
    std::ofstream vm_file(vm_filepath);
    vm_file << assemblyCode;
    vm_file.close();
    std::cout << "Code generation successful! Assembly written to " << vm_filepath << std::endl;
    std::cout << "Run with: ./vm.exe " << vm_filepath << std::endl;

    // --- Cleanup ---
    delete root_ast_node;
    fclose(yyin);

    return 0;
}
