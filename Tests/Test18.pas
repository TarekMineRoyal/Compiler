// Test 18: Global Array Declaration and Access
// Tests declaration, assignment, and retrieval for a global array.

PROGRAM Test18;
VAR
  my_array: ARRAY[0..4] OF INTEGER;
BEGIN
  my_array[2] := 88;
  writeln(my_array[2]);
END.

// Expected Output: 88