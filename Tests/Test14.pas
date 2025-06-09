// Test 14: Simple Function Call
// Tests defining and calling a function that returns a literal.

PROGRAM Test14;
VAR
  result: INTEGER;
FUNCTION GetNumber: INTEGER;
BEGIN
  RETURN 100;
END;

BEGIN
  result := GetNumber;
  writeln(result);
END.

// Expected Output: 100