PROGRAM SemanticError5;
PROCEDURE BadProc;
BEGIN
  RETURN 123; // Procedures cannot return a value
END;

BEGIN
  BadProc;
END.

// Error Test 10: Semantic - RETURN in a Procedure
// Tests if a RETURN statement with a value is correctly flagged as an error inside a procedure.
// Expected Error(s): Semantic Error (L:4, C:3): RETURN statement found outside of a function or in a function with no name context.