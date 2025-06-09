// Test 32: Type Promotion in Function Return
// Tests that an INTEGER parameter can be used in a REAL calculation and returned as a REAL.

PROGRAM Test32;
VAR
  result: REAL;

FUNCTION AddReal(val: INTEGER): REAL;
BEGIN
  RETURN val + 0.5;
END;

BEGIN
  result := AddReal(10);
  writeln(result);
END.

// Expected Output: 10.5