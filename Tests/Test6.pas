// Test 6: Type Promotion in Expressions
// Ensures an INTEGER added to a REAL promotes the result to REAL.

PROGRAM Test6;
VAR
  my_int: INTEGER;
  my_real: REAL;
  result: REAL;
BEGIN
  my_int := 5;
  my_real := 2.5;
  result := my_int + my_real;
  writeln(result);
END.

// Expected Output: 7.5