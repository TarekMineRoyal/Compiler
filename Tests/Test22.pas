// Test 22: Read Multiple Values
// Tests the ability to read into multiple variables in a single `read` call.

PROGRAM Test22;
VAR
  a: INTEGER;
  b: REAL;
BEGIN
  writeln('Enter an integer, then a real number:');
  read(a, b);
  writeln('Your values were:');
  writeln(a);
  writeln(b);
END.

{
Enter an integer, then a real number:
(User enters 42, then 99.5)
Your values were:
42
99.5
}