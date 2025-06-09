// Test 17: Global Variable Access From Function
// Ensures a function can read a global variable.

PROGRAM Test17;
VAR
  global_factor: INTEGER;
  result: INTEGER;

FUNCTION MultiplyByGlobal(val: INTEGER): INTEGER;
BEGIN
  RETURN val * global_factor;
END;

BEGIN
  global_factor := 3;
  result := MultiplyByGlobal(10);
  writeln(result);
END.

// Expected Output: 30