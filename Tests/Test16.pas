// Test 16: Function with a Local Variable
// Tests that a function can declare and use its own local variables.

PROGRAM Test16;
VAR
  result: INTEGER;
FUNCTION Calc: INTEGER;
  VAR
    local_val: INTEGER;
  BEGIN
    local_val := 50;
    RETURN local_val * 2;
  END;

BEGIN
  result := Calc;
  writeln(result);
END.

// Expected Output: 100