PROGRAM ArrayErrors;
VAR
  arr1: ARRAY[1..10] OF INTEGER;
  x: INTEGER;
BEGIN
  x := arr1; // Error: Cannot assign whole array to integer
  arr1[5.5] := 1; // Error: Array index must be an integer
  x[2] := 10; // Error: 'x' is not an array
END.

{
Error Test 14: Invalid Array Operations
Tests multiple types of invalid array usage.
Expected Error(s):
Semantic Error (L:6, C:13): Type mismatch in assignment to 'x'. Cannot assign type Array to variable of type Integer.
Semantic Error (L:7, C:8): Array index for 'arr1' must be an INTEGER expression, but found Real.
Semantic Error (L:8, C:3): Identifier 'x' is of type Integer and cannot be indexed as an array.
}