// Test 11: WHILE Loop
// Tests a simple WHILE loop for correct iteration and termination.

PROGRAM Test11;
VAR
  i: INTEGER;
BEGIN
  i := 0;
  WHILE i < 3 DO
  BEGIN
    write(i);
    i := i + 1;
  END;
  writeln;
END.
// Expected Output:
// 0
// 1
// 2