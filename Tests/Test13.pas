// Test 13: Procedure with a Value Parameter
// Tests passing a value to a procedure parameter.

PROGRAM Test13;
PROCEDURE PrintValue(val: INTEGER);
BEGIN
  writeln(val);
END;

BEGIN
  PrintValue(42);
END.

// Expected Output: 42