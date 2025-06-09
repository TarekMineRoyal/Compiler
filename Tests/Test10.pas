// Test 10: IF-THEN-ELSE Statement
// Tests that both branches of an IF-THEN-ELSE statement work correctly.

PROGRAM Test10;
VAR
  x: INTEGER;
  cond: BOOLEAN;
BEGIN
  cond := FALSE;
  x := 0;
  IF cond THEN
    x := 1
  ELSE
    x := 2;
  writeln(x);
END.

// Expected Output: 2