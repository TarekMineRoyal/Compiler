# MiniPascal Compiler

A compiler for MiniPascal, a custom simplified Pascal-like programming language. Implemented from scratch in C++ (std=c++17) using Flex for lexical analysis and Bison for syntax analysis. This project aims to explore and implement the core phases of compiler design.

## Current Status (as of June 5, 2025)

The compiler frontend is functionally complete and has been successfully tested. This includes:
* **Lexical Analysis:** Tokenization of MiniPascal source code.
* **Syntax Analysis:** Parsing of tokens and construction of an Abstract Syntax Tree (AST).
* **Semantic Analysis:** Scope resolution, type checking, declaration verification, and semantic validation of built-in I/O procedures. The AST is annotated with determined type information.

## Key Features of MiniPascal Supported

The compiler currently supports the following MiniPascal language features:

* **Program Structure:** `PROGRAM ... BEGIN ... END.`
* **Declarations:**
    * `VAR`: For global and local variables.
    * Data Types: `INTEGER`, `REAL`, `BOOLEAN`.
    * `ARRAY [low..high] OF standard_type`: One-dimensional arrays of standard types with literal integer bounds.
* **Subprograms:**
    * `PROCEDURE Name(params); local_vars; BEGIN ... END;`
    * `FUNCTION Name(params) : return_type; local_vars; BEGIN ... RETURN value; END;`
    * Parameters are passed by value (conceptually).
* **Statements:**
    * Assignment: `variable := expression;`
    * Compound: `BEGIN ... END;`
    * Procedure Calls: `ProcName;` or `ProcName(arg1, arg2);`
    * Conditional: `IF condition THEN statement ELSE statement;` (and `IF...THEN;`)
    * Looping: `WHILE condition DO statement;`
    * Function Return: `RETURN expression;`
* **Expressions:**
    * Arithmetic: `+`, `-`, `*`, `/` (real division), `DIV` (integer division).
    * Relational: `=`, `<>`, `<`, `<=`, `>`, `>=`.
    * Logical: `AND`, `OR`, `NOT`.
    * Function calls within expressions.
    * Parentheses for grouping.
    * Correct operator precedence and associativity.
* **Literals:** Integer, Real, Boolean (`TRUE`, `FALSE`), String (e.g., `'Hello'`).
* **Comments:** Single-line (`// ...`) and multi-line (`{ ... }`).
* **Built-in I/O (Semantically Checked):** `read`, `readln`, `write`, `writeln`.

## Technologies Used

* **C++:** (Standard: C++17) for AST implementation, semantic analysis, and main compiler logic.
* **GNU Flex:** For lexical analysis (scanner generation).
* **GNU Bison:** For syntax analysis (parser generation).
* **GNU Make:** For building the project.
* **Development Environment:** MinGW64 on Windows (as per project setup).

## How to Build

1.  Ensure you have GNU Flex, GNU Bison, and a C++ compiler (like g++) installed and in your system's PATH.
2.  Navigate to the root directory of the project in your terminal.
3.  Run the `make` command:
    ```bash
    make
    ```
    This will generate `parser.cpp`, `parser.h`, `scanner.cpp`, and then compile all necessary C++ files to produce the executable `my_compiler` (or the name specified in your Makefile).

## How to Run

After successfully building the compiler, you can run it with a MiniPascal source file.

* **Provide a file as an argument:**
    ```bash
    ./my_compiler your_program.pas
    ```
* **Pipe input to the compiler (if it reads from stdin by default):**
    ```bash
    cat your_program.pas | ./my_compiler
    ```
    The compiler will output parsing status, semantic analysis status (including any errors), and an AST dump if errors are not prohibitive.

## Next Steps / Future Goals

1.  **Intermediate Code Generation (ICG):** Translate the semantically validated AST into a machine-independent intermediate representation (e.g., Three-Address Code).
2.  **Optimization:** Apply optimizations to the intermediate code.
3.  **Target Code Generation:** Translate the intermediate code into executable machine code or assembly for a specific target architecture.

---
Project Lead: Tarek
