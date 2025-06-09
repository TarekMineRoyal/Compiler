// Test 21: Read a Single Integer
// Verifies the implementation of the `read` procedure for a single integer.

PROGRAM Test21;
VAR
  input_val: INTEGER;
BEGIN
  writeln('Enter an integer:');
  read(input_val);
  writeln('Value read * 2 =');
  writeln(input_val * 2);
END.

{
Expected Output:
Enter an integer:
(User enters 15)
Value read * 2 =
30
}