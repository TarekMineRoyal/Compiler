// Test 25: Recursive Function (Factorial)
// Tests if a function can correctly call itself and manage the stack.

PROGRAM Test25;
VAR
  result: INTEGER;

FUNCTION Factorial(n: INTEGER): INTEGER;
BEGIN
  IF n > 1 THEN
    RETURN n * Factorial(n-1)
  ELSE
    RETURN 1;
END;

BEGIN
  result := Factorial(5); // 5 * 4 * 3 * 2 * 1 = 120
  writeln(result);
END.

// Expected Output: 120