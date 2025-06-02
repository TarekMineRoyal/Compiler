#ifndef COMMON_H
#define COMMON_H

// This enum will be used by the scanner to return token types
// and by the parser to identify them.
// The parser (Bison) will generate its own version of these,
// often in a file like `parser.hpp` or `parser.tab.hpp`.
// We list them here for clarity and for the scanner to use.
// Bison will expect these exact names for terminal symbols.

enum TokenType {
    // End of File
    TOK_EOF = 0, // Often 0 is used for EOF

    // Keywords
    TOK_PROGRAM,
    TOK_VAR,
    TOK_ARRAY,
    TOK_OF,
    TOK_INTEGER, // Type keyword
    TOK_REAL,    // Type keyword
    TOK_FUNCTION,
    TOK_PROCEDURE,
    TOK_BEGIN,
    TOK_END,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_WHILE,
    TOK_DO,
    // TOK_FOR, TOK_TO, TOK_DOWNTO, // Add if you plan to support for loops

    // Operators
    TOK_ASSIGN,     // :=
    TOK_PLUS,       // +
    TOK_MINUS,      // -
    TOK_MULTIPLY,   // *
    TOK_DIVIDE,     // / (real division)
    TOK_DIV,        // div (integer division)
    TOK_MOD,        // mod

    // Relational Operators
    TOK_EQ,         // =
    TOK_NEQ,        // <>
    TOK_LT,         // <
    TOK_LTE,        // <=
    TOK_GT,         // >
    TOK_GTE,        // >=

    // Logical Operators (Optional for now, can be added later)
    // TOK_AND,
    // TOK_OR,
    // TOK_NOT,

    // Punctuation
    TOK_LPAREN,     // (
    TOK_RPAREN,     // )
    TOK_LBRACKET,   // [
    TOK_RBRACKET,   // ]
    TOK_COMMA,      // ,
    TOK_SEMICOLON,  // ;
    TOK_COLON,      // :
    TOK_DOT,        // .
    TOK_DOTDOT,     // .. (for array ranges)

    // Literals and Identifiers
    TOK_IDENTIFIER,
    TOK_INTEGER_LITERAL,
    TOK_REAL_LITERAL,
    // TOK_STRING_LITERAL, // If you add string literals

    // Error token (optional, for error handling)
    TOK_ERROR
};

#endif // COMMON_H