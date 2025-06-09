PROGRAM FullTest;
VAR
  x, y, z: INTEGER;
  
FUNCTION addition(a: INTEGER; b: INTEGER): INTEGER;
  VAR
    result: INTEGER;
  BEGIN
    result := a + b;
    RETURN result;
  END;

BEGIN
  x := 15;
  y := 27;
  z := addition(x, y);
  write('The result of add(15, 27) is: ', z);
  writeln;
END.

// Expected Output: 42