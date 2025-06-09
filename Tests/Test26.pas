// Test 26: Empty BEGIN-END Block
// Ensures the parser and generator can handle empty compound statements.

PROGRAM Test26;
PROCEDURE EmptyBody;
BEGIN
  // This space is intentionally left blank
END;

BEGIN
  writeln('Before call');
  EmptyBody;
  writeln('After call');
END.

{
Expected Output:
Before call
After call
}