// Test 23: Nested IF Statements
// Verifies correct branching with nested conditional logic.

PROGRAM Test23;
VAR
  a, b: INTEGER;
BEGIN
  a := 10;
  b := 10;
  IF a = 10 THEN
  BEGIN
    writeln('Outer IF is true');
    IF b > 15 THEN
    BEGIN
      writeln('Inner IF is true');
    END
    ELSE
      writeln('Inner IF is false');
  END
  ELSE
    writeln('Outer IF is false');
END.

{
Expected Output:
Outer IF is true
Inner IF is false
}