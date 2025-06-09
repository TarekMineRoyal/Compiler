// Test 8: Parentheses in Expressions
// Ensures parentheses override default operator precedence.

PROGRAM Test8;
VAR
  result: INTEGER;
BEGIN
  result := (2 + 3) * 4; // Should be 5 * 4 = 20
  writeln(result);
END.

// Expected Output: 20