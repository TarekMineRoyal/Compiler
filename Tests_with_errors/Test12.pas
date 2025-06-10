PROGRAM MultiSemantic;
FUNCTION Problematic(a: INTEGER): REAL;
  VAR
    b: BOOLEAN;
  BEGIN
    b := a + 5.5; // Error: Cannot assign REAL to BOOLEAN
    IF b THEN
      b := z; // Error: 'z' is not declared
    RETURN a;
  END;

BEGIN
END.

{
Error Test 12: Multiple Semantic Errors in One Function
Tests the analyzer's ability to find multiple semantic errors within one subprogram.
Expected Error(s):
Semantic Error (L:6, C:18): Type mismatch in assignment to 'b'. Cannot assign type Real to variable of type Boolean.
Semantic Error (L:8, C:12): Identifier 'z' is not declared.
}