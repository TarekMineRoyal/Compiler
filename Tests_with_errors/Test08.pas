PROGRAM SemanticError3;
PROCEDURE MyProc(a: INTEGER; b: INTEGER);
BEGIN
END;

BEGIN
  MyProc(1); // Expects 2 arguments, given 1
END.

// Error Test 8: Semantic - Wrong Number of Arguments
// Tests for an error when calling a procedure with too few arguments.
// Expected Error(s): Semantic Error (L:7, C:3): Procedure 'MyProc' called with 1 arguments, but definition expects 2.