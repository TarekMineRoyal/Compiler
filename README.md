# MiniPascal Compiler

A compiler for MiniPascal, a custom simplified Pascal-like programming language. Implemented from scratch in C++ (std=c++17) using Flex for lexical analysis and Bison for syntax analysis. This project implements all the core phases of compiler design, from tokenizing to generating executable assembly for a custom stack-based virtual machine.

## Current Status (as of June 8, 2025)

The compiler is functionally complete for single-block programs. All major phases have been implemented and tested:

* **Lexical & Syntax Analysis:** The frontend correctly tokenizes source code and builds a valid Abstract Syntax Tree (AST).
* **Semantic Analysis:** The semantic analyzer correctly validates the AST for scope resolution, type checking (including `INTEGER`, `REAL`, `BOOLEAN`, and `ARRAY` types), and declaration verification.
* **Code Generation:** The backend successfully traverses the semantically-validated AST and generates correct, executable assembly code for the target stack-based Virtual Machine. Key tested features include:
    * Variable declaration and assignment.
    * Full arithmetic, relational, and logical expression evaluation.
    * Correct type promotion from `INTEGER` to `REAL`.
    * `IF...THEN...ELSE` and `WHILE...DO` control flow structures.
    * Array allocation and access with both literal and expression-based indices.
    * Formatted I/O with `write` and `writeln`.

## Key Features of MiniPascal Supported

The compiler currently supports the following MiniPascal language features:

* **Program Structure:** `PROGRAM ... BEGIN ... END.`
* **Declarations:**
    * `VAR`: For global variables.
    * Data Types: `INTEGER`, `REAL`, `BOOLEAN`.
    * `ARRAY [low..high] OF standard_type`: One-dimensional arrays with support for variable indices.
* **Subprograms:**
    * Parsing support for `PROCEDURE` and `FUNCTION` is in place. Code generation is the next major goal.
* **Statements:**
    * Assignment: `variable := expression;`
    * Compound: `BEGIN ... END;`
    * Conditional: `IF condition THEN statement ELSE statement;`
    * Looping: `WHILE condition DO statement;`
* **Expressions:**
    * Arithmetic: `+`, `-`, `*`, `/` (real division), `div` (integer division).
    * Relational: `=`, `<>`, `<`, `<=`, `>`, `>=`.
    * Logical: `AND`, `OR`, `NOT`.
    * Parentheses for grouping with correct operator precedence.
* **Literals:** Integer, Real, Boolean (`TRUE`, `FALSE`), String (e.g., `'Hello'`).
* **Built-in I/O:** `write`, `writeln`.

## Target Virtual Machine

The compiler targets a custom stack-based virtual machine.

* **Architecture:** It uses separate stacks for execution and calls, with `gp` (global pointer) and `fp` (frame pointer) registers to manage variable scopes.
* **Instruction Set:** The generator produces text-based assembly code (e.g., `PUSHI`, `STOREG`, `ALLOC`, `JUMP`, `CALL`) that the VM executes directly.

## Technologies Used

* **C++:** (Standard: C++17) for the AST, semantic analysis, code generator, and main compiler logic.
* **GNU Flex:** For lexical analysis (scanner generation).
* **GNU Bison:** For syntax analysis (parser generation).
* **GNU Make:** For building the project.
* **Development Environment:** MinGW64 on Windows.

## How to Build

1.  Ensure you have GNU Flex, GNU Bison, and a C++ compiler (like g++) installed and in your system's PATH.
2.  Navigate to the root directory of the project in your terminal.
3.  Run the `make` command:
    ```bash
    make
    ```
    This will produce the executable `my_compiler`.

## How to Run

1.  Compile a MiniPascal source file:
    ```bash
    ./my_compiler your_program.pas
    ```
    This will generate an `output.vm` file containing the assembly code.

2.  Execute the generated code with the provided virtual machine:
    ```bash
    ./vm.exe output.vm
    ```

## Next Steps / Future Goals

With the core functionality for single-block programs complete and stable, the immediate next goals are to implement full support for subprograms:

1.  **Procedures:** Implement the `CALL`/`RETURN` sequence for simple procedures without parameters.
2.  **Parameters:** Add support for passing parameters to procedures and functions.
3.  **Local Variables:** Enable support for variables declared within subprograms.
4.  **Functions & Return Values:** Implement the logic for functions to return values to the caller.
5.  **Input:** Implement the built-in `read` and `readln` procedures.

---
Project Lead: Tarek
