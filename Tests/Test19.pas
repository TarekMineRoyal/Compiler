// Test 19: Array Access with a Variable Index
// Tests using a variable to index into an array.

PROGRAM Test19;
VAR
  my_array: ARRAY[1..5] OF INTEGER;
  idx, val: INTEGER;
BEGIN
  my_array[1] := 10;
  my_array[2] := 20;
  my_array[3] := 30;
  idx := 3;
  val := my_array[idx];
  writeln(val);
END.

// Expected Output: 30