// Test 27: Passing Array Element as Parameter
// Tests if an array element can be evaluated and passed as a value to a procedure.

PROGRAM Test27;
VAR
  global_arr: ARRAY[1..3] OF INTEGER;

PROCEDURE PrintValue(val: INTEGER);
BEGIN
  writeln('Value passed to procedure:');
  writeln(val);
END;

BEGIN
  global_arr[1] := 11;
  global_arr[2] := 22;
  global_arr[3] := 33;
  PrintValue(global_arr[2]);
END.

// Expected Output:
// Value passed to procedure: 22