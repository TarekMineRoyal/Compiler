// Test 34: Procedure and Function with Same Name
// Purpose: This test verifies that the compiler can correctly handle a PROCEDURE and a FUNCTION that share
// the same name, because their mangled names ('p_DoWork' and 'f_DoWork') are unique.

PROGRAM TestSameName;
VAR
  result: INTEGER;
// This will be stored in the symbol table with the key "p_DoWork".
PROCEDURE DoWork;
BEGIN
  writeln('Procedure doing work.');
END;

// This is no longer an error. It will be stored with the key "f_DoWork".
FUNCTION DoWork: INTEGER;
BEGIN
  RETURN 100;
END;

BEGIN
  // Call the procedure. The semantic analyzer will look for 'p_DoWork'.
  DoWork; 
  
  // Call the function. The semantic analyzer will look for 'f_DoWork'.
  result := DoWork;
  writeln('Function returned:');
  writeln(result);
END.

{
Procedure doing work.
Function returned:
100
}