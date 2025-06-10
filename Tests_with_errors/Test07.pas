PROGRAM SemanticError2;
VAR
  my_int: INTEGER;
BEGIN
  my_int := 10.5;
END.

// Error Test 7: Semantic - Type Mismatch in Assignment
// Tests for a type error when assigning a REAL to an INTEGER variable.
// Expected Error(s): Semantic Error (L:5, C:18): Type mismatch in assignment to 'my_int'. Cannot assign type Real to variable of type Integer.