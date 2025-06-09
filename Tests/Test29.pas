// Test 29: Unary Minus Precedence
// Tests that unary minus is handled correctly in an expression.

PROGRAM Test29;
VAR
  x, y : INTEGER;
BEGIN
  x := 10;
  y := -x; // y should be -10
  writeln(y);
  writeln(x - -3); // 10 - (-3) = 13
END.

// Expected Output:
// -10
// 13