// Test 28: Relational Operators with REALs
// Tests that >, <, and = work correctly with REAL values.

PROGRAM Test28;
BEGIN
  IF 5.5 > 5.4 THEN
    writeln('Test 1 Passed');
  
  IF 99.8 < 99.9 THEN
    writeln('Test 2 Passed');

  IF NOT (3.14 = 3.15) THEN
    writeln('Test 3 Passed');
END.

{
Expected Output:
Test 1 Passed
Test 2 Passed
Test 3 Passed
}