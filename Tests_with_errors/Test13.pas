PROGRAM ScopeErrors;
VAR
  x: INTEGER;
  x: REAL; // Error: Redeclaration of 'x' in global scope

PROCEDURE Test;
  VAR
    y: INTEGER;
    y: INTEGER; // Error: Redeclaration of 'y' in local scope
  BEGIN
  END;

BEGIN
END.

{
Error Test 13: Redeclaration and Scope Errors
Tests errors for redeclaring variables in the same scope.
Expected Error(s):
Semantic Error (L:4, C:3): Variable 'x' re-declared in the current scope. Conflicts with existing Variable declared at L:3
Semantic Error (L:9, C:5): Variable 'y' re-declared in the current scope. Conflicts with existing Variable declared at L:8
}