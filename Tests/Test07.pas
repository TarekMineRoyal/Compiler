// Test 7: Operator Precedence
// Ensures multiplication is performed before addition.

PROGRAM Test7;
VAR
  result: INTEGER;
BEGIN
  result := 2 + 3 * 4; // Should be 2 + 12 = 14
  writeln(result);
END.

// Expected Output: 14