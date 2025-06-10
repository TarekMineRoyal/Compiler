PROGRAM SemanticError1;
BEGIN
  x := 10; // 'x' is not declared
END.

// Error Test 6: Semantic - Undeclared Variable
// Tests if the semantic analyzer reports the use of a variable that was never declared.
// Expected Error(s): Semantic Error (L:3, C:3): Identifier 'x' is not declared.