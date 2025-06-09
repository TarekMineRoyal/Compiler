// Test 5: Integer and Real Division
// Tests REAL division (/) and INTEGER division (DIV).

PROGRAM Test5;
VAR
  r_result: REAL;
  i_result: INTEGER;
BEGIN
  r_result := 10 / 4;  // 2.5
  i_result := 10 DIV 4; // 2
  writeln(r_result);
  writeln(i_result);
END.

// Expected Output:
// 2.5
// 2