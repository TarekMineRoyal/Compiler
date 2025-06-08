PROGRAM ComplexArrayIndexTest;
VAR
  myArray: ARRAY [0..10] OF INTEGER;
  i: INTEGER;
  offset: INTEGER;
BEGIN
  i := 3;
  offset := 2;

  // Store the value 99 at index (3 * 2) + 1 = 7
  myArray[i * offset + 1] := 99;

  // Read back from the same calculated index and print it
  write('Value at calculated index 7 should be 99: ');
  write(myArray[i * offset + 1]);
  writeln;
END.
