// Test 9: Simple IF-THEN Statement
// Tests a simple IF-THEN statement with a true condition.

PROGRAM Test9;
VAR
  x: INTEGER;
BEGIN
  x := 10;
  IF TRUE THEN
    x := 99;
  writeln(x);
END.

// Expected Output: 99