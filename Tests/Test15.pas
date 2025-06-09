// Test 15: Function with Parameters
// Tests a function that uses parameters in its calculation.

PROGRAM Test15;
VAR
  result: INTEGER;
FUNCTION Subtract(a: INTEGER; b: INTEGER): INTEGER;
BEGIN
  RETURN a - b;
END;

BEGIN
  result := Subtract(10, 3);
  writeln(result);
END.

// Expected Output: 7