PROGRAM RealArrayTest;
VAR
  temps: ARRAY [1..3] OF REAL;
BEGIN
  temps[1] := 98.6;
  temps[2] := 32.5 + 5.5; // Test expression assignment
  temps[3] := temps[1] - 10.0;

  write('Temp 1: ', temps[1]);
  writeln;
  write('Temp 2: ', temps[2]);
  writeln;
  write('Temp 3: ', temps[3]);
  writeln;
END.