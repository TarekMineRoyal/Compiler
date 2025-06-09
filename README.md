# MiniPascal Compiler

A compiler for MiniPascal, a custom, simplified Pascal-like programming language. Implemented from scratch in C++ (std=c++17) using Flex for lexical analysis and Bison for syntax analysis. This project implements all the core phases of compiler design, from tokenizing to generating executable assembly for a custom stack-based virtual machine.

### Current Status (as of June 9, 2025)

The compiler is now **functionally complete**. All major phases have been implemented, allowing the compiler to process MiniPascal source code with complex features, including nested subprograms, local and global scopes, and arrays, and generate executable assembly.

* **Lexical & Syntax Analysis:** The frontend correctly tokenizes source code and builds a valid Abstract Syntax Tree (AST).
* **Semantic Analysis:** The semantic analyzer correctly validates the AST for scope resolution, type checking (including `INTEGER`, `REAL`, `BOOLEAN`, and `ARRAY` types), and declaration verification.
* **Code Generation:** The backend successfully traverses the semantically-validated AST and generates correct, executable assembly code for the target stack-based Virtual Machine.

**All major language features have been implemented and tested, including full subprogram support (procedures and functions) and built-in I/O.**

**Known Issues:**
* There is a known bug causing a "VM error: Illegal Operand" when accessing local arrays declared inside a procedure or function. All other features, including global arrays and local simple variables, are working correctly.

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

### How to Build

1.  Ensure you have GNU Flex, GNU Bison, and a C++ compiler (like g++) installed and in your system's PATH.
2.  Navigate to the root directory of the project in your terminal.
3.  Run the `make` command:
    ```bash
    make
    ```
    This will produce the executable `my_compiler`.

### How to Run

1.  **Compile a MiniPascal source file:**
    ```bash
    ./my_compiler your_program.pas
    ```
    This will generate an `output.vm` file containing the assembly code.
    There is a folder with 32 tests to try.

3.  **Execute the generated code** with the provided virtual machine:
    ```bash
    ./vm.exe output.vm
    ```

### Next Steps / Future Goals

With the core language features now fully implemented, the project goals have shifted to stabilization and fixing bugs.

* **Future Goal:**
    * Fix the outstanding bug related to the allocation and use of **local arrays** inside subprograms.

---
Project Lead: Tarek
