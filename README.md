# MiniPascal Compiler

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Flex](https://img.shields.io/badge/Tool-Flex-orange.svg)
![Bison](https://img.shields.io/badge/Tool-Bison-purple.svg)
![Build](https://img.shields.io/badge/Build-Make-green.svg)
![Compiler](https://img.shields.io/badge/Compiler-MinGW64-red.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

A compiler for MiniPascal, a custom, simplified Pascal-like programming language. Implemented from scratch in C++ (std=c++17) using Flex for lexical analysis and Bison for syntax analysis. This project implements all the core phases of compiler design, from tokenizing to generating executable assembly for a custom stack-based virtual machine.

### Current Status (as of June 18, 2025)

The compiler is now **functionally complete**. All major phases have been implemented, allowing the compiler to process MiniPascal source code with complex features—including nested subprograms, local/global scopes, arrays, and **function/procedure overloading**—and generate executable assembly.

* **Lexical & Syntax Analysis:** The frontend correctly tokenizes source code and builds a valid Abstract Syntax Tree (AST).
* **Semantic Analysis:** The semantic analyzer correctly validates the AST for scope resolution, type checking (including `INTEGER`, `REAL`, `BOOLEAN`, and `ARRAY` types), and declaration verification. It now fully supports **overload resolution** for subprograms.
* **Code Generation:** The backend successfully traverses the semantically-validated AST and generates correct, executable assembly code for the target stack-based Virtual Machine.

**All major language features have been implemented and tested, including full subprogram support (procedures and functions with overloading) and built-in I/O.**

### Key Features of MiniPascal Supported

The compiler currently supports the following MiniPascal language features:

* **Program Structure:** `PROGRAM ... BEGIN ... END.`
* **Declarations:**
    * `VAR`: For both global and local variables.
    * Data Types: `INTEGER`, `REAL`, `BOOLEAN`.
    * `ARRAY [low..high] OF standard_type`: One-dimensional arrays with support for variable indices.
* **Subprograms:**
    * `PROCEDURE Name(params); local_vars; BEGIN ... END;`
    * `FUNCTION Name(params) : return_type; local_vars; BEGIN ... RETURN value; END;`
    * **Function & Procedure Overloading:** Subprograms with the same name are differentiated by their parameter signatures.
    * Correct handling of parameters, local variables, and global variables.
    * Support for recursive function calls.
* **Statements:**
    * Assignment: `variable := expression;`
    * Compound: `BEGIN ... END;`
    * Procedure Calls: `ProcName;` or `ProcName(arg1, arg2);`
    * Conditional: `IF condition THEN statement ELSE statement;`
    * Looping: `WHILE condition DO statement;`
    * Function Return: `RETURN expression;`
* **Expressions:**
    * Arithmetic: `+`, `-`, `*`, `/` (real division), `div` (integer division).
    * Relational: `=`, `<>`, `<`, `<=`, `>`, `>=`.
    * Logical: `AND`, `OR`, `NOT`.
    * Parentheses for grouping with correct operator precedence.
* **Literals:** Integer, Real, Boolean (`TRUE`, `FALSE`), String (e.g., `'Hello'`).
* **Built-in I/O:** `write`, `writeln`, `read`, `readln`.

### Target Virtual Machine

The compiler targets a custom stack-based virtual machine.

* **Architecture:** It uses separate stacks for execution and calls, with `gp` (global pointer) and `fp` (frame pointer) registers to manage variable scopes.
* **Instruction Set:** The generator produces text-based assembly code (e.g., `pushi`, `storeg`, `alloc`, `jump`, `call`) that the VM executes directly.

### Technologies Used

* **C++:** (Standard: C++17) for the AST, semantic analysis, code generator, and main compiler logic.
* **GNU Flex:** For lexical analysis (scanner generation).
* **GNU Bison:** For syntax analysis (parser generation).
* **GNU Make:** For building the project.
* **Development Environment:** MinGW64 on Windows.

---
Project Lead: Tarek
