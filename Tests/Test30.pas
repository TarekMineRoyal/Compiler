// Test 30: WHILE Loop with False Condition
// Ensures a WHILE loop's body is not executed if the condition is initially false.

PROGRAM Test30;
VAR
  x : INTEGER;
BEGIN
  x := 100;
  WHILE FALSE DO
    x := 0; // This should not execute
  
  writeln(x);
END.

// Expected Output: 100